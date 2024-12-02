#!/bin/bash
wget http://sun.hasenbraten.de/vasm/release/vasm.tar.gz
tar -xf vasm.tar.gz
cd vasm
make CPU=6502 SYNTAX=oldstyle
mv vasm6502_oldstyle ..
cd ..
rm -rf vasm vasm.tar.gz
