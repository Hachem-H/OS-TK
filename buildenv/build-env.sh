#!/usr/bin/env sh

docker run --rm -it                    \
    -v "$(pwd):/home/anon/ostk"        \
    -h OS-TK ostk-env
