# MiniDB

A miniature relational database engine built from scratch in **C++20** to understand how real databases such as PostgreSQL, MySQL, and SQLite work internally.

This project focuses on implementing the core components of a database system instead of using existing database libraries. The implementation gradually evolves from a simple storage engine into a page-based database with an in-memory buffer pool.

> **Project Status:** Phase 4 Complete (Buffer Pool Manager)

---

# Features

## Phase 1 – Basic Storage Engine

* Interactive SQL shell
* Create tables
* Insert records
* Select all records
* Persistent binary storage
* Multiple table support
* Metadata management
* Error handling

Supported SQL:

```sql
CREATE TABLE users;

INSERT INTO users VALUES(1, 'Alice');

SELECT * FROM users;
```

---

## Phase 2 – SQL Parser

* SQL Lexer (Tokenizer)
* SQL Parser
* Abstract Syntax Tree (AST)
* Command execution through AST
* Extensible parser architecture

Supported Statements

```sql
CREATE TABLE users;

INSERT INTO users VALUES(1,'Alice');

SELECT * FROM users;
```

---

## Phase 3 – Page-Based Storage

Instead of writing rows directly to disk, MiniDB stores data inside fixed-size pages.

Features:

* 4 KB pages
* Page allocation
* Read page
* Write page
* Row insertion into pages
* Multiple pages per table
* Binary page serialization

Page Layout

```
users.tbl

+-----------+
| Page 0    |
+-----------+
| Page 1    |
+-----------+
| Page 2    |
+-----------+
```

Each page contains multiple rows.

---

## Phase 4 – Buffer Pool Manager

To reduce expensive disk I/O, MiniDB caches frequently accessed pages in memory.

Features

* Fixed-size buffer pool
* FetchPage()
* UnpinPage()
* FlushPage()
* Dirty page tracking
* LRU page replacement
* Automatic eviction when memory is full

Flow

```
SQL Query
     │
     ▼
Buffer Pool
     │
Page in Memory?
 ├── Yes → Return Page
 └── No
      │
      ▼
 Read From Disk
      │
      ▼
 Insert Into Buffer Pool
```

---

# Architecture

```
                   +----------------------+
                   |     SQL Shell        |
                   +----------+-----------+
                              |
                              ▼
                   +----------------------+
                   |      SQL Parser      |
                   +----------+-----------+
                              |
                              ▼
                   +----------------------+
                   |     AST Executor     |
                   +----------+-----------+
                              |
                              ▼
                   +----------------------+
                   |      Database        |
                   +----------+-----------+
                              |
                              ▼
                   +----------------------+
                   |        Table         |
                   +----------+-----------+
                              |
                              ▼
                   +----------------------+
                   | Buffer Pool Manager  |
                   +----------+-----------+
                              |
                              ▼
                   +----------------------+
                   |   Storage Manager    |
                   +----------+-----------+
                              |
                              ▼
                   +----------------------+
                   |     Page Manager     |
                   +----------+-----------+
                              |
                              ▼
                   +----------------------+
                   |   Binary Disk Files  |
                   +----------------------+
```

---

# Project Structure

```
MiniDB/
│
├── CMakeLists.txt
├── README.md
│
├── data/
│   ├── schema.meta
│   └── *.tbl
│
├── include/
│   ├── parser/
│   ├── database/
│   ├── storage/
│   ├── buffer/
│   └── executor/
│
├── src/
│   ├── main.cpp
│   ├── parser/
│   ├── database/
│   ├── storage/
│   ├── buffer/
│   └── executor/
│
└── tests/
```

---

# Current Components

## Database

Responsible for

* Managing tables
* Loading metadata
* Creating tables
* Looking up existing tables

---

## Table

Responsible for

* Insert operations
* Select operations
* Communicating with the storage layer

---

## Storage Manager

Responsible for

* Reading pages
* Writing pages
* Allocating new pages
* Managing table files

---

## Page

Represents a single fixed-size page.

Responsibilities

* Store rows
* Serialize page
* Deserialize page
* Track page metadata

---

## Buffer Pool Manager

Responsibilities

* Cache pages in RAM
* Reduce disk reads
* Manage dirty pages
* Evict pages using LRU
* Flush pages to disk

---

# Example

```
MiniDB > CREATE TABLE users;

Table created.

MiniDB > INSERT INTO users VALUES(1, 'Alice');

1 row inserted.

MiniDB > INSERT INTO users VALUES(2, 'Bob');

1 row inserted.

MiniDB > SELECT * FROM users;

+----+-------+
| ID | Name  |
+----+-------+
| 1  | Alice |
| 2  | Bob   |
+----+-------+
```

---

# Technologies

* C++20
* CMake
* GCC / Clang
* WSL (Ubuntu)
* Binary File I/O
* Object-Oriented Design
* STL

---

# Concepts Covered

* Binary serialization
* File systems
* Page-based storage
* Buffer pool management
* LRU cache
* SQL parsing
* Abstract Syntax Trees (AST)
* Memory management
* Disk I/O
* Database architecture

---

# Upcoming Phases

## Phase 5

* B+ Tree Index
* Index creation
* Index search
* Node split
* Leaf nodes
* Internal nodes

## Phase 6

* Query Execution Engine
* Sequential Scan
* Index Scan
* Projection
* Filter operators

## Phase 7

* Cost-Based Query Optimizer
* Execution plan generation
* Index vs sequential scan selection
* Basic table statistics

## Phase 8

* Transactions
* BEGIN
* COMMIT
* ROLLBACK
* Write-Ahead Logging (WAL)
* Crash recovery

---

# Learning Objectives

This project is designed to understand the internals of modern relational database systems by implementing their core components from scratch.

Rather than focusing on SQL features alone, the emphasis is on storage engines, memory management, indexing, query execution, and transaction processing—the building blocks used in production-grade database systems.

---

# License

This project is intended for educational purposes and personal learning.
