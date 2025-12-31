# ATM System

A console-based ATM application written in C++17 using object-oriented design principles.

## Project tree structure
```bash
├── CMakeLists.txt
├── include
│   ├── Account.hpp
│   ├── ATM.hpp
│   ├── Bank.hpp
│   ├── Card.hpp
│   ├── Transaction.hpp
│   └── UI.hpp
├── main.cpp
├── README.md
├── src
│   ├── Account.cpp
│   ├── ATM.cpp
│   ├── Bank.cpp
│   ├── Card.cpp
│   ├── Transaction.cpp
│   └── UI.cpp
├── test
├── umls
│   ├── class diagram.png
│   ├── UI.txt
│   └── use case diagram.txt
└── utils
    ├── AnsiRenderer.cpp
    ├── AnsiRenderer.hpp
    ├── Form.cpp
    ├── Form.hpp
    ├── Keyboard.cpp
    ├── Keyboard.hpp
    ├── Label.cpp
    ├── Label.hpp
    ├── Menu.cpp
    ├── Menu.hpp
    ├── TestBuffer.cpp
    ├── TextBuffer.hpp
    ├── TextField.cpp
    └── TextField.hpp
```

## Project build 
```bash
mkdir build
cd build
cmake ..
cmake --build .
./atm
```