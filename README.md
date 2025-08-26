# Evolutionary Boolean Reaction Systems (EvoBRS)
Evolutionary Boolean Reaction Systems is a new algorithm paradigm to construct bent and highly non linear balanced Boolean functions.

## Repository Structure

- **`src/`**  
  Contains all headers (`.hpp`) and source (`.cpp`) files implementing the following classes:  
  - `BoolReaction`: encodes reactions over Boolean entities with reactants and inhibitors (`R` and `I` bitsets);
  - `BRS`: models a Boolean Reaction System as a collection of `BoolReaction` objects;
  - `EvoBRS`: implements the EvoBRS algorithm;
  - `BoolFunction`: represents Boolean functions as truth tables, providing methods to compute the Fast Walsh–Hadamard Transform (FWHT), Hamming weight, non linearity and unbalancedness;

- **`results/`** folder where the experiments conducted are stored.  

- **`scripts/`** folder for auxiliary scripts. 

- **`Parameters.env`** file to set parameters before building the code.    

- **`run.sh`** bash file to automatically build and run the EvoRS algorithm for the `BAL` or the `BENT` problem.  

## Requirements

C++17-compatible compiler (e.g. `g++` 7.0+ or `clang++` 6.0+).  

## Build & Run

Configure the algorithm parameters in the file `Parameters.env`:
- `PROBLEM_NAME` must be set to `"BAL"` or `"BENT"`;
- `N` is size of the problem to solve;
- `INITSIZEMIN`,`INITSIZEMAX` are the minimun and maximum number of reactions in a random Boolean Reaction System;
- `MAXFITEVALS` is the number of fitness evaluation for each run.

Running `run.sh` script will:  
1. Build an executable saved in the `./bin/` folder.  
2. Run the executable `NUM_RUNS` times.  
3. Save the results in a newly created folder inside `results/`.  