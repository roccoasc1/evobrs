// EvoBRS.cpp
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <ranges>
#include <cassert>
#include "EvoBRS.hpp"

template<class T>
double calculateMean(std::array<T,POPSIZE>& arr)
{
    double sum = 0.0;
    int size = arr.size();
    // Calculate the sum of elements in the vector
    for (int i = 0; i < size; ++i) {
        sum += arr[i];
    }
    // return the mean
    return (double)sum / size;
}


template<class T>
double calculateStandardDeviation(std::array<T,POPSIZE>& arr, double mean)
{
    double standardDeviation = 0.0;
    int size = arr.size();
    // Calculate the sum of squared differences from the mean
    for (int i = 0; i < size; ++i) {
        standardDeviation += pow(arr[i] - mean, 2);
    }
    // Calculate the square root of the variance to get the
    // standard deviation
    return sqrt(standardDeviation / size);
}


EvoBRS::EvoBRS(std::function<double(const BRS&)> _fit)
    : fit_func(_fit)
{   
    pop.reserve(POPSIZE*4);
    popelite.reserve(POPELITESIZE);

    for (int i = 0; i < POPSIZE; ++i) {
        pop.emplace_back(rand_n_reactions(RNG),fit_func);
        pop[i].update_fitness();
        //fitness(pop[i]);
    }
    // order the population
    ordering();
    n_unique_elements = static_cast<int>(pop.size());
    bestRS = pop[0];
    worstRS = pop.back();
    medianRS = pop[POPSIZE / 2];

    popelite.assign(pop.begin(), pop.begin() + POPELITESIZE);
}

double EvoBRS::fitness(BRS& rs) const {
    return fit_func(rs);
}

void EvoBRS::ordering() {
    std::sort(pop.begin(), pop.end(), [this](const BRS& a, const BRS& b) {
        return a.get_fitness() > b.get_fitness();
    });
}

void EvoBRS::truncatedSelection(bool verbose) {
    pop.insert(pop.end(), popelite.begin(), popelite.end()); // add the popelite population

    //erase equal reaciton system
    std::set<BRS> set_pop(pop.begin(), pop.end());
    std::vector<BRS> uniquepop;
    uniquepop.reserve(POPSIZE*3);
    for (const auto& brs : set_pop) {
        uniquepop.emplace_back(brs);
    }
    pop = std::move(uniquepop);

    ordering(); //ordering by fitness
    
    if ((int)pop.size() > POPSIZE) {
        pop.resize(POPSIZE);
    }

    n_unique_elements = static_cast<int>(pop.size());
    
    bestRS = pop[0];
    worstRS = pop.back();
    medianRS = pop[pop.size() / 2];
    
    popelite.assign(pop.begin(), pop.begin() + POPELITESIZE);
    
    if (verbose) {
        std::cout << "  Number of unique RS: " << pop.size() << "\n";
        std::cout << "  Fitness of top individuals: ";
        for (int i = 0; i < std::min(POPELITESIZE + 5, (int)pop.size()); ++i) {
            std::cout << pop[i].get_fitness() << " ";
        }
        std::cout << "\n  Fitness (max/median/min): "
                  << bestRS.get_fitness() << ", "
                  << medianRS.get_fitness() << ", "
                  << worstRS.get_fitness() << "\n";
    }

    // get median size
    std::array<unsigned int,POPSIZE> sizes;
    for (int i = 0; i < POPSIZE; ++i) {
        sizes[i] = pop[i].n_reactions();
    }
    n_reaction_median = sizes[POPSIZE / 2];
}

void EvoBRS::mutationRenewal() {
    std::vector<BRS> new_mutated;
    for (int i = 0; i < POPSIZE; ++i) {
        if (rand_01(RNG) < PREN) {
            new_mutated.emplace_back(rand_n_reactions(RNG),fit_func);
        }
    }
    pop.insert(pop.end(), std::make_move_iterator(new_mutated.begin()), std::make_move_iterator(new_mutated.end()));
}

