#! /usr/bin/env bash

shopt -s globstar

files=(**/*.[ch]*)
if [ "$1" = check ]
then
    clang-format --dry-run -Werror ${files[@]}
else
    clang-format -i ${files[@]}
fi
