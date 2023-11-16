# Matching Engine Implementation

## Overview
This project implements a matching engine, a crucial component in financial markets, used to match buy and sell orders from participants. It is built in C++ and utilizes various data structures like Skip Lists, linked lists, and hash maps for efficient processing.

## Features
- Order Processing: Supports different order types including GFD (Good For Day) and IOC (Immediate or Cancel).
- Order Matching: Efficiently matches buy and sell orders.
- Skip List Implementation: Utilizes skip lists for optimized order management.
- Order Book Management: Maintains separate order books for buy and sell orders.

## Installation
Provide detailed steps on how to install and set up the project, including any prerequisites or dependencies.

## Usage
Explain how to use the matching engine, including how to input commands, and any specific syntax or command line arguments.

### Example Commands
- BUY
- SELL
- MODIFY
- CANCEL
- PRINT

## Code Structure
Briefly describe the main classes and their functionalities:
- `Order`: Represents a market order.
- `SkipListNode`: Nodes used in the skip list for order sorting.
- `SkipList`: Data structure for managing orders efficiently.
- `OrderFactory`: Factory pattern implementation for creating and managing orders.
- `OrderBook`: Manages and processes orders.
- `MatchingEngine`: Core engine to process commands and manage order book.
