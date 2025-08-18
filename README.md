# Evolutionary Boolean Reaction Systems (EvoBRS)
Evolutionary Boolean Reaction Systems is a new algorithm paradigm to construct bent and highly non linear balanced Boolean functions.

## Repository Structure

- **`code/`**  
  Contains all headers (`.hpp`) and source (`.cpp`) files implementing the following classes:  
  - `BoolReaction`: encodes reactions over Boolean entities with reactants and inhibitors (`R` and `I` bitsets);
  - `ReactionSystem` (class `BRS`): models a Boolean Reaction System as a collection of `BoolReaction` objects;
  - `EvoBRS`: implements the EvoBRS algorithm.
  - `BoolFunction`: represents Boolean functions as truth tables, providing methods to compute the Fast Walsh–Hadamard Transform (FWHT), Hamming weight, non linearity and unbalancedness;

- **`experiments/`**  
  Stores experiments and test data.  

- **`Parameters.hpp`**  
  Header file to set parameters before building the code.  

- **`main_EvoBRS.cpp`**  
  Main file to run the EvoRS algorithm.  


## Requirements

C++17-compatible compiler (e.g. `g++` 7.0+ or `clang++` 6.0+).  

## Build & Run

From the project’s base folder, compile using:

```bash
g++ -std=c++17 -O3 -o EvoBRS.exe ./code/*.cpp ./main_EvoBRS.cpp
```

and execute the program with:

```bash
./EvoBRS.exe INPUT_SEED
```

where INPUT_SEED is an integer used to initialise the random number generator.

