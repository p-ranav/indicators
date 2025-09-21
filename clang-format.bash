#!/usr/bin/env bash
find ./include ./demo/ ./samples/ -type f \( -iname \*.cpp -o -iname \*.hpp \) | xargs clang-format -style=file -i
