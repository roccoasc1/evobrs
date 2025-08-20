#include "BoolReaction.hpp"
#include <sstream>
#include <random>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <iostream>

#include <cassert>

// ===== BoolReaction =====

BoolReaction::BoolReaction() 
    : entities(n), R(), I() {}

BoolReaction::BoolReaction(std::bitset<n> R, std::bitset<n> I): R(R),I(I) {
    if (R.size() != I.size()) {
        throw std::invalid_argument("Size of R and I must match size.");
    }
    for (int i = 0; i < R.size(); i++){
        if (R[i] && I[i]) {
            throw std::invalid_argument("A symbol cannot be both a reactant and an inhibitor.");
        }
    }
    entities = R.size();
    };

BoolReaction::BoolReaction(std::bitset<n> support) {
    for (int i = 0; i < n; i++) {
        if (support[i]){
            if (rand_bool(RNG)){
                R[i] = true;
            }
            else {
                I[i] = true;
            }
            
        }
    }
};

BoolReaction::BoolReaction(const std::size_t size) {
    //std::cout << "[BoolReaction::SizeCtor] size=" << size << " entities=" << entities << std::endl;
    if (size < 1) {
        throw std::invalid_argument("Size must be at least 1.");
    }
    if (size > n){
        throw std::invalid_argument("The number of entities must be greater  or equal than the size.");
    }
    std::array<bool,n> newsupport{};
    //std::cout << size << entities << std::endl;

    // Set the first k entries to true (1)
    for (int i = 0; i < size; ++i) {
        newsupport[i] = true;
    }

    // Shuffle the vector randomly
    std::shuffle(newsupport.begin(), newsupport.end(), RNG);
    // vedere se questo for si può sustituire con BoolReaction(support) convertendo new support in a bitset
    for (int i = 0; i < n; i++) {
        if (newsupport[i]){
            if (rand_bool(RNG)){
                R[i] = true;
            }
            else {
                I[i] = true;
            }
            
        }
    }
    //assert(std::count (newsupport.begin(), newsupport.end(), true)==size);
};

int BoolReaction::getsize() const { 
    int n_reactants = R.count();    
    int n_inhibitors = I.count();
    return n_reactants + n_inhibitors; }

std::size_t BoolReaction::getentities() const { return entities; }

std::bitset<n> BoolReaction::getsupport() const { 
    std::bitset<n> support;
    for (int i = 0; i < R.size(); i++){
        if (R[i] && I[i]) {
            throw std::invalid_argument("A symbol cannot be both a reactant and an inhibitor.");
        }
        support[i] = R[i] | I[i];
    }
    return support; }
std::bitset<n> BoolReaction::getR() const { return R; }
std::bitset<n> BoolReaction::getI() const { return I; }

bool BoolReaction::isEnabled(const std::bitset<n>& T) const {
    if (T.size() != R.size() || T.size() != I.size()) {
        std::cout<< T.size() << R.size() << I.size() <<"\n";
        throw std::invalid_argument("Size of T must match size of R and I");
    }
    for (int i = 0; i < R.size(); i++) {
        if (R[i] && !T[i]) return false; // If R is true but T is false, reaction is not enabled
        if (I[i] && T[i]) return false;  // If I is true and T is also true, reaction is not enabled
    }
    return true; // All conditions for enabling the reaction are satisfied
}

bool BoolReaction::operator==(const BoolReaction& other) const {
    return R == other.R && I == other.I;
}

bool BoolReaction::operator<=(const BoolReaction& other) const {
    return (*this < other) || (*this == other);
}

bool BoolReaction::operator<(const BoolReaction& other) const {
    //std::cout << "entities=" << entities << ", other.entities=" << other.entities << std::endl;
    if (entities != other.entities){
        std::cerr << "Comparison error: entities=" << this->entities << ", other.entities=" << other.entities << std::endl;
        return false;
        //std::cout << "self " << (*this);
        //std::cout << "other " << other;
        throw this;
        
        
    }
    if (getsize() > other.getsize()){ return false;}
    if (getsize() < other.getsize()){ return true;}

    unsigned int thisR_int = R.to_ulong();
    unsigned int otherR_int = other.R.to_ulong();
    if (thisR_int < otherR_int){ return true; }
    if (thisR_int > otherR_int){ return false; }

    unsigned int thisI_int = I.to_ulong();
    unsigned int otherI_int = other.I.to_ulong();
    if (thisI_int < otherI_int){ return true; }
    
    return false; 
}

bool BoolReaction::operator!=(const BoolReaction& other) const{
    return !operator==(other);
}


std::ostream& operator<<(std::ostream& os, const BoolReaction& r)
{   
    os  << "(";
    //reactants
    os << "{";
    auto start = 1;
    for (size_t i = 0; i <r.R.size(); ++i) {
        if (r.R[i]) {
            if (start){
                os << "x_"<<i;
                start=0;
            }
            else {
                os << ",x_"<<i;
            }
        }
    }
    os << "}, ";
    //inhibitors
    os << "{";
    start = 1;
    for (size_t i = 0; i <r.I.size(); ++i) {
        if (r.I[i]) {
            if (start){
                os << "x_"<<i;
                start=0;
            }
            else {
                os << ",x_"<<i;
            }
        }
    }
    os << "}, {True})";
    return os;
}

bool BoolReaction::le(const BoolReaction& other) const{
    if (entities != other.entities){
        std::cout << "self" << (*this);
        std::cout << "other" << other;
        std::cerr << "Comparison error: entities=" << entities << ", other.entities=" << other.entities << std::endl;
        throw std::invalid_argument("Cannot le-compare reactions with different entities.");
    }
    for(int i=0; i<entities; i++){
        if (R[i] && !other.R[i]){ 
            return false;
        }
        if (I[i] && !other.I[i]){ 
            return false;
        }
    }
    return true;
}

bool BoolReaction::supportless(const BoolReaction& other) const{
    if (entities != other.entities){
        std::cout << "self" << (*this);
        std::cout << "other" << other;
        std::cerr << "Comparison error: entities=" << entities << ", other.entities=" << other.entities << std::endl;
        throw std::invalid_argument("Cannot le-compare reactions with different entities.");
    }

    unsigned int support_int = getsupport().to_ulong();
    unsigned int othersupport_int = other.getsupport().to_ulong();
    if (support_int < othersupport_int){ return true; }
    return false;
}