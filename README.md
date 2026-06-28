# Bank Management System in C

![Language](https://img.shields.io/badge/Language-C-blue)
![Project](https://img.shields.io/badge/Project-Console%20Application-success)
![Storage](https://img.shields.io/badge/Storage-Binary%20File-orange)

A complete console-based **Bank Management System** built using **C programming only**.  
This project demonstrates file handling, structures, menu-driven programming, account operations, and basic authentication using a PIN.

## Preview

```text
=====================================
        BANK MANAGEMENT SYSTEM
=====================================
1. Create new account
2. View account details
3. Deposit money
4. Withdraw money
5. Transfer money
6. Update account details
7. Delete account
8. List all accounts
0. Exit
-------------------------------------
Enter your choice:
```

## Features

- Create new bank accounts
- Generate account numbers automatically
- View account details with PIN verification
- Deposit money into an account
- Withdraw money with balance validation
- Transfer money between accounts
- Update customer details
- Delete accounts safely
- List all active accounts
- Store records permanently using binary file handling

## Tech Stack

- **Language:** C
- **Compiler:** GCC / MinGW / MSYS2 GCC
- **Concepts Used:** Structures, functions, file handling, validation, menu-driven programming
- **Data Storage:** Local binary file named `accounts.dat`

## Project Structure

```text
Bank-Management-System-C/
├── bank_management_system.c
├── jarvis.py
├── Run_Jarvis.bat
├── JARVIS_README.md
├── README.md
└── .gitignore
```

Generated files such as `bank_management_system.exe` and `accounts.dat` are ignored by Git.

## How to Compile

### Windows

```powershell
gcc bank_management_system.c -o bank_management_system.exe
```

### Linux or macOS

```bash
gcc bank_management_system.c -o bank_management_system
```

## How to Run

### Windows

```powershell
.\bank_management_system.exe
```

### Linux or macOS

```bash
./bank_management_system
```

## Main Operations

| Option | Operation |
|---:|---|
| 1 | Create new account |
| 2 | View account details |
| 3 | Deposit money |
| 4 | Withdraw money |
| 5 | Transfer money |
| 6 | Update account details |
| 7 | Delete account |
| 8 | List all accounts |
| 0 | Exit |

## Extra Project

This repository also includes a Python **Jarvis Voice Assistant**.
See [JARVIS_README.md](JARVIS_README.md) for setup, commands, and usage.

## Learning Outcomes

This project is useful for students and beginners who want to practice:

- Creating a real-world C console application
- Working with `struct`
- Reading and writing binary files using `fread` and `fwrite`
- Updating records with `fseek` and `ftell`
- Organizing code into reusable functions
- Handling user input safely

## Important Note

This is an academic project for learning C programming. It is not intended for real banking or production financial use.

## Author

Created by **0xADITYA-ROOT**
