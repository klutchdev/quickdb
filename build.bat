@echo off
cls

:: Display Header
echo ========================================
echo           QuickDB Build Script          
echo ========================================
echo.
echo Starting build ...
echo.

:: Compilation steps
echo Compiling Source Files...
echo ----------------------------------------

echo     Compiling cJSON.c...
gcc -c src\cJSON.c -o cJSON.o

echo     Compiling document.c...
gcc -c src\document.c -o document.o

echo     Compiling collection.c...
gcc -c src\collection.c -o collection.o

echo     Compiling collection_manager.c...
gcc -c src\collection_manager.c -o collection_manager.o

echo     Compiling json_utils.c...
gcc -c src\json_utils.c -o json_utils.o

echo     Compiling cli.c...
gcc -c src\cli.c -o cli.o

echo     Compiling main.c...
gcc -c src\main.c -o main.o

echo ----------------------------------------

:: Linking
echo Linking object files...
gcc main.o cli.o document.o collection.o collection_manager.o cJSON.o json_utils.o -o quickdb.exe
echo.

:: Footer
echo     BUILD SUCCESS!
echo.

echo Run it with ./quickdb.exe
echo.
