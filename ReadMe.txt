Prerequisites
    CMake               : https://cmake.org/download
    VisualStudio 2019   : https://visualstudio.com

Repo
    git clone https://github.com/bsolomenco/Teme.git

Build
    Linux/gcc   : rm -rf .tmp   ; cmake -B .tmp && cmake --build .tmp --target install --config Release
    Windows/VS  : rd /Q /S .tmp & cmake -B .tmp && cmake --build .tmp --target install --config Release

Run
- start each executable without arguments and it will tell you how to use it
- there is a test file in each folder, use it as input

2.1 Stack (list based, type=int)

2.2 ParensMatch
- obvious rule: matching pairs: () [] {}
- unclear rule: nesting rank ("sirul ([]) nu este corect"), assume: () < [] < {}
- allowed??? (()) [[]] {{}}

2.3 RPN (Reverse Polish Notation)
- limitation: works only with single digit numbers

2.8 Connex
- there is a mistake in requirements: object 1 from the example is WRONG!!!

2.9 Maze
- show ALL minimal solutions/paths (marked with "1" in the matrix)