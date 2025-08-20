// EvoBRS.hpp
#ifndef EVORS_HPP
#define EVORS_HPP

#include "ReactionSystem.hpp"
#include <vector>
#include <functional>
#include <random>
#include <algorithm>
#include <iostream>

class EvoBRS {
private:
    const std::size_t entities{n};
    std::function<double(const BRS&)> fit_func;

public:
    std::vector<BRS> pop;
    std::vector<BRS> popelite;
    BRS bestRS, medianRS, worstRS;
    int n_unique_elements;
    int n_reaction_median;

    EvoBRS(std::function<double(const BRS&)> _fit);

    void evolutionCycle(bool verbose = false);
    void truncatedSelection(bool verbose = false);
    void mutationInsertRemove();
    void mutationInsert();
    void mutationRenewal();
    void crossoverPopulation();
    void minimization();
    void ordering();

    double fitness(BRS& rs) const;

    friend std::ostream& operator<<(std::ostream& os, const EvoBRS& evo);
};

#endif
