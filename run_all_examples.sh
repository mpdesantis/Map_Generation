#!/bin/bash
###############################################################################
# run_all_examples.sh
# -------------------
# Run all simulation binaries in `./bin`.
#
# Usage:
# ------
# $ ./run_all_examples.sh
#
# Positional Arguments:
#   NA
#
# Optional Arguments:
#   NA
# 
# Notes:
# ------
# Ensure you have built the binaries with the `build_sim.sh` script before use.
# Ensure this script has executable permission before use.
#
###############################################################################
# Author: M. De Santis
# Date: 2025/03/07
###############################################################################

set -e

SCRIPT=$(readlink -f "$0")
SCRIPT_DIR=$(dirname ${SCRIPT})

echo -e "EXECUTING $SCRIPT...\n"

# Run all simulations
SIMULATION_DIR=${PWD}/bin
echo -e "Executing all simulation binaries (with default arguments) in ${SIMULATION_DIR} ...\n"
for BINARY in "${SIMULATION_DIR}"/*;
do
    # TODO: Bake in binary calls with appropriate arguments.
    if [[ -x "${BINARY}" && -f "${BINARY}" ]]; then
        echo -e "Executing simulation binary: bin/$(basename ${BINARY})"
        "${BINARY}" 
    else
        echo "Skipping non-executable file: ${BINARY}"
    fi
done

echo -e "\n--- YOU EXECUTED ---"

