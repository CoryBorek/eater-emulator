#!/bin/bash
set -e

git clone https://github.com/beneater/msbasic

PATH=$PWD/cc65/bin:$PWD/cc65/lib:$PATH
cd msbasic
./make.sh
