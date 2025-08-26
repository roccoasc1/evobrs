#!/bin/bash

# === READING an UPDATE PARAMETERS ===
source scripts/update_parameters.sh 

# === Generate tag ===
PARAM_TAG="min${INITSIZEMIN}_max${INITSIZEMAX}_pc${PC}_pr${PRMIN}"
SAFE_TAG=${PARAM_TAG//./_}
BIN_NAME="./bin/evo${PROBLEM_NAME}${N}_$(date +%Y-%m-%d_%H-%M-%S)_${SAFE_TAG}"
OUTPUT_FOLDER="./results/${PROBLEM_NAME}${N}_$(date +%Y-%m-%d_%H-%M-%S)_${SAFE_TAG}"

# === COMPILE ===
echo "$PROBLEM_NAME $N"
echo "Compiling with parameters: $PARAM_TAG"
mkdir -p bin
g++ -std=c++17 -O3 -o "$BIN_NAME" ./src/*.cpp || {
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