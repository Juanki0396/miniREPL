#! /bin/env bash

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' 
MAIN="miniREPL.c"
OUTPUT="miniREPL"

if [[ ! -d "./build" ]];
then
    mkdir build
fi 

if [[ $1 == "test" ]];
then
    MAIN="test.c"
    OUTPUT="test"
fi

gcc -Wall -Wextra -o ./build/$OUTPUT $MAIN

if [[ $? == 0 ]];
then
    echo -e "${GREEN}SUCCSES${NC}: Compilation end succesfully. Runnig program with: ./${OUTPUT}"
    ./build/$OUTPUT
else
    echo -e "${RED}FAILED${NC}: Compilation has failed."
fi
