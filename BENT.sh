#!/bin/bash

PARAM_FILE="./Parameters.hpp"
NUM_RUNS=10

# === PARAMETERS ===
POPSIZE=100
POPELITESIZE=5
MAXFITEVALS=500000
N=6
FITNESS_FUNC="FitnessBENT"
PC=0.2
PRMIN=0.8
PREN=0.1
INITSIZEMIN=80
INITSIZEMAX=100

# === Function to update Parameters.hpp and ./main_EvoBRS.cpp===
update_params() {
  local min=$1
  local max=$2

  sed -i "s/^constexpr unsigned int INITSIZEMIN *=.*;/constexpr unsigned int INITSIZEMIN=${min};/" "$PARAM_FILE"
  sed -i "s/^constexpr unsigned int INITSIZEMAX *=.*;/constexpr unsigned int INITSIZEMAX=${max};/" "$PARAM_FILE"
  sed -i "s/^constexpr double PC *=.*;/constexpr double PC = ${PC};/" "$PARAM_FILE"
  sed -i "s/^constexpr double PRMIN *=.*;/constexpr double PRMIN = ${PRMIN};/" "$PARAM_FILE"
  sed -i "s/^constexpr double PREN *=.*;/constexpr double PREN = ${PREN};/" "$PARAM_FILE"

  sed -i "s/^constexpr std::size_t POPSIZE *=.*;/constexpr std::size_t POPSIZE = ${POPSIZE};/" "$PARAM_FILE"
  sed -i "s/^constexpr int POPELITESIZE *=.*;/constexpr int POPELITESIZE = ${POPELITESIZE};/" "$PARAM_FILE"
  sed -i "s/^constexpr unsigned int MAXFITEVALS *=.*;/constexpr unsigned int MAXFITEVALS = ${MAXFITEVALS};/" "$PARAM_FILE"
  sed -i "s/^constexpr std::size_t n *=.*;/constexpr std::size_t n = ${N};/" "$PARAM_FILE"
  
  sed -i "s|^\(\s*const std::function<double(const BRS&)> fit = \).*;|\1${FITNESS_FUNC};|" ./main_EvoBRS.cpp
}

counter=0
update_params "$INITSIZEMIN" "$INITSIZEMAX" "$PC" "$PRMIN" "$PIN"

# === Generate tag ===
PARAM_TAG="min${INITSIZEMIN}_max${INITSIZEMAX}_pc${PC}_pr${PRMIN}_pin${PIN}"
SAFE_TAG=${PARAM_TAG//./_}
BIN_NAME="./bin/evoBENT${N}_$(date +%Y-%m-%d_%H-%M-%S)_${SAFE_TAG}"
OUTPUT_FOLDER="./results/BENT${N}_$(date +%Y-%m-%d_%H-%M-%S)_${SAFE_TAG}"

# === COMPILE ===
echo "BENT $N"
echo "Compiling with parameters: $PARAM_TAG"
mkdir -p bin
g++ -std=c++17 -O3 -o "$BIN_NAME" ./main_EvoBRS.cpp ./src/*.cpp || {
    echo "Compilation failed for $PARAM_TAG"
    continue
}

# === OUTPUT ===
mkdir -p "$OUTPUT_FOLDER"
echo "Running $NUM_RUNS experiments for $BIN_NAME..."

for ((i=1; i<=NUM_RUNS; i++)); do
    echo "Run seed $i"
    "$BIN_NAME" "$i" > "${OUTPUT_FOLDER}/run${i}.csv" &
done

echo "Launched  $NUM_RUNS runs for $PARAM_TAG"

wait
echo "All runs completed."