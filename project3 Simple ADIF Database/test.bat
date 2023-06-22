@echo off

:: compile the program
g++ code.cpp -o code.exe

:: display the C++ version
echo Using C++ version: c++ 11

code.exe -i a.adi

code.exe -i b.adi

code.exe -s BD8GK

code.exe -l 20070130050400 20070131113100

code.exe -o test.adi

code.exe -o test.csv

code.exe -d b.adi

code.exe -i c.csv

code.exe -o test.csv
pause