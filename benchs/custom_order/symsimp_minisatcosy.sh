#!/bin/bash

$(dirname $0)//sat "$1"
if [ $? -ne 0 ]; then
    exit 0
fi

BASE="/tmp/"
FILE=$(basename "$1")
REDUCE="$BASE/reduce-$FILE"
ORDER="$BASE/order-$FILE"
GROUP="$BASE/group-$FILE"

$(dirname $0)//minisat_cosy "$REDUCE" -order="$ORDER" -symmetry="$GROUP"

rm -f "$ORDER" "$GROUP" "$REDUCE"
