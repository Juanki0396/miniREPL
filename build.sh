#! /bin/env bash

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' 
OUTPUT="miniREPL"
CMD="gcc -Wall -Wextra -o ${OUTPUT} main.c"

echo $CMD
gcc -Wall -Wextra -o $OUTPUT main.c


if [[ $? == 0 ]];
then
    echo -e "${GREEN}SUCCSES${NC}: Compilation end succesfully. Run program with: ./${OUTPUT}"
else
    echo -e "${RED}FAILED${NC}: Compilation has failed."
fi
