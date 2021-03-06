#!/bin/bash

DATE_STR=$(date +%d%m%y_%H%M%S)
EXEC_DIR=./sol/EXEC_$DATE_STR
mkdir $EXEC_DIR

bash ./sh/exec.sh $EXEC_DIR | tee $EXEC_DIR/sol$DATE_STR.glob

bash ./sh/calcAvg.sh $EXEC_DIR