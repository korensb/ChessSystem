#!/usr/bin/env bash

NAME=submission_test.zip
FINAL=ex1.zip

pushd src/chess
    zip "../../${NAME}" makefile *.{h,c} mtm_map/*.{h,c}
    zip "../../${NAME}" -d chessSystem.h mtm_map/map.h test_utilities.h
popd

cmake --build build --target dry-pdf
zip -j "${NAME}" build/dry.pdf

./finalcheck "${NAME}"

rm -i "${NAME}"
if [ -f "${NAME}" ]; then
    echo "Saving ${NAME} as ${FINAL}"
    mv "${NAME}" "${FINAL}"
fi