void EvoBRS::mutationInsertRemove() {
    std::vector<BRS> new_individuals; 
    for (int i = 0; i < pop.size(); ++i) {
        if (rand_01(RNG) < PRMIN) {
            BRS rs = pop[i];
            int max_rem = rs.n_reactions() / 3;
            if (max_rem == 0) continue;

            int n_sub = std::uniform_int_distribution<>(1, max_rem)(RNG);
            
            std::vector<int> indices(rs.n_reactions());
            std::iota(indices.begin(), indices.end(), 0); // Fill with 0, 1, ..., n-1
            std::shuffle(indices.begin(), indices.end(), RNG);
            
            std::vector<int>to_remove(indices.begin(), indices.begin() + n_sub);
            
            const std::vector<BoolReaction>& reactions = rs.getreactions();

            std::vector<BoolReaction> new_reactions;
            new_reactions.reserve(n_sub); // Reserve space before filling

            for (int rm : to_remove) {
                std::size_t size = reactions[rm].getsize();
                if (size == MIN_ENTITIES){
                    auto oldsupport = reactions[rm].getsupport();
                    //new_reactions.push_back(BoolReaction(oldsupport)); // con questo funzionava bene: avevamo 114!
                    new_reactions.emplace_back(oldsupport);
                } else{
                    new_reactions.emplace_back(size);
                }
            }
            rs.removeReactions(to_remove);
            rs.addReactions(new_reactions);
            auto after = rs.n_reactions();
            new_individuals.emplace_back(std::move(rs.getreactions()), fit_func);
        }
    }
    pop.insert(pop.end(), std::make_move_iterator(new_individuals.begin()), std::make_move_iterator(new_individuals.end()));
}

void EvoBRS::crossoverPopulation() {
    std::vector<BRS> offspring;

    std::vector<int> indices(pop.size());
    std::iota(indices.begin(), indices.end(), 0); // Fill with 0, 1, ..., n-1 // added this line only on 19.06.2025!!!
    std::shuffle(indices.begin(), indices.end(), RNG);

    for (int i = 1; i < POPSIZE; ++i) {
        if (rand_01(RNG) >= PC) continue;
        
        std::array<int,4> selection;
        // Manually select 4 unique random individuals from the population
        
        std::shuffle(indices.begin(), indices.end(), RNG);
        for (int j = 0; j < 4; ++j) {
            selection[j] = indices[j];
        }

        std::sort(selection.begin(), selection.end(), [this](const int& a, const int& b) {
            return pop[a].get_fitness() > pop[b].get_fitness();
        });
    
        BRS& p1 = pop[selection[0]], p2 = pop[selection[1]];

        std::unordered_map<size_t, std::vector<BoolReaction>> map1, map2;
        for (auto& r : p1.getreactions()) {
            map1[r.getsize()].push_back(r);
        }
        for (auto& r : p2.getreactions()){
            map2[r.getsize()].push_back(r);
        } 

        std::vector<size_t> common;
        for (auto& [k, _] : map1) {
            if (map2.find(k) != map2.end()) {
                common.push_back(k);
            }
        }
        if (common.empty()) continue;

        size_t k = common[RNG() % common.size()];
        std::vector<BoolReaction> new1 = std::move(map2[k]);
        std::vector<BoolReaction> new2 = std::move(map1[k]);

        for (auto& [sz, rlist] : map1){
            if (sz != k) {
                new1.insert(new1.end(), rlist.begin(), rlist.end());
            }
        } 
        for (auto& [sz, rlist] : map2) {
            if (sz != k) {
                new2.insert(new2.end(), rlist.begin(), rlist.end());
            }
        }

        offspring.emplace_back(std::move(new1),fit_func);
        offspring.emplace_back(std::move(new2),fit_func);
    }
    pop.insert(pop.end(),
           std::make_move_iterator(offspring.begin()),
           std::make_move_iterator(offspring.end()));
}

void EvoBRS::minimization() {
    for (int i=POPSIZE; i< pop.size();i++){
        pop[i].minimize();
        pop[i].update_fitness();
    }    
}

void EvoBRS::evolutionCycle(bool verbose) {
    truncatedSelection(verbose);
    mutationRenewal();
    crossoverPopulation();
    mutationInsertRemove();
    minimization();
}

std::ostream& operator<<(std::ostream& os, const EvoBRS& evo) {
    for (size_t i = 0; i < evo.pop.size(); ++i) {
        os << "Reaction System " << i << " (fitness " << evo.pop[i].get_fitness() << "):\n"
           << evo.pop[i];
    }
    return os;
}
