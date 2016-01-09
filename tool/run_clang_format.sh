#!/bin/bash
find ../src -name \*.h -print -o -name \*.cpp -print |xargs clang-format-3.6 -i
find ../test -name \*.h -print -o -name \*.cpp -print |xargs clang-format-3.6 -i
