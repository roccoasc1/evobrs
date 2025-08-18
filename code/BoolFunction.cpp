#include "BoolFunction.hpp"
#include <iostream>

BoolFunction::BoolFunction(const std::bitset<(1 << n)>& truth_table): truth_table(truth_table){
    //assert that the trhuth table is a power of 2
    //n_input = static_cast<int>(std::log2(truth_table.size()));
};

void BoolFunction::printTruthTable(){
        for (int i=0; i< truth_table.size(); i++) {
            std::cout << truth_table[i] << " ";
        }
        std::cout << std::endl;
    }

std::vector<int> BoolFunction::getTruthTablePolar() const {
    static std::vector<int> polar(truth_table.size());
    for (auto i=0; i<truth_table.size(); i++){
        polar[i] = 2*truth_table[i] -1;
    }
    return polar;
}

int BoolFunction::hammingWeight() const {
    return truth_table.count();
}

void BoolFunction::fwht(std::vector<int>& a) {
    int n = (int)a.size();
    for (int len = 1; 2 * len <= n; len <<= 1) {
        for (int i = 0; i < n; i += 2 * len) {
            for (int j = 0; j < len; ++j) {
                int u = a[i + j];
                int v = a[i + j + len];
                a[i + j] = u + v;
                a[i + j + len] = u - v;
            }
        }
    }
}

int BoolFunction::n_max_values_FWT() const {
    auto vec = getTruthTablePolar();
    fwht(vec);
    for (auto& val : vec) {
        val = std::abs(val);
    }
    int max_value = *std::max_element(vec.begin(), vec.end());
    int n_max_value = std::count(vec.begin(), vec.end(), max_value);
    return n_max_value;
}

double BoolFunction::fit_BAL_max_values() const{
    if (unbalancedness() > 0){
        return -unbalancedness();
    }
    return nonlinearity()+(double)((1 << n_input)-n_max_values_FWT())/(1 << n_input);
}

double BoolFunction::fit_BENT_max_values() const {
        return nonlinearity()+(double)((1 << n_input)-n_max_values_FWT())/(1 << n_input);
}

int BoolFunction::computeFWT(std::vector<int>& vector, int start, int length) const {
    int half = length / 2;
    for (int i = start; i < start + half; ++i) {
        int temp = vector[i];
        vector[i] += vector[i + half];
        vector[i + half] = temp - vector[i + half];
    }

    if (half > 1) {
        int val1 = computeFWT(vector, start, half);
        int val2 = computeFWT(vector, start + half, half);
        return std::max(val1, val2);
    } else {
        return std::max(std::abs(vector[start]), std::abs(vector[start + half]));
    }
}

int BoolFunction::spectralRadius() const {
    auto vec = getTruthTablePolar();
    return computeFWT(vec, 0, vec.size());
}

int BoolFunction::nonlinearity() const {
    return (1 << (n_input - 1)) - spectralRadius() / 2;
}

int BoolFunction::unbalancedness() const {
    return std::abs((1 << (n_input - 1)) - hammingWeight());
}

std::ostream& operator<<(std::ostream& os, const BoolFunction& f){
    for (int i=0; i< f.truth_table.size(); i++) {
        os << f.truth_table[i];
    }
    return os;
}; 