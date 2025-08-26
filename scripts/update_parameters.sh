source Parameters.env

PARAM_FILE="./src/Parameters.hpp"

FITNESS_FUNC="Fitness${PROBLEM_NAME}"

POPSIZE=100
POPELITESIZE=5
PC=0.2
PRMIN=0.8
PREN=0.1

# === Function to update Parameters.hpp and main_EvoBRS.cpp===
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
  
  sed -i "s|^\(\s*const std::function<double(const BRS&)> fit = \).*;|\1${FITNESS_FUNC};|" ./src/main_EvoBRS.cpp
}

update_params "$INITSIZEMIN" "$INITSIZEMAX" "$PC" "$PRMIN" "$PIN"