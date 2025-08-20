#include "ReactionSystem.hpp"
#include <sstream>
#include <random>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <cassert>

// ===== BRS =====
BRS::BRS()
    : reactions(), entities(n) {}

BRS::BRS(const std::vector<BoolReaction>& reactions_,std::function<double(const BRS&)>& fit)
    : reactions(reactions_), fit_func(fit) {
    }

BRS::BRS(std::vector<BoolReaction>&& reactions_,std::function<double(const BRS&)>& fit):
    reactions(std::move(reactions_)),fit_func(fit){
    }

BRS::BRS(const std::size_t& n_reactions,std::function<double(const BRS&)>& fit): fit_func(fit){
    reactions.reserve(INITSIZEMAX);
    for(int i = 0; i< n_reactions; i++){
        reactions.emplace_back(rand_entities(RNG));
    }
    
    // check for minimal reactions with the same support
    std::sort(reactions.begin(), reactions.end(), [](const BoolReaction& a, const BoolReaction& b) { 
        return a.supportless(b); });

    std::vector<int> toremove;
    toremove.reserve(reactions.size());
    
    for(int i=1; i<reactions.size() && (reactions[i].getsize() == MIN_ENTITIES); ++i){
        if (reactions[i].getsupport() == reactions[i-1].getsupport()){
            toremove.push_back((i-1));
        }
    }

    if (toremove.size()>0){
        removeReactions(toremove);
    }
}

BRS& BRS::operator=(const BRS& other) {
    if (this != &other) {
        reactions = other.reactions;
        fitness = other.fitness;
        fit_func = other.fit_func;
    }
    return *this;
} // Copy assignment operator

BRS& BRS::operator=(BRS&& other) noexcept {
    if (this != &other) {
        reactions = std::move(other.reactions);
        fitness = other.fitness;
        fit_func = std::move(other.fit_func); // in move
    }
    return *this;
} // Move assignment operator

std::size_t BRS::getentities() const{
    return entities;
}

void BRS::update_fitness(){
    fitness = fit_func(*this);
}

double BRS::get_fitness() const{
    return fitness;
}

std::vector<BoolReaction>& BRS::getreactions(){
    return reactions;
}
const std::vector<BoolReaction>& BRS::getreactions() const {
    return reactions;
}

void BRS::order() {
    std::sort(reactions.begin(), reactions.end(), [](const BoolReaction& a, const BoolReaction& b) {
        return a<=b; });
}

const size_t BRS::n_reactions() const {
    return reactions.size();
}

void BRS::reserve_n_reactions(const int n_reserve){
    reactions.reserve(n_reserve);
};

void BRS::removeReaction(const BoolReaction& r) {
    reactions.erase(std::remove(reactions.begin(), reactions.end(), r), reactions.end());
}

void BRS::removeReactions(const std::vector<BoolReaction>& rs) {
    for (const auto& r : rs) {
        removeReaction(r);
    }
}

void BRS::removeReactions(std::vector<int>& index) {
    // first remove the indices with higher values so that the lower one still are valid
    std::sort(index.begin(),index.end(),[](int a, int b) {return a > b;}); 
    
    for (const auto& i : index) {
        reactions.erase(reactions.begin()+i);
    }
}

void BRS::addReactions(const std::vector<BoolReaction>& rs) {
    for (const auto& r : rs) {
        assert(r.getentities() == entities && "Trying to add a reaction with mismatched entities!");
    }
    reactions.insert(reactions.end(), rs.begin(), rs.end());
}

void BRS::addReactions(std::vector<BoolReaction>&& rs) {
    for (const auto& r : rs) {
        assert(r.getentities() == entities && "Trying to add a reaction with mismatched entities!");
    }
    reactions.insert(reactions.end(),
                     std::make_move_iterator(rs.begin()),
                     std::make_move_iterator(rs.end()));
}

void BRS::addReaction(const BoolReaction& r) {
    assert(r.getentities() == entities && "Trying to add a reaction with mismatched entities!");
    reactions.push_back(r);
}

void BRS::minimize() {
    const size_t n_reacts = n_reactions();
    std::vector<bool> toRemove_mask(n_reacts, false);

    //auto bf_before = this->boolf();

    
    for (size_t i = 0; i < n_reacts; ++i) {
        if (toRemove_mask[i]) continue;
        for (size_t j = i+1; j <n_reacts; ++j) {
            if (toRemove_mask[j]) continue;
            if (reactions[i].le(reactions[j])) {
                toRemove_mask[i] = true;
                break;  // i is subsumed, skip to next i
            } else if (reactions[j].le(reactions[i])) { 
                toRemove_mask[j] = true;
                // no break: keep checking if i can be subsumed further
            }
        }
    }

    std::vector<int> toRemove; // save the indicies of the reactions to remove
    toRemove.reserve(n_reacts);

    for (size_t i = 0; i < n_reacts; ++i) {
        if (toRemove_mask[i]) {
            toRemove.push_back(static_cast<int>(i));
        }
    }   

    removeReactions(toRemove);
}

bool BRS::res(const std::bitset<n>& T) const {
    for (const auto& r : reactions) {
        if (r.isEnabled(T)){
            return true;
        }
    }
    return false;
}

BoolFunction BRS::boolf() const{
    std::bitset<n_pow> TT;
    for(int t=0; t<TT.size(); t++){
        std::bitset<n> T;
        for (std::size_t i = 0; i < entities; ++i) {
            T[i] = (t >> i) & 1;
        }
        TT[t] = res(T);
    }
    return BoolFunction(TT);
}

bool BRS::operator==(const BRS& other) const {
    if (entities == other.entities && reactions.size() == other.reactions.size())
    {
        auto a = reactions, b = other.reactions;
        std::sort(a.begin(), a.end(),[](const BoolReaction& r1, const BoolReaction& r2) {return r1 < r2; });
        std::sort(b.begin(), b.end(),[](const BoolReaction& r1, const BoolReaction& r2) {return r1 < r2; }); 

        for (size_t i = 0; i < a.size(); ++i) {
            if (a[i] != b[i]) return false;
        }
        return true;
    }
    return false;
}

bool BRS::operator<(const BRS& other) const {
    if (reactions.size() != other.reactions.size()) {
        return reactions.size() < other.reactions.size();
    }

    std::vector<BoolReaction> a = reactions;
    std::vector<BoolReaction> b = other.reactions;
    std::sort(a.begin(), a.end(),[](const BoolReaction& r1, const BoolReaction& r2) {return r1 < r2; });
    std::sort(b.begin(), b.end(),[](const BoolReaction& r1, const BoolReaction& r2) {return r1 < r2; });
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i] == b[i]){ continue; }
        if (a[i] < b[i]) { return true; }
        return false;
    }

    return false;
}

std::ostream& operator<<(std::ostream& os, const BRS& rs){
    os << "Number of reactions: " << rs.n_reactions() << "\n";
    for (const auto& r : rs.reactions) {
        os << " [size=" << r.getsize() << "] " << r << "\n";
    }
    return os;
}