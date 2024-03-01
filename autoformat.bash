#! /usr/bin/env bash

files=(*/*.c*)
if [ "$1" = check ]
then
    clang-format --dry-run -Werror ${files[@]}
else
    clang-format -i ${files[@]}
fi
