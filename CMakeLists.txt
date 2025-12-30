cmake_minimum_required(VERSION 3.16)

project(ATM_System
    VERSION 1.0
    DESCRIPTION "Console-based ATM system using modern C++ and OOP"
    LANGUAGES CXX
)

# ----------------------------
# C++ Standard
# ----------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ----------------------------
# Source files
# ----------------------------
set(SRC_FILES
    src/Account.cpp
    src/ATM.cpp
    src/Bank.cpp
    src/Card.cpp
    src/Transaction.cpp
    src/UI.cpp
)

set(UTILS_FILES
    utils/AnsiRenderer.cpp
    utils/Form.cpp
    utils/Keyboard.cpp
    utils/Label.cpp
    utils/Menu.cpp
    utils/TestBuffer.cpp
    utils/TextField.cpp
)

# ----------------------------
# Executable
# ----------------------------
add_executable(atm
    main.cpp
    ${SRC_FILES}
    ${UTILS_FILES}
)

# ----------------------------
# Include directories
# ----------------------------
target_include_directories(atm
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/utils
)

# ----------------------------
# Compiler warnings
# ----------------------------
if (MSVC)
    target_compile_options(atm PRIVATE /W4)
else()
    target_compile_options(atm PRIVATE -Wall -Wextra -Wpedantic)
endif()

# ----------------------------
# Build type default
# ----------------------------
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
