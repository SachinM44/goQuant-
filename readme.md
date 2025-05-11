GoQuant Trade Simulator
Overview
GoQuant is a high-performance trade simulator designed to estimate transaction costs and market impact using real-time market data. It connects to cryptocurrency exchanges via WebSocket to process Level 2 (L2) orderbook data.

Features
Real-time processing of L2 orderbook data

Almgren-Chriss market impact model implementation

Regression models for slippage estimation

Rule-based fee model

Console-based interface for parameter input and output display

High-performance WebSocket client

System Architecture
Core Components
WebSocket Client
src/websocket/WebSocketClient.cpp

Handles real-time connection to the OKX exchange

Processes L2 orderbook data streams

Implements error handling and reconnection logic

Market Impact Model
src/models/AlmgrenChriss.cpp

Implements Almgren-Chriss model for market impact estimation

Calculates temporary and permanent impact components

Optimizes execution strategy

Fee Model
src/models/FeeModel.cpp

Rule-based fee calculation

Supports multiple fee tiers

Considers maker/taker roles

Slippage Model
src/models/SlippageModel.cpp

Implements regression-based slippage estimation

Processes historical trade data

Adapts to market conditions

Installation
Clone the repository:

bash
Copy
Edit
git clone https://github.com/SachinM44/goQuant-.git
cd goQuant
Build the project:

bash
Copy
Edit
mkdir build
cd build
cmake ..
cmake --build . --config Release
Run the simulator:

bash
Copy
Edit
./goQuant
Usage
Input Parameters
Exchange Selection

Currently supports OKX exchange

Configurable via console interface

Asset Configuration

Spot asset selection

Supports major cryptocurrency pairs

Order Parameters

Order type (market orders)

Quantity specification (e.g., ~$100 USD equivalent)

Volatility settings

Fee tier selection

Output Metrics
Transaction Cost Analysis

Expected slippage (regression model)

Fee calculations (rule-based)

Market impact estimation (Almgren-Chriss)

Net cost computation

Performance Metrics

Maker/taker proportion

Internal latency measurements

Processing time per tick

Model Implementation Details
Almgren-Chriss Model
Calculates market impact using price dynamics and liquidity

Considers both temporary and permanent impact components

Optimizes execution strategy based on risk aversion parameters

Slippage Estimation
Uses regression techniques for price prediction

Incorporates market depth and volatility

Adapts to changing market conditions

Fee Calculation
Implements tiered fee structure

Considers trading volume and user level

Supports dynamic fee adjustments

Performance Optimization
Efficient data structures for orderbook management

Optimized WebSocket communication

Minimal latency processing pipeline

Memory-efficient implementation

Error Handling
Robust WebSocket connection management

Data validation and sanitization

Graceful error recovery

Comprehensive logging