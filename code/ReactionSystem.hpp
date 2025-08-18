#ifndef REACTIONSYSTEM_HPP
#define REACTIONSYSTEM_HPP

#include <vector>
#include <string>
#include <functional>
#include <unordered_set>
#include <unordered_map>
#include <random>
#include <algorithm>
#include "BoolReaction.hpp"

class BRS {
private:
    const std::size_t entities{n};
    std::vector<BoolReaction> reactions;
    std::function<double(const BRS&)> fit_func;
    double fitness = (double)(-n_pow);
public: 
    BRS(); // Default constructor
   
    BRS(const BRS& other) = default; // Copy constructor
    BRS(BRS&& other) noexcept = default; // Move constructor
    
    BRS& operator=(const BRS& other); // Copy assignment operator
    BRS& operator=(BRS&& other) noexcept; // Move assignment operator

    ~BRS() = default; // Destructor

    // Custom constructors
    BRS(const std::vector<BoolReaction>& reactions,std::function<double(const BRS&)>& fit);
    BRS(std::vector<BoolReaction>&& reactions,std::function<double(const BRS&)>& fit);
    BRS(const std::size_t& n_reactions,std::function<double(const BRS&)>& fit); // random constructor
   

    void update_fitness();
    double get_fitness() const;

    std::size_t getentities() const;
    std::vector<BoolReaction>& getreactions();
    const std::vector<BoolReaction>& getreactions() const;

    void order();
    const size_t n_reactions() const;
    void reserve_n_reactions(const int n_reserve);
    void removeReaction(const BoolReaction& r);
    void removeReactions(const std::vector<BoolReaction>& rs);
    void removeReactions(std::vector<int>& index);
    void addReaction(const BoolReaction& r);
    void addReactions(const std::vector<BoolReaction>& rs);
    void addReactions(std::vector<BoolReaction>&& rs);
    void minimize();
    

    //std::unordered_map<int, int> sizesOfReactions() const;
    bool res(const std::bitset<n>& T) const;
    BoolFunction boolf() const;

    bool operator==(const BRS& other) const; //cannot be const because I need to order the reactions
    bool operator!=(const BRS& other) const {return !(operator==(other)); }
    
    //bool operator<=(const BRS& other) const;
    bool operator<(const BRS& other) const;

    friend std::ostream& operator<<(std::ostream& os, const BRS& rs);  
};

#endif
