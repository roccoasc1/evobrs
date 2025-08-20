# Evolutionary Boolean Reaction Systems (EvoBRS)
Evolutionary Boolean Reaction Systems is a new algorithm paradigm to construct bent and highly non linear balanced Boolean functions.

## Repository Structure

- **`src/`**  
  Contains all headers (`.hpp`) and source (`.cpp`) files implementing the following classes:  
  - `BoolReaction`: encodes reactions over Boolean entities with reactants and inhibitors (`R` and `I` bitsets);
  - `BRS`: models a Boolean Reaction System as a collection of `BoolReaction` objects;
  - `EvoBRS`: implements the EvoBRS algorithm;
  - `BoolFunction`: represents Boolean functions as truth tables, providing methods to compute the Fast Walsh–Hadamard Transform (FWHT), Hamming weight, non linearity and unbalancedness;

- **`results/`**  
  Stores experiments conducted.  

- **`Parameters.hpp`**  
  Header file to set parameters before building the code.  

- **`main_EvoRS.cpp`**  
  Main source file to run the EvoRS algorithm.  

- **`BAL.sh`** and **`BENT.sh`** 
  Bash files to automatically build and run the EvoRS algorithm for the `BAL` and `BENT` problems.  

## Requirements

C++17-compatible compiler (e.g. `g++` 7.0+ or `clang++` 6.0+).  

## Build & Run

You can configure the algorithm parameters for the two problems in the files `BAL.sh` and `BENT.sh`.  

Running either `.sh` script will:  
1. Build an executable saved in the `./bin/` folder.  
2. Run the executable `NUM_RUNS` times.  
3. Save the results in a newly created folder inside `results/`.  

### Manual Compilation

If you prefer to compile the code manually, follow these steps from the project’s base directory:

```bash
g++ -std=c++17 -O3 -o EvoBRS.exe ./src/*.cpp ./main_EvoBRS.cpp
```

and execute the program with:

```bash
./EvoBRS.exe INPUT_SEED
```

where `INPUT_SEED` is an integer used to initialise the random number generator.

