@echo off
g++ .\main.cpp qwqwq.o -o main.exe -lgdi32 -fpermissive -lcomctl32 -lcomdlg32 -ldismapi -L . -lwim -static-libgcc -static-libstdc++