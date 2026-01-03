# ATM System

A console-based ATM application written in C++17 using object-oriented design principles. This project simulates the basic functionalities of an Automated Teller Machine.

## Features

* **Card Validation:** Verifies the card's authenticity, expiration date, and status (active/blocked).
* **User Authentication:** Authenticates the user through card number and PIN.
* **Withdraw Cash:** Allows users to withdraw cash from their account, checking for sufficient funds in both the user's account and the ATM.
* **Deposit Cash:** Allows users to deposit cash into their account.
* **Transfer Funds:** Enables transferring funds to another account.
* **Balance Inquiry:** Displays the current account balance.
* **Change PIN:** Allows users to change their card's PIN.
* **Transaction History:** Records all transactions.

## Project Structure

The project is organized into the following directories:

```
.
├── CMakeLists.txt
├── include/         // Header files for the core ATM logic (e.g., Account.hpp, ATM.hpp)
├── src/             // Source files for the core ATM logic (e.g., Account.cpp, ATM.cpp)
├── utils/           // A simple TUI framework for the console interface (e.g., AnsiRenderer.hpp, Form.hpp)
├── uml/             // UML diagrams for the project (e.g., Class Diagram.jpg, Sequence Diagrams)
└── main.cpp         // Entry point of the application
```

## UML Diagrams

The `uml/` directory contains various diagrams that illustrate the system's design, including:
- Class Diagram
- Use Case Diagram
- Sequence Diagrams for different scenarios (login, withdraw, transfer)
- Activity Diagrams

## Build and Run

The project is built using CMake.

### Prerequisites

* C++17 compatible compiler (GCC, Clang, MSVC)
* CMake (version 3.16 or higher)

### Steps

1.  **Clone the repository:**
    ```bash
    git clone <repository-url>
    cd ATM-Project
    ```

2.  **Create a build directory:**
    ```bash
    mkdir build
    cd build
    ```

3.  **Configure the project with CMake:**
    ```bash
    cmake ..
    ```

4.  **Build the project:**
    ```bash
    cmake --build .
    ```

5.  **Run the application:**
    ```bash
    ./atm
    ```
