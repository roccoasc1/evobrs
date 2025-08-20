#ifndef BOOLFUNCTION_HPP
#define BOOLFUNCTION_HPP

#include <vector>
#include <cmath>
#include <algorithm>
#include <bitset>
#include <functional>
#include "../Parameters.hpp"

class BoolFunction {
private:
    static constexpr int n_input{n};
    const std::bitset<n_pow> truth_table;
public:
    BoolFunction(const std::bitset<n_pow>& truth_table);

    BoolFunction(const BoolFunction& other) = default; // copy constructor
    BoolFunction(BoolFunction&& other) noexcept = default; // move constructor
    BoolFunction& operator=(const BoolFunction& other) = default; // copy assigment
    BoolFunction& operator=(BoolFunction&& other) noexcept = default; // move assigment

    int get_n_input(){return n_input;};

    std::vector<int> getTruthTablePolar() const;

    int hammingWeight() const;
    int computeFWT(std::vector<int>& vector, int start, int length) const;
    
    static void fwht(std::vector<int>& a);
    int n_max_values_FWT() const;
    int spectralRadius() const;
    int nonlinearity() const;
    int unbalancedness() const;

    void printTruthTable();

    double fit_BAL_max_values() const;
    double fit_BENT_max_values() const;

    friend std::ostream& operator<<(std::ostream& os, const BoolFunction& f); 
};

#endif
