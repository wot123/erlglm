#!/bin/sh
set -e
test `basename $PWD` != "c_src" && cd c_src

(test -d glm || git clone https://github.com/g-truc/glm)

