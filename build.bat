@echo off
echo Compiling RideX...
g++ -std=c++17 -Wall -Wextra -Iinclude src\*.cpp -o ridex.exe
if %ERRORLEVEL% EQU 0 (
    echo Compilation Successful!
    echo Running ridex.exe...
    echo.
    ridex.exe
) else (
    echo Compilation Failed!
)
