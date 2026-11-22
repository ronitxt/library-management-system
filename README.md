# Library Management System (C++)

A console-based educational library application built with C++17, object-oriented programming, and file handling.

## Features
- Add books with title, author, and copy count
- Register library members
- Display all books and their available copies
- Issue books to registered members
- Return previously issued books
- Search books by title or author (case-insensitive partial matching)
- Search members by member ID
- View borrowing records
- Persist books, members, and borrowing records in separate text files

## Project structure
```text
library-management-system/
├── include/
│   ├── Book.h
│   ├── Member.h
│   └── Library.h
├── src/
│   ├── main.cpp
│   ├── Book.cpp
│   ├── Member.cpp
│   └── Library.cpp
├── data/
│   ├── books.txt
│   ├── members.txt
│   └── transactions.txt
├── CMakeLists.txt
├── .gitignore
└── README.md
```

## Build and run (GitHub Codespaces / Linux)
From the repository root:
```bash
cmake -S . -B build
cmake --build build -j2
./build/library_management
```

If CMake or a C++ compiler is missing on Ubuntu/Debian:
```bash
sudo apt update
sudo apt install -y build-essential cmake
```

## Data files
- `data/books.txt`: book ID, title, author, total copies, available copies
- `data/members.txt`: member ID, name, email
- `data/transactions.txt`: book ID, member ID, issue marker, return marker, returned status

Records use the pipe (`|`) character as a field separator. Avoid entering that character in names, titles, authors, or emails.

## OOP concepts demonstrated
- **Classes and objects:** `Book`, `Member`, and `Library`
- **Encapsulation:** class data is private and accessed through public member functions
- **Constructors:** initialize book and member objects
- **Member functions:** issue, return, search, load, and save operations

## Educational-use notice
This is a learning project. It is not intended for production library deployment or concurrent multi-user access. Back up the data files before making manual changes.
