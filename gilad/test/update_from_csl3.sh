#!/usr/bin/env bash
scp -r "$1@csl3.cs.technion.ac.il:~mtm/public/2021b/ex1" .
mv ex1/chessSystem.h src/chess/
mv ex1/finalcheck .
mv ex1/test_utilities.h src/chess/
mv ex1/tests/* src/chess/tests/
mv ex1/mtm_map/map.h src/chess/mtm_map/
cp ex1/libmap_for_csl3/libmap.a libmap_for_csl3/
mv ex1/libmap_for_csl3/libmap.a src/chess/
rm -ri ex1
