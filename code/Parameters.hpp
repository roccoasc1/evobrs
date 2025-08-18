// Parameters.hpp
#ifndef PARAMETERS_HPP // include guard
#define PARAMETERS_HPP

#include <functional>
#include <random>
#include <random>
#include <memory>

constexpr std::size_t POPSIZE = 100;
constexpr int POPELITESIZE = 5;
constexpr unsigned int MAXFITEVALS = 100000;

constexpr std::size_t n = 6;
constexpr std::size_t n_pow = 1<<n;
constexpr unsigned int MIN_ENTITIES = std::ceil((float)n / 2);

//=== CONFIGURABLE PARAMETERS BEGIN ===
constexpr unsigned int INITSIZEMIN=80;
constexpr unsigned int INITSIZEMAX=100;
//=== CONFIGURABLE PARAMETERS END ===
constexpr double PC = 0.2;
constexpr double PRMIN = 0.8;
constexpr double PREN = 0.1;


// PC    PRMIN      PIN 
// 0.0   0.8        0.2
// 0.2   0.8        0.0/0.2
// 0.5   0.2/0.4    0.0/0.2
// 0.8   0.2        0.0/0.2

inline std::mt19937 RNG;
static std::uniform_real_distribution<double> rand_01(0.0, 1.0);
static std::bernoulli_distribution rand_bool(0.5);
static std::uniform_int_distribution<std::size_t> rand_entities(MIN_ENTITIES, n);
static std::uniform_int_distribution<std::size_t> rand_n_reactions(INITSIZEMIN, INITSIZEMAX);

#endif /* PARAMETERS_HPP */
