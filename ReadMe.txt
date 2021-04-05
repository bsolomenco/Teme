Prerequisites
    CMake               : https://cmake.org/download
    VisualStudio 2019   : https://visualstudio.com

Repo
    git clone https://github.com/bsolomenco/Teme.git

Build
    Linux/gcc   : rm -rf .tmp   ; cmake -B .tmp && cmake --build .tmp --target install --config Release
    Windows/VS  : rd /Q /S .tmp & cmake -B .tmp && cmake --build .tmp --target install --config Release


Stack (list based, type=int)

Queue (list based, type=int)

Parens
- obvious rule: matching pairs: () [] {}
- unclear rule: nesting rank ("sirul ([]) nu este corect"), assume: () < [] < {}
- allowed??? (()) [[]] {{}}

Conex
- there is a mistake in requirements: the example for 2.8!!!