# RISCV_Simulator

## Compiling and running
 - Install CMake
 - Install latest gcc version (supporting C23)
 - Run `cmake -S . -B build`
 	- If it build for wrong compiler, add `-G "MinGW Makefiles"`
 - Run `cmake --build build`
 - Start program by running `.\build\RISCV_Simulator.exe TESTFILE`, where TESTFILE is the path to your test file <br>eg: ``.\build\RISCV_Simulator.exe .\tests\task1\addlarge.bin``
