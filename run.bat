@echo off

cd bin
cmake -G "MinGW Makefiles" .
mingw32-make -j
start "" main.exe
cd ..