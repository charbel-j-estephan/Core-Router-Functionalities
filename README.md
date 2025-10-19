# Core Router Functionalities System

A comprehensive C++ router simulation system that implements Quality of Service (QoS) management, routing table operations, and packet history tracking with loop detection capabilities.

## 📋 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [System Architecture](#system-architecture)
- [Data Structures](#data-structures)
- [Algorithms](#algorithms)
- [Installation](#installation)
- [Usage](#usage)
- [File Structure](#file-structure)
- [Class Documentation](#class-documentation)
- [Configuration](#configuration)
- [Examples](#examples)
- [Time Complexity Analysis](#time-complexity-analysis)
- [Contributing](#contributing)
- [License](#license)

---

## 🎯 Overview

This project simulates core functionalities of a network router, demonstrating practical applications of various data structures in computer networking. The system handles packet classification based on Quality of Service (QoS) policies, performs intelligent routing decisions using longest prefix matching, and maintains comprehensive packet history for analysis and debugging.

**Domain:** Network Routing and Quality of Service Management

**Language:** C++  
**Platform:** Cross-platform (Windows, Linux, macOS)  
**Compiler:** C++11 or later

---

## ✨ Features

### 1. Quality of Service (QoS) Management
- ✅ Three-tier priority queue system (High, Medium, Low)
- ✅ Automatic packet classification based on port numbers
- ✅ Priority-based packet processing
- ✅ Queue overflow handling with configurable maximum size
- ✅ Real-time queue status monitoring

### 2. Routing Table Management
- ✅ Dynamic routing table with CIDR notation support
- ✅ Longest Prefix Matching (LPM) algorithm
- ✅ Multiple route entries with different prefix lengths
- ✅ Metric-based route selection for tie-breaking
- ✅ Default gateway support (0.0.0.0/0)
- ✅ Add/remove routing entries dynamically

### 3. Packet History Tracking
- ✅ Complete packet journey recording
- ✅ Hop-by-hop trace information
- ✅ Loop detection mechanism
- ✅ Persistent storage to text file
- ✅ Multiple display formats (detailed, compact, reverse)
- ✅ Search and filter capabilities
- ✅ Statistics generation (total hops, delays, final actions)

### 4. Interactive Menu System
- ✅ User-friendly command-line interface
- ✅ Run simulation on demand
- ✅ View complete history file
- ✅ View specific packet histories
- ✅ Search packet histories by criteria
- ✅ Display routing table and statistics

---

## 🏗️ System Architecture

```
┌─────────────────────────────────────────────────────────┐
│                    RouterDriver                          │
│              (Main Controller & Menu)                    │
└──────────────┬──────────────────────┬───────────────────┘
               │                      │
       ┌───────▼────────┐    ┌───────▼────────┐
       │   QoService    │    │ RoutingTable   │
       │   (Activity 1) │    │  (Activity 2)  │
       └───────┬────────┘    └───────┬────────┘
               │                      │
       ┌───────▼────────┐    ┌───────▼────────┐
       │ Priority Queues│    │  RouterEntry   │
       │  High/Med/Low  │    │   (Routes)     │
       └────────────────┘    └────────────────┘
                │
        ┌───────▼────────┐
        │ PacketHistory  │
        │  (Activity 3)  │
        └───────┬────────┘
                │
        ┌───────▼────────┐
        │  TraceEntry    │
        │   (Hops)       │
        └────────────────┘
```

### Workflow

1. **Packet Arrival** → System reads packets from input file
2. **QoS Classification** → Packets sorted into priority queues
3. **Priority Processing** → High priority packets processed first
4. **Routing Decision** → LPM algorithm finds best route
5. **TTL Check** → Packets with TTL ≤ 0 are dropped
6. **History Recording** → Each action recorded in PacketHistory
7. **File Output** → Complete history saved to text file

---

## 📊 Data Structures

### Chapter 3 Data Structures (STL Containers)

#### 1. Queue (`std::queue`)
**Location:** `QoService` class  
**Purpose:** FIFO packet processing within priority levels  
**Operations:**
- `push()` - Enqueue packet - O(1)
- `pop()` - Dequeue packet - O(1)
- `front()` - Access next packet - O(1)
- `empty()` - Check if empty - O(1)

```cpp
std::queue<packets> highPriorityQueue;
std::queue<packets> mediumPriorityQueue;
std::queue<packets> lowPriorityQueue;
```

#### 2. Map (`std::map`)
**Location:** `RoutingTable` class  
**Purpose:** Store routing entries with efficient lookup  
**Operations:**
- `insert()` - Add route - O(log n)
- `find()` - Lookup route - O(log n)
- `erase()` - Remove route - O(log n)

```cpp
std::map<std::string, RouterEntry> routes;
// Key format: "networkPrefix/prefixLength"
// Example: "192.168.1.0/24"
```

### Chapter 4 Data Structures (STL Containers)

#### 3. List (`std::list`)
**Location:** `PacketHistory` class  
**Purpose:** Sequential storage of packet trace entries  
**Operations:**
- `push_back()` - Add trace entry - O(1)
- Forward iteration - O(n)
- Reverse iteration - O(n)

```cpp
std::list<TraceEntry> traceList;
```

**Why List?**
- Maintains insertion order (critical for hop sequence)
- Efficient insertion at both ends
- Bidirectional traversal for reverse traces
- No random access needed

#### 4. Set (`std::set`)
**Location:** `PacketHistory` class  
**Purpose:** Track visited routers for loop detection  
**Operations:**
- `insert()` - Add router ID - O(log n)
- `find()` - Check if visited - O(log n)
- No duplicates automatically

```cpp
std::set<std::string> visitedRouters;
```

---

## 🧮 Algorithms

### Longest Prefix Matching (LPM)

**Purpose:** Find the most specific (longest) matching route for a destination IP address

**Algorithm Steps:**
1. Convert destination IP to 32-bit integer
2. For each routing entry:
   - Convert network prefix to integer
   - Create subnet mask based on prefix length
   - Apply mask to both IPs
   - Compare masked values
3. Select route with longest matching prefix
4. If tie, select route with lowest metric

**Time Complexity:** O(n) where n = number of routes  
**Space Complexity:** O(1)

**Implementation:**
```cpp
RouterEntry* RoutingTable::findBestRoute(const string& destIP) {
    RouterEntry* bestMatch = nullptr;
    int longestMatch = -1;
    
    for (auto& pair : routes) {
        RouterEntry& entry = pair.second;
        
        if (ipMatchesPrefix(destIP, entry)) {
            int currentPrefixLen = entry.getPrefixLength();
            
            if (currentPrefixLen > longestMatch) {
                bestMatch = &entry;
                longestMatch = currentPrefixLen;
            }
            else if (currentPrefixLen == longestMatch && bestMatch != nullptr) {
                if (entry.getMetric() < bestMatch->getMetric()) {
                    bestMatch = &entry;
                }
            }
        }
    }
    
    return bestMatch;
}
```

**Example:**
```
Routing Table:
  192.168.1.0/24   -> Router_A (metric 1)
  192.168.0.0/16   -> Router_C (metric 2)
  0.0.0.0/0        -> DefaultGateway (metric 10)

Destination IP: 192.168.1.100

Matches:
  ✓ 192.168.1.0/24   (24 bits match)
  ✓ 192.168.0.0/16   (16 bits match)
  ✓ 0.0.0.0/0        (0 bits match)

Best Match: 192.168.1.0/24 (longest prefix = 24)
Forward to: Router_A
```

---

## 🛠️ Installation

### Prerequisites
- C++ compiler with C++11 support or later
  - GCC 4.8+ (Linux/macOS)
  - MSVC 2015+ (Windows)
  - Clang 3.3+
- Visual Studio 2017 or later (for Windows)
- Make utility (optional, for Linux/macOS)

### Windows (Visual Studio)

1. Clone or download the repository
```bash
git clone https://github.com/yourusername/Core-Router-Functionalities.git
cd Core-Router-Functionalities
```

2. Open `Core-Router-Functionalities.sln` in Visual Studio

3. Build the solution:
   - Press `Ctrl+Shift+B` or
   - Menu: Build → Build Solution

4. Run the application:
   - Press `F5` (Debug mode) or `Ctrl+F5` (Release mode)

### Linux/macOS

1. Clone the repository
```bash
git clone https://github.com/yourusername/Core-Router-Functionalities.git
cd Core-Router-Functionalities
```

2. Compile using g++
```bash
g++ -std=c++11 -o router *.cpp
```

3. Run the application
```bash
./router
```

### Alternative: Manual Compilation

```bash
# Compile all source files
g++ -c -std=c++11 Source.cpp
g++ -c -std=c++11 RouterDriver.cpp
g++ -c -std=c++11 qoservice.cpp
g++ -c -std=c++11 Packets.cpp
g++ -c -std=c++11 RoutingTable.cpp
g++ -c -std=c++11 RouterEntry.cpp
g++ -c -std=c++11 PacketHistory.cpp
g++ -c -std=c++11 TraceEntry.cpp

# Link object files
g++ -o router Source.o RouterDriver.o qoservice.o Packets.o RoutingTable.o RouterEntry.o PacketHistory.o TraceEntry.o

# Run
./router
```

---

## 📖 Usage

### Quick Start

1. **Prepare Input File**

Create `file.txt` with packet data:
```csv
ID,Source,Destination,Port,TTL
1,192.168.1.10,10.0.0.5,80,5
2,172.16.0.20,192.168.2.15,443,3
3,10.0.0.100,172.16.5.50,8080,4
```

Format: `ID,Source IP,Destination IP,Port,TTL`

2. **Run the Application**
```bash
./router
```

3. **Navigate the Menu**
```
========================================
         ROUTER MANAGEMENT MENU
========================================
1. Run Simulation
2. View Complete History File
3. View Specific Packet History
4. Search Packet Histories
5. Display Statistics
6. Display Routing Table
0. Exit
========================================
Enter your choice:
```

### Menu Options Explained

#### Option 1: Run Simulation
- Reads packets from `file.txt`
- Classifies packets by QoS priority
- Processes packets through routing table
- Records complete history
- Saves results to `packet_history.txt`

**Example Output:**
```
Initializing Router Components...
Configuring Routing Table...
Routing Table (7 routes):
  192.168.1.0/24 -> Router_A (Metric:1)
  192.168.2.0/24 -> Router_B (Metric:1)
  ...

Reading packets from file...
Loaded 15 packets
Classifying packets by QoS...
Queue Status - High:5 Medium:7 Low:3

Processing packets...
Packet 1 [ID:1] 192.168.1.10 -> 10.0.0.5 TTL:5 [FORWARDED to Router_D]
Packet 2 [ID:2] 172.16.0.20 -> 192.168.2.15 TTL:3 [FORWARDED to Router_B]
...

--- Processing Summary ---
Total Packets: 15
Forwarded: 12
Dropped (TTL): 2
Dropped (No Route): 1
```

#### Option 2: View Complete History File
- Displays entire contents of `packet_history.txt`
- Shows all packets with detailed hop information

#### Option 3: View Specific Packet History
- Prompts for packet ID
- Displays detailed history for that packet
- Shows hop count, total delay, final action

**Example:**
```
Available Packet IDs: 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
Enter Packet ID to view: 1

========================================
PACKET FLOW HISTORY - Packet ID: 1
========================================
Hop #1: Router ID: Router_Main
        Action: RECEIVED
        Queue Delay: 0 ms
        Remaining TTL: 5
        Next Hop: 

Hop #2: Router ID: Router_Main
        Action: FORWARDED
        Queue Delay: 5 ms
        Remaining TTL: 4
        Next Hop: Router_D
----------------------------------------
Total Hops: 2 | Total Delay: 5ms
Final Action: FORWARDED
========================================
```

#### Option 4: Search Packet Histories
Two search modes:

**By Final Action:**
```
Search by:
1. Final Action
2. Minimum Hops
Enter choice: 1

Enter action to search (FORWARDED, DROPPED_TTL, DROPPED_NO_ROUTE): DROPPED_TTL

Packets with final action 'DROPPED_TTL':
Packet 6: Router_Main => Router_Main [DROPPED_TTL, 2 hops, 0ms]
Packet 14: Router_Main => Router_Main [DROPPED_TTL, 2 hops, 0ms]
```

**By Minimum Hops:**
```
Enter minimum number of hops: 2

Packets with at least 2 hops:
Packet 1: Router_Main => Router_Main [FORWARDED, 2 hops, 5ms]
Packet 2: Router_Main => Router_Main [FORWARDED, 2 hops, 5ms]
...
```

#### Option 5: Display Statistics
Shows current system statistics:
```
--- Final Statistics ---
Total Routes: 7
Packet Histories Stored: 15
Queue Status - High:0 Medium:0 Low:0
```

#### Option 6: Display Routing Table
Lists all configured routes:
```
Routing Table (7 routes):
  192.168.1.0/24 -> Router_A (Metric:1)
  192.168.2.0/24 -> Router_B (Metric:1)
  192.168.0.0/16 -> Router_C (Metric:2)
  10.0.0.0/8 -> Router_D (Metric:3)
  172.16.0.0/12 -> Router_E (Metric:2)
  172.20.0.0/16 -> Router_F (Metric:1)
  0.0.0.0/0 -> DefaultGateway (Metric:10)
```

---

## 📁 File Structure

```
Core-Router-Functionalities/
│
├── Source.cpp                 # Main entry point
├── RouterDriver.cpp           # Main controller implementation
├── RouterDriver.h             # Controller header
│
├── qoservice.cpp              # QoS management implementation
├── qoservice.h                # QoS header
├── Packets.cpp                # Packet class implementation
├── Packets.h                  # Packet class header
│
├── RoutingTable.cpp           # Routing table implementation
├── RoutingTable.h             # Routing table header
├── RouterEntry.cpp            # Route entry implementation
├── RouterEntry.h              # Route entry header
│
├── PacketHistory.cpp          # History tracking implementation
├── PacketHistory.h            # History tracking header
├── TraceEntry.cpp             # Trace entry implementation
├── TraceEntry.h               # Trace entry header
│
├── file.txt                   # Input packet data (CSV)
├── packet_history.txt         # Output history file (generated)
│
├── Core-Router-Functionalities.sln      # Visual Studio solution
├── Core-Router-Functionalities.vcxproj  # VS project file
├── .gitignore                 # Git ignore rules
└── README.md                  # This file
```

---

## 📚 Class Documentation

### 1. RouterDriver

**Purpose:** Main controller and integration point for all activities

**Key Methods:**
```cpp
void run()                    // Start menu system
void initializeComponents()   // Initialize QoS and routing table
void configureRoutingTable()  // Set up initial routes
void processPackets()         // Main packet processing loop
void saveHistoryToFile()      // Save histories to file
void displayMenu()            // Interactive menu
void viewHistoryFile()        // Display history file
void viewPacketHistory()      // View specific packet
void searchPacketHistory()    // Search functionality
```

**Members:**
- `QoService* qos` - QoS management system
- `RoutingTable* routingTable` - Routing table
- `map<int, PacketHistory> packetHistories` - All packet histories

---

### 2. QoService

**Purpose:** Quality of Service management and packet classification

**Key Methods:**
```cpp
vector<packets> readPacketsFromFile(const string& filename)
// Time Complexity: O(n) - Read n packets from file

void classifyPackets(const vector<packets>& packetVec)
// Time Complexity: O(n) - Classify n packets into queues

packets getNextPacket()
// Time Complexity: O(1) - Dequeue from highest priority

bool allQueuesEmpty() const
// Time Complexity: O(1) - Check if all queues empty

void displayQueueStatus() const
// Time Complexity: O(1) - Display queue counts
```

**Priority Classification:**
- **High Priority (0-1023):** Well-known ports (HTTP, HTTPS, SSH, DNS)
- **Medium Priority (1024-49151):** Registered ports
- **Low Priority (49152-65535):** Dynamic/private ports

**Members:**
```cpp
queue<packets> highPriorityQueue
queue<packets> mediumPriorityQueue
queue<packets> lowPriorityQueue
int maxQueueSize  // Queue capacity limit
```

---

### 3. packets

**Purpose:** Represent individual network packets

**Attributes:**
```cpp
int id                    // Unique packet identifier
string source             // Source IP address
string destination        // Destination IP address
int port                  // Destination port number
int TTL                   // Time To Live
string priority           // QoS priority level
```

**Key Methods:**
```cpp
void decrementTTL()       // Reduce TTL by 1
void setPriority(const string& priority)
void display() const      // Print packet information
```

---

### 4. RoutingTable

**Purpose:** Manage routing entries and perform route lookups

**Key Methods:**
```cpp
void addRoute(const string& prefix, int prefixLen, 
              const string& nextHop, int metric = 1)
// Time Complexity: O(log n) - Insert into map

void removeRoute(const string& prefix, int prefixLen)
// Time Complexity: O(log n) - Erase from map

RouterEntry* findBestRoute(const string& destIP)
// Time Complexity: O(n) - Longest prefix matching

unsigned long ipToInt(const string& ip)
// Time Complexity: O(1) - Convert IP to integer

bool ipMatchesPrefix(const string& destIP, const RouterEntry& entry)
// Time Complexity: O(1) - Check if IP matches route
```

**Members:**
```cpp
map<string, RouterEntry> routes  // Key: "prefix/length"
```

---

### 5. RouterEntry

**Purpose:** Represent a single routing table entry

**Attributes:**
```cpp
string networkPrefix      // Network address (e.g., "192.168.1.0")
int prefixLength          // CIDR prefix length (e.g., 24)
string nextHop            // Next hop router ID
int metric                // Route cost/preference
```

---

### 6. PacketHistory

**Purpose:** Track complete journey of a packet through the router

**Key Methods:**
```cpp
void addTrace(const TraceEntry& entry)
// Time Complexity: O(1) - Add to list, O(log n) - Add to set

bool hasVisitedRouter(const string& routerID) const
// Time Complexity: O(log n) - Set lookup

bool detectLoop(const string& routerID)
// Time Complexity: O(log n) - Check visited set

void displayHistory() const
// Time Complexity: O(h) - h = number of hops

int getTotalDelay() const
// Time Complexity: O(h) - Sum delays

string getFinalAction() const
// Time Complexity: O(1) - Last element access
```

**Members:**
```cpp
int packetID
list<TraceEntry> traceList          // Sequential hop records
set<string> visitedRouters          // For loop detection
```

---

### 7. TraceEntry

**Purpose:** Record a single hop in a packet's journey

**Attributes:**
```cpp
string routerID          // Router that processed packet
time_t timestamp         // When action occurred
string action            // RECEIVED, FORWARDED, DROPPED_TTL, etc.
int queueDelay           // Processing delay in milliseconds
int remainingTTL         // TTL after processing
string nextHop           // Where packet was forwarded
```

---

## ⚙️ Configuration

### Modifying Queue Sizes

Edit `RouterDriver` constructor:
```cpp
RouterDriver driver("file.txt", 20);  // Max 20 packets per queue
```

### Adding New Routes

Edit `RouterDriver::configureRoutingTable()`:
```cpp
routingTable->addRoute("10.10.0.0", 16, "Router_X", 2);
```

### Changing Input File

Edit `RouterDriver` constructor:
```cpp
RouterDriver driver("my_packets.txt", 10);
```

### Customizing History File Name

Edit `RouterDriver` constructor initialization:
```cpp
RouterDriver::RouterDriver(const string& filename, int maxQueueSize)
    : inputFile(filename), historyFile("my_custom_history.txt") {
    // ...
}
```

---

## 💡 Examples

### Example 1: Basic Simulation

**Input (file.txt):**
```csv
ID,Source,Destination,Port,TTL
1,192.168.1.10,10.0.0.5,80,5
2,172.16.0.20,192.168.2.15,443,3
3,10.0.0.100,172.16.5.50,8080,4
```

**Output:**
```
Processing packets...
Packet 1 [ID:1] 192.168.1.10 -> 10.0.0.5 TTL:5 [FORWARDED to Router_D]
Packet 2 [ID:2] 172.16.0.20 -> 192.168.2.15 TTL:3 [FORWARDED to Router_B]
Packet 3 [ID:3] 10.0.0.100 -> 172.16.5.50 TTL:4 [FORWARDED to Router_E]

--- Processing Summary ---
Total Packets: 3
Forwarded: 3
Dropped (TTL): 0
Dropped (No Route): 0
```

### Example 2: TTL Expiration

**Input:**
```csv
ID,Source,Destination,Port,TTL
1,192.168.1.10,10.0.0.5,80,1
```

**Output:**
```
Packet 1 [ID:1] 192.168.1.10 -> 10.0.0.5 TTL:1 [DROPPED - TTL Expired]
```

**Explanation:** TTL decremented to 0 before routing decision

### Example 3: No Route Available

**Input:**
```csv
ID,Source,Destination,Port,TTL
1,200.200.200.200,250.250.250.250,80,5
```

**Output:**
```
Packet 1 [ID:1] 200.200.200.200 -> 250.250.250.250 TTL:5 [DROPPED - No Route]
```

**Explanation:** No route matches destination, not even default gateway

---

## ⏱️ Time Complexity Analysis

### Activity 1: QoS Management

| Operation | Time Complexity | Explanation |
|-----------|----------------|-------------|
| Read packets from file | O(n) | Read n packets sequentially |
| Classify packets | O(n) | Process each packet once |
| Enqueue packet | O(1) | Queue push operation |
| Dequeue packet | O(1) | Queue pop operation |
| Check all queues empty | O(1) | Three constant checks |

**Overall Complexity:** O(n) where n = number of packets

### Activity 2: Routing Table Management

| Operation | Time Complexity | Explanation |
|-----------|----------------|-------------|
| Add route | O(log r) | Map insertion, r = routes |
| Remove route | O(log r) | Map deletion |
| Find best route (LPM) | O(r) | Linear search through routes |
| IP to integer conversion | O(1) | Fixed 4 octets |
| IP prefix matching | O(1) | Bitwise operations |

**Overall Complexity:** O(n × r) for processing n packets with r routes

### Activity 3: Packet History Tracking

| Operation | Time Complexity | Explanation |
|-----------|----------------|-------------|
| Add trace entry | O(log r) + O(1) | Set insert + List append |
| Check visited router | O(log r) | Set lookup, r = routers |
| Detect loop | O(log r) | Set lookup |
| Display history | O(h) | Iterate h hops |
| Calculate total delay | O(h) | Sum h delays |
| Save to file | O(p × h) | p packets × h hops each |

**Overall Complexity:** O(p × h × log r) where p = packets, h = hops, r = routers

### Combined System Complexity

**Total Time Complexity:** O(n × r + p × h × log r)

- n = number of input packets
- r = number of routes
- p = number of processed packets (p ≤ n)
- h = average hops per packet

**Space Complexity:** O(n + r + p × h)

- O(n): Queue storage for packets
- O(r): Routing table entries
- O(p × h): Packet histories with traces

---

## 🔍 Troubleshooting

### Common Issues

**Issue 1: File not found**
```
Error: Cannot open file file.txt
```
**Solution:** Ensure `file.txt` is in the same directory as the executable

**Issue 2: Invalid packet format**
```
Loaded 0 packets
```
**Solution:** Check CSV format. Header row required: `ID,Source,Destination,Port,TTL`

**Issue 3: Compilation errors**
```
error: 'to_string' is not a member of 'std'
```
**Solution:** Use C++11 or later: `g++ -std=c++11 ...`

**Issue 4: History file empty**
```
Error: Cannot open history file packet_history.txt
```
**Solution:** Run simulation (Option 1) first to generate history file

---

## 🤝 Contributing

Contributions are welcome! Please follow these guidelines:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

### Code Style Guidelines
- Use meaningful variable names
- Add comments for complex logic
- Follow existing indentation (4 spaces)
- Include time complexity comments for algorithms
- Update README for new features

---

## 📝 License

This project is created for educational purposes as part of CSC 313 coursework.

---

## 🙏 Acknowledgments

- Computer Networks concepts from CSC 313 course
- STL documentation and C++ references
- Router simulation design patterns from networking literature

---

## 📞 Contact

For questions or issues, please:
- Open an issue on GitHub
- Contact via course communication channels

---

**Last Updated:** October 19, 2025  
**Version:** 1.0.0
