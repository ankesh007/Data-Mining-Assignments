#!/bin/sh
### Reading Inputs
INPUT_FILE="$1"
SUPPORT="$2"
MODE="$3"
OUTPUT_FILE="$4"
TIME_FILE="$(dirname $0)/timings.txt"
PLOT_FILE="$(dirname $0)/plot.py"

if [ "$SUPPORT" = "-plot" ]; then
    MODE="-plot"
fi
case "${MODE}" in
    "-apriori")
        echo "[$(basename $0)] Generating frequent item set using Apriori algorithm"
        ./freq_itemset "$INPUT_FILE" "$OUTPUT_FILE" "$SUPPORT" 0
        ;;
    "-fptree")
        echo "[$(basename $0)] Generating frequent item set using FPTree algorithm"
        ./freq_itemset "$INPUT_FILE" "$OUTPUT_FILE" "$SUPPORT" 1
        ;;
    "-plot")
        echo "[$(basename $0)] Generating frequent item set using Apriori, FPTree algorithm and plotting relative performance"
        rm -f "$TIME_FILE"
        RANDOM_FILE="$(mktemp $(dirname $0)/out_XXX.txt)"
        { \time -f "%E" ./freq_itemset "$INPUT_FILE" "$RANDOM_FILE" 1  0; } 2>  "$TIME_FILE"
        { \time -f "%E" ./freq_itemset "$INPUT_FILE" "$RANDOM_FILE" 5  0; } 2>> "$TIME_FILE"
        { \time -f "%E" ./freq_itemset "$INPUT_FILE" "$RANDOM_FILE" 10 0; } 2>> "$TIME_FILE"
        { \time -f "%E" ./freq_itemset "$INPUT_FILE" "$RANDOM_FILE" 25 0; } 2>> "$TIME_FILE"
        { \time -f "%E" ./freq_itemset "$INPUT_FILE" "$RANDOM_FILE" 50 0; } 2>> "$TIME_FILE"
        { \time -f "%E" ./freq_itemset "$INPUT_FILE" "$RANDOM_FILE" 90 0; } 2>> "$TIME_FILE"

        { \time -f "%E" ./freq_itemset "$INPUT_FILE" "$RANDOM_FILE" 1  1; } 2>> "$TIME_FILE"
        { \time -f "%E" ./freq_itemset "$INPUT_FILE" "$RANDOM_FILE" 5  1; } 2>> "$TIME_FILE"
        { \time -f "%E" ./freq_itemset "$INPUT_FILE" "$RANDOM_FILE" 10 1; } 2>> "$TIME_FILE"
        { \time -f "%E" ./freq_itemset "$INPUT_FILE" "$RANDOM_FILE" 25 1; } 2>> "$TIME_FILE"
        { \time -f "%E" ./freq_itemset "$INPUT_FILE" "$RANDOM_FILE" 50 1; } 2>> "$TIME_FILE"
        { \time -f "%E" ./freq_itemset "$INPUT_FILE" "$RANDOM_FILE" 90 1; } 2>> "$TIME_FILE"
        rm -f "$RANDOM_FILE"
        python "${PLOT_FILE}" "${TIME_FILE}"

        echo "[$(basename $0)] Plot comparing apriori and fptree timings saved"
        echo "[$(basename $0)] Location $(dirname $0)/plot.png"
        ;;
    *)
        echo "[$(basename $0)] Invalid mode option. Exiting"
        exit 1
esac

echo "[$(basename $0)] Run complete. Exiting"
