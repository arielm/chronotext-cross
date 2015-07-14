#!/bin/sh

export CROSS_PATH="$(pwd)"
alias RUN_TEST='ctest -S $CROSS_PATH/core/cmake/run.cmake -VV'
