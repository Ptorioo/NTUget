#!/bin/bash

# Defined variables
COMPILER="gcc"
CFLAGS="-Wall -Wextra -O2 -pedantic -fsanitize=address"
INCLUDE_PATH="-I../include"
LIBS="-lcurl -lssl -lcrypto -lz -lbrotlidec -lbrotlicommon -lpsl"
SRC_PATH="../src"
TEST_PROGRAM="test.c"
OUTPUT="run-test"

echo "Unit test starting..."

$COMPILER $CFLAGS $INCLUDE_PATH $LIBS -o $OUTPUT $TEST_PROGRAM $SRC_PATH/encoder.c unit-001.c && ./$OUTPUT
$COMPILER $CFLAGS $INCLUDE_PATH $LIBS -o $OUTPUT $TEST_PROGRAM $SRC_PATH/utils.c unit-002.c && ./$OUTPUT

echo "Unit test finished..."