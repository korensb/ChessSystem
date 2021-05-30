#!/usr/bin/env bash
sudo apt-get update
sudo apt-get install --yes cmake ninja-build g++ valgrind
echo "You may want to install pandoc and a pdf engine to compile dry.md into a pdf"
