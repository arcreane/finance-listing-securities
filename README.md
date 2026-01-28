# Network User

A modern desktop application for monitoring network connectivity, measuring latency (ping), and testing internet speed (download/upload). Built with **Qt 6** (C++) for the frontend and **Python Flask** for the backend server.

---

## Table of Contents

1. [Overview](#overview)
2. [Features](#features)
3. [Project Structure](#project-structure)
4. [Architecture](#architecture)
5. [Prerequisites](#prerequisites)
6. [Installation](#installation)
7. [Running the Application](#running-the-application)
8. [Usage Guide](#usage-guide)
9. [Configuration](#configuration)
10. [API Reference](#api-reference)
11. [Components Documentation](#components-documentation)
12. [Troubleshooting](#troubleshooting)

---

## Overview

**Network User** is a network monitoring tool that provides:
- Real-time latency (ping) monitoring with auto-refresh
- Download and upload speed testing
- Network connection type detection (WiFi, Ethernet, etc.)
- A clean, modern UI with visual gauges and status indicators

The application consists of two parts:
1. **Qt Desktop Client** - The main user interface
2. **Python Backend Server** - Provides endpoints for network tests

---

## Features

| Feature | Description |
|---------|-------------|
| **Login System** | Local authentication using a JSON file with username/password |
| **Ping/Latency** | Measures round-trip time to the server in milliseconds |
| **Download Speed** | Tests download bandwidth by receiving a 1 MB payload |
| **Upload Speed** | Tests upload bandwidth by sending a 1 MB payload |
| **Connection Type** | Auto-detects WiFi, Ethernet, or other connection types |
| **Visual Gauge** | Animated semicircular gauge with gradient coloring |
| **Status Indicator** | Green/red dot showing server online/offline status |
| **Auto-Refresh** | Ping updates automatically every second |

---

## Project Structure

```
networkuser_full_project/
├── networkuser.pro              # Qt project file (qmake)
├── README.md                    # This documentation
│
├── src/                         # Source code
│   ├── main.cpp                 # Application entry point
│   │
│   ├── ui/                      # User interface components
│   │   ├── LoginWindow.h/.cpp       # Login screen
│   │   ├── NetworkWindow.h/.cpp     # Main dashboard
│   │   ├── SpeedGaugeWidget.h/.cpp  # Animated gauge widget
│   │   └── Theme.h/.cpp             # Application styling (QSS)
│   │
│   ├── services/                # Backend services
│   │   ├── ApiClient.h/.cpp         # HTTP client for server communication
│   │   └── NetworkMonitor.h/.cpp    # Connection type detection
│   │
│   └── ressources/              # Static resources
│       └── users.json               # User credentials database
│
├── server/                      # Backend server
│   ├── server.py                # Flask server with all endpoints
│   └── requirements.txt         # Python dependencies
│
└── build/                       # Build output (generated)
```

---

## Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                        Qt Desktop Client                         │
├─────────────────────────────────────────────────────────────────┤
│                                                                   │
│  ┌─────────────┐    ┌──────────────┐    ┌───────────────────┐   │
│  │ LoginWindow │───>│ NetworkWindow │<───│ SpeedGaugeWidget │   │
│  └─────────────┘    └──────────────┘    └───────────────────┘   │
│         │                  │                                      │
│         │                  │                                      │
│         v                  v                                      │
│  ┌─────────────┐    ┌─────────────┐                              │
│  │ users.json  │    │  ApiClient  │                              │
│  │ (local auth)│    │ (HTTP calls)│                              │
│  └─────────────┘    └──────┬──────┘                              │
│                            │                                      │
└────────────────────────────│──────────────────────────────────────┘
                             │ HTTP
                             v
┌─────────────────────────────────────────────────────────────────┐
│                     Python Flask Server                          │
├─────────────────────────────────────────────────────────────────┤
│                                                                   │
│  ┌─────────────────────────────────────────────────────────┐    │
│  │  Endpoints:                                              │    │
│  │    GET  /ping              → Latency test               │    │
│  │    GET  /speedtest/download → Returns 1 MB payload      │    │
│  │    POST /speedtest/upload   → Accepts upload data       │    │
│  └─────────────────────────────────────────────────────────┘    │
│                                                                   │
└─────────────────────────────────────────────────────────────────┘
```

---

## Prerequisites

### For the Qt Client
- **Qt 6.x** (tested with Qt 6.10)
- **C++17** compatible compiler (MSVC 2022, GCC, Clang)
- **Qt Creator** (recommended) or command-line qmake/CMake

### For the Backend Server
- **Python 3.8+**
- **pip** (Python package manager)

---

## Installation

### 1. Clone or Download the Project

```bash
cd /path/to/your/projects
# If using git:
git clone <repository-url>
cd networkuser_full_project
```

### 2. Set Up the Backend Server

```bash
# Navigate to server directory
cd server

# Create a virtual environment (recommended)
python -m venv venv

# Activate the virtual environment
# On Windows:
venv\Scripts\activate
# On macOS/Linux:
source venv/bin/activate

# Install dependencies
pip install -r requirements.txt
```

### 3. Build the Qt Client

#### Using Qt Creator (Recommended)
1. Open `networkuser.pro` in Qt Creator
2. Configure the project for your Qt kit (e.g., Desktop Qt 6.10 MSVC2022 64bit)
3. Click **Build** → **Build Project "networkuser"**

#### Using Command Line
```bash
# Navigate to project root
cd networkuser_full_project

# Create build directory
mkdir build && cd build

# Run qmake
qmake ../networkuser.pro

# Build
# On Windows with MSVC:
nmake
# On Linux/macOS:
make
```

---

## Running the Application

### Step 1: Start the Backend Server

```bash
cd server
python server.py
```

You should see:
```
============================================================
  Network User Backend Server
============================================================

  Endpoints:
    GET  /              - Server status
    GET  /ping          - Latency test
    GET  /speedtest/download - Download speed test
    POST /speedtest/upload   - Upload speed test

  Starting server on http://127.0.0.1:8080
============================================================
```

### Step 2: Run the Qt Application

#### From Qt Creator
- Click the **Run** button (green play icon)

#### From Command Line
```bash
# Navigate to build output
cd build/debug  # or release

# Run the executable
./networkuser    # Linux/macOS
networkuser.exe  # Windows
```

### Step 3: Log In

Use one of these default credentials:

| Username | Password |
|----------|----------|
| admin    | admin123 |
| user     | password |
| Piou     | Piou1234 |

---

## Usage Guide

### Main Dashboard

After logging in, you'll see the main network monitoring dashboard:

```
┌─────────────────────────────────────────────────────────┐
│ Network User                                        [●] │  ← Status dot (green=online)
├─────────────────────────────────────────────────────────┤
│ [http://127.0.0.1:8080                               ]  │  ← Backend URL
├─────────────────────────────────────────────────────────┤
│ ┌───────────────┐ ┌───────────────┐                     │
│ │ Connection    │ │ Latency       │                     │
│ │ WiFi          │ │ 12 ms         │                     │  ← Info cards
│ └───────────────┘ └───────────────┘                     │
│ ┌───────────────────────────────────────────────────┐   │
│ │ Server status                                      │   │
│ │ UP                                                 │   │
│ └───────────────────────────────────────────────────┘   │
├─────────────────────────────────────────────────────────┤
│  PING         DOWNLOAD       UPLOAD                     │
│  12 ms        45.32 Mbps     12.50 Mbps                │  ← Speed stats
│                                                          │
│            ╭─────────────────╮                          │
│           ╱                   ╲                         │
│          │      12 ms         │                         │  ← Gauge
│          │       PING         │                         │
│           ╲_________●________╱                          │
│                                                          │
├─────────────────────────────────────────────────────────┤
│  [Ping]  [Speed Test]                                   │  ← Action buttons
│  [         Reboot Connection          ]                 │
└─────────────────────────────────────────────────────────┘
```

### Buttons

| Button | Action |
|--------|--------|
| **Ping** | Manually trigger a ping test |
| **Speed Test** | Run download and upload speed tests |
| **Reboot Connection** | Placeholder for network reset (not implemented) |

### Auto-Refresh

The ping/latency is automatically refreshed every **1 second**. The gauge needle and all latency displays update in real-time.

---

## Configuration

### Changing the Backend URL

In the application, edit the URL field at the top to point to your server:
- Default: `http://127.0.0.1:8080`
- For remote servers: `http://<your-server-ip>:8080`

### Adding New Users

Edit `src/ressources/users.json`:

```json
{
  "users": [
    {
      "username": "admin",
      "password": "admin123"
    },
    {
      "username": "newuser",
      "password": "newpassword"
    }
  ]
}
```

**Note:** After modifying, rebuild the project to copy the updated file to the build directory.

### Changing Speed Test Payload Size

In `server/server.py`, modify:

```python
DOWNLOAD_SIZE = 1 * 1024 * 1024  # 1 MB - Change this value
```

In `src/services/ApiClient.cpp`, modify:

```cpp
const int uploadSize = 1 * 1024 * 1024; // 1 MB - Change this value
```

---

## API Reference

### Base URL
```
http://127.0.0.1:8080
```

### Endpoints

#### GET /
Returns server status information.

**Response:**
```json
{
  "name": "Network User Backend",
  "version": "1.0.0",
  "status": "running",
  "endpoints": {
    "/ping": "GET - Latency test",
    "/speedtest/download": "GET - Download speed test (1 MB payload)",
    "/speedtest/upload": "POST - Upload speed test"
  }
}
```

#### GET /ping
Returns a simple response for latency measurement.

**Response:**
```json
{
  "status": "pong",
  "message": "pong",
  "timestamp": 1706450000.123
}
```

#### GET /speedtest/download
Returns a 1 MB binary payload for download speed testing.

**Response:**
- Content-Type: `application/octet-stream`
- Body: 1 MB of binary data

#### POST /speedtest/upload
Accepts binary data for upload speed testing.

**Request:**
- Content-Type: `application/octet-stream`
- Body: Binary data (any size)

**Response:**
```json
{
  "ok": true,
  "bytes_received": 1048576,
  "message": "Received 1048576 bytes"
}
```

---

## Components Documentation

### LoginWindow
**File:** `src/ui/LoginWindow.h/.cpp`

Handles user authentication against the local `users.json` file.

| Signal | Description |
|--------|-------------|
| `loginSuccess()` | Emitted when login credentials are valid |

### NetworkWindow
**File:** `src/ui/NetworkWindow.h/.cpp`

The main dashboard window displaying all network information.

| Component | Description |
|-----------|-------------|
| `m_backendUrl` | Text field for server URL |
| `m_pingGauge` | Visual gauge for ping display |
| `m_statusDot` | Green/red connection indicator |
| `m_pingTimer` | 1-second auto-refresh timer |

### SpeedGaugeWidget
**File:** `src/ui/SpeedGaugeWidget.h/.cpp`

A custom QWidget that renders an animated semicircular gauge.

| Method | Description |
|--------|-------------|
| `setRange(min, max)` | Set gauge value range |
| `setValue(v)` | Set current needle position |
| `setUnit(u)` | Set display unit (e.g., "ms", "Mbps") |
| `setLabel(l)` | Set label text (e.g., "PING") |

### ApiClient
**File:** `src/services/ApiClient.h/.cpp`

HTTP client for communicating with the backend server.

| Method | Signal | Description |
|--------|--------|-------------|
| `ping(base)` | `pingFinished(ok, ms, msg)` | Measure latency |
| `testDownloadSpeed(base)` | `downloadSpeedFinished(ok, mbps, msg)` | Test download speed |
| `testUploadSpeed(base)` | `uploadSpeedFinished(ok, mbps, msg)` | Test upload speed |
| `login(base, user, pass)` | `loginFinished(success, user, token, err)` | Server-based login |

### NetworkMonitor
**File:** `src/services/NetworkMonitor.h/.cpp`

Monitors the local network interface to detect connection type.

| Method | Description |
|--------|-------------|
| `connectionType()` | Returns current type: "WiFi", "Ethernet", "Connected", or "Unknown" |

| Signal | Description |
|--------|-------------|
| `connectionTypeChanged(type)` | Emitted when connection type changes |

### Theme
**File:** `src/ui/Theme.h/.cpp`

Applies the application-wide Qt Style Sheet (QSS).

Colors:
- Primary: `#7a5cff` (purple)
- Background: `#f6f6f6` (light gray)
- Cards: `#ffffff` (white)
- Online status: `#24c96b` (green)
- Offline status: `#e34b4b` (red)

---

## Troubleshooting

### "Cannot open users.json"
**Cause:** The `users.json` file is not in the same directory as the executable.

**Solution:**
- Rebuild the project (qmake copies it automatically on Windows)
- Or manually copy `src/ressources/users.json` to the build output folder

### Latency shows 4000+ ms
**Cause:** The backend server is not running or not reachable.

**Solution:**
1. Ensure the Flask server is running (`python server.py`)
2. Check the URL in the application matches the server address
3. Check firewall settings

### Speed Test shows "Error"
**Cause:** The `/speedtest/download` or `/speedtest/upload` endpoints are not available.

**Solution:**
1. Ensure you're using the Python backend server (not a third-party URL)
2. Check the server console for error messages

### IDE shows Qt include errors
**Cause:** Qt include paths are not configured in your IDE.

**Solution:**
- These are IntelliSense/IDE configuration issues, not compilation errors
- The project will compile correctly with qmake/Qt Creator
- Configure your IDE's include paths to point to your Qt installation

### Connection Type shows "Unknown"
**Cause:** No active network interface detected.

**Solution:**
- Check your network connection
- The detection works best on Windows with standard WiFi/Ethernet adapters

---

## License

This project is provided for educational purposes.

---

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Submit a pull request

---

**Developed with Qt 6 and Python Flask**
