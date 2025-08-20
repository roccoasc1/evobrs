#ifndef BOOLREACTION_HPP
#define BOOLREACTION_HPP

#include <set>
#include <vector>
#include <string>
#include <functional>
#include <unordered_set>
#include <unordered_map>
#include <random>
#include <iostream>
#include <algorithm>
#include "BoolFunction.hpp"

class BoolReaction {
private:
    std::bitset<n> R,I;
    std::size_t entities{n};
public:
    BoolReaction();
    BoolReaction(std::bitset<n> R,std::bitset<n> I);
    BoolReaction(std::bitset<n> support);
    BoolReaction(const std::size_t size);
    
    // Copy constructor and assignment
    BoolReaction(const BoolReaction&) = default;
    BoolReaction& operator=(const BoolReaction&) = default;

    // Move constructor and assignment
    BoolReaction(BoolReaction&&) noexcept= default;
    BoolReaction& operator=(BoolReaction&&) noexcept= default;

    int getsize() const;
    std::size_t getentities() const;
    std::bitset<n> getsupport() const;
    std::bitset<n> getR() const;
    std::bitset<n> getI() const;

    bool isEnabled(const std::bitset<n>& T) const;

    bool operator==(const BoolReaction& other) const;
    bool operator!=(const BoolReaction& other) const;
    bool operator<=(const BoolReaction& other) const;
    bool operator<(const BoolReaction& other) const;

    bool le(const BoolReaction& other) const;

    bool supportless(const BoolReaction& other) const;

    friend std::ostream& operator<<(std::ostream& os, const BoolReaction& r);    
};

#endif
