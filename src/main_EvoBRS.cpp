#include "EvoBRS.hpp"
#include <iostream>
#include <chrono>

static unsigned int fit_evals = 0;

double FitnessBAL(const BRS& rs) {
    fit_evals ++;
    return rs.boolf().fit_BAL_max_values(); 
}

double FitnessBENT(const BRS& rs) {
    fit_evals ++;
    return rs.boolf().fit_BENT_max_values(); 
}

int main(int argc, char * argv[]){
    
    auto start_time = std::chrono::steady_clock::now();

    unsigned int run{1234}; 
    if(argc>1){
        std::cout << "Input seed: "<< std::stoul(argv[1]) <<std::endl;
        run = std::stoul(argv[1]);
    } else {
        std::cout << "Default seed: "<< run <<std::endl;
    }    
    
    RNG.seed(run);

    std::cout << "gen,bestfit,medianfit,uniqueRS,n_reactions_median,n_reactions_mean,n_reactions_std\n";

    const std::function<double(const BRS&)> fit = FitnessBAL;
    
    EvoBRS evo(fit);

    const unsigned int ERAS = MAXFITEVALS / POPSIZE; // upperbound
    const unsigned int print_era = 100;
    std::array<double,print_era> bestfit,medianfit;
    std::array<int,print_era> uniqueRS;
    std::array<double,print_era> n_reactions_median;//,n_reactions_mean;//,n_reactions_std;

    unsigned int gen = 0;
    while(fit_evals < MAXFITEVALS){
        
        evo.evolutionCycle(false);

        if (gen %print_era == 0 && gen>1){
        for (int i = 0; i < print_era; ++i) { 
            std::cout << (gen-print_era)+i <<"," << bestfit[i]<<"," << medianfit[i]<<"," << uniqueRS[i] << "," << n_reactions_median[i] <<"\n";
        }
        }
        
        auto index = gen%print_era;
        bestfit[index] = evo.bestRS.get_fitness();
        medianfit[index] = evo.medianRS.get_fitness();
        uniqueRS[index] = evo.n_unique_elements;
        n_reactions_median[index] = evo.n_reaction_median;
        
        gen++;
    }

    for (int i = 0; i < gen%print_era; ++i) { 
        std::cout << (gen - gen%print_era) + i <<"," << bestfit[i]<<"," << medianfit[i]<<"," << uniqueRS[i] << "," << n_reactions_median[i] <<"\n";
    }

    auto end_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = end_time - start_time;

    
    std::cout << "\nPARAMETERS:"<<
        "\n  fit evaluations: "<<fit_evals <<
        "\n  eras: "<<gen <<
        "\n  popsize: "<<POPSIZE<<
        "\n  n: " << n << 
        "\n  INITSIZEMIN:"<< INITSIZEMIN << 
        "\n  INITSIZEMAX:"<< INITSIZEMAX <<
        "\n  PC: " << PC <<
        "\n  PRMIN: " << PRMIN <<
        "\n  PREN: " << PREN;

    std::cout << "\n\nFinished in " << duration.count() << " seconds."<<
    "\n\nBest RS found:\n";

    evo.truncatedSelection();
    std::cout << evo.bestRS;
    std::cout << "Truth Table: "<< evo.bestRS.boolf() << "\n";
    std::cout << "Fitness: "<< evo.bestRS.get_fitness() << "\n";
    std::cout << "Non linearity: "<< evo.bestRS.boolf().nonlinearity() << std::endl;
    std::cout << "Unbalancedness: "<< evo.bestRS.boolf().unbalancedness() << std::endl;
    return 0;
}
