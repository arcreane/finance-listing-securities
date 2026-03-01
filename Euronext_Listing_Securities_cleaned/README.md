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
12. [Running Tests](#running-tests)
13. [Troubleshooting](#troubleshooting)

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
| **Login System** | Local authentication using a JSON file with SHA-256 hashed passwords |
| **Ping/Latency** | Measures round-trip time to the server in milliseconds |
| **Download Speed** | Tests download bandwidth by receiving a 1 MB payload |
| **Upload Speed** | Tests upload bandwidth by sending a 1 MB payload |
| **Connection Type** | Auto-detects WiFi, Ethernet, or other connection types |
| **Visual Gauge** | Animated semicircular gauge with gradient coloring |
| **Status Indicator** | Green/red dot showing server online/offline status |
| **Auto-Refresh** | Ping updates automatically every second |
| **Multi-Server Support** | Switch between 3 servers: Server 1, Server 2 (local), and Real Server (AlwaysData) |
| **Auto-Server Failover** | Automatically switches to the next server after 3 consecutive failed pings |
| **AlwaysData Integration** | Remote production server hosted on AlwaysData with MySQL database |
| **Environment Modes** | Launch with `--env local` or `--env real` to select the target environment |
| **Server Identity Proof** | Displays a unique server identifier to verify which backend you are connected to |

---

## Project Structure

```
networkuser_full_project/
├── networkuser.pro              # Qt project file (qmake)
├── build_release.bat            # One-click release build script (Windows)
├── README.md                    # This documentation
│
├── src/                         # Source code
│   ├── main.cpp                 # Application entry point + env flag parsing
│   ├── Config.h                 # Environment configuration (local/real URLs)
│   │
│   ├── ui/                      # User interface components
│   │   ├── LoginWindow.h/.cpp       # Login screen (SHA-256 auth)
│   │   ├── NetworkWindow.h/.cpp     # Main dashboard (multi-server)
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
│   ├── real_server.py           # Flask server (WSGI-ready for AlwaysData)
│   ├── server.py                # Local Flask development server
│   ├── test_server.py           # Python unit tests (pytest)
│   └── requirements.txt         # Python dependencies
│
├── tests/                       # Qt unit tests
│   ├── tests.pro                # Test project file
│   ├── tst_apiclient.cpp        # ApiClient tests
│   ├── tst_networkmonitor.cpp   # NetworkMonitor tests
│   └── tst_speedgaugewidget.cpp # SpeedGaugeWidget tests
│
├── release/                     # Release build output
└── build/                       # Debug build output (Qt Creator)
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
- **Qt Creator** (recommended) or command-line qmake
- **Visual Studio 2022** Build Tools (for MSVC on Windows)

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

#### Using the Release Build Script (Windows)
```powershell
# From the project root directory
.\build_release.bat
```
This script automatically:
1. Initializes the MSVC 2022 x64 environment
2. Runs `qmake` on `networkuser.pro` with `CONFIG+=release`
3. Compiles with `nmake` and outputs `release/networkuser.exe`

> **Note:** Make sure the application is not running before recompiling, otherwise the linker will fail with `LNK1104` (file locked).

#### Using Command Line (Manual)
```powershell
# Open a Developer Command Prompt or initialize MSVC environment:
cmd /c "call ""C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"" && cd /d ""<project-root>"" && ""C:\Qt\6.10.0\msvc2022_64\bin\qmake.exe"" networkuser.pro CONFIG+=release && nmake /f Makefile.Release"
```

#### Rebuilding After Code Changes
1. **Close the running application** (the `.exe` must not be locked)
2. Run `build_release.bat` or use Qt Creator **Build** → **Rebuild**
3. The updated executable is at `release/networkuser.exe`

---

## Running the Application

The application supports two environments:

| Environment | Flag | Description |
|-------------|------|-------------|
| **Local** | `--env local` (default) | Connects to a local Flask server on `127.0.0.1:8080` |
| **Real** | `--env real` | Connects directly to the AlwaysData remote server |

### Option A: Local Mode (development)

#### Step 1: Start the Local Backend Server

```bash
cd server
python server.py
```

You should see:
```
============================================================
  STARTING LOCAL SERVER
  MODE: LOCAL
  PROOF: IDENTIFIER: [LOCAL_VIRTUAL_EMULATOR]
  Access at: http://127.0.0.1:8080  (local)
  Access at: http://192.168.x.x:8080  (network)
============================================================
```

#### Step 2: Run the Qt Application

```powershell
# From the project root
.\release\networkuser.exe
```

Or from Qt Creator: click the **Run** button (green play icon).

### Option B: Real Mode (production — AlwaysData)

No local server needed! The client connects directly to the remote AlwaysData server.

```powershell
# From the project root
.\release\networkuser.exe --env real
```

The application will:
- Automatically select the **Real Server (AlwaysData)**
- Show `Active server: Real (AlwaysData)` in the UI
- Connect to `http://guemoune4.alwaysdata.net`
- Display the server identity proof: `IDENTIFIER: [ALWAYS_DATA_SECURE_REMOTE_v1.2]`

### Step 3: Log In

Use one of these default credentials:

| Username | Password |
|----------|----------|
| admin    | admin123 |
| user     | password |
| Piou     | Piou1234 |

### Multi-Server Switching

Once logged in, click the **Switch Server** button to cycle through:
1. **Server 1** — `http://127.0.0.1:8080` (local)
2. **Server 2** — `http://127.0.0.1:8081` (local)
3. **Real Server** — `http://guemoune4.alwaysdata.net` (remote)

The application also features **automatic failover**: after 3 consecutive failed pings, it automatically switches to the next server.

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

### Environment Configuration

The environment (local or real) is configured in `src/Config.h`:

```cpp
static QString getBaseUrl() {
    if (currentEnv == Environment::Real) {
        return "http://guemoune4.alwaysdata.net";
    }
    return "http://127.0.0.1:8080";
}
```

To change the AlwaysData URL, edit the `return` value in the `Real` branch.

### Changing the Backend URL at Runtime

In the application, you can also edit the URL fields directly in the UI:
- **Server 1:** `http://127.0.0.1:8080`
- **Server 2:** `http://127.0.0.1:8081`
- **Real Server:** `http://guemoune4.alwaysdata.net`

### Adding New Users

Edit `src/ressources/users.json`:

```json
{
  "users": [
    {
      "username": "admin",
      "password": "240be518fabd2724ddb6f04eeb1da5967448d7e831c08c8fa822809f74c720a9"
    }
  ]
}
```

> **Note:** Passwords are stored as **SHA-256 hashes**. To generate a hash, use:
> ```bash
> python -c "import hashlib; print(hashlib.sha256(b'your_password').hexdigest())"
> ```

After modifying, rebuild the project to copy the updated file to the build directory.

### AlwaysData Server Configuration

The production server (`server/real_server.py`) is deployed on AlwaysData with:
- **URL:** `http://guemoune4.alwaysdata.net`
- **Database:** MySQL on `mysql-guemoune4.alwaysdata.net`
- **WSGI:** uWSGI with `real_server:app`
- **Environment Variable:** `DB_PASSWORD` must be set in the AlwaysData panel

### Changing Speed Test Payload Size

In `server/server.py` (or `real_server.py`), modify:

```python
payload = b'X' * (1024 * 1024)  # 1 MB - Change this value
```

In `src/services/ApiClient.cpp`, modify:

```cpp
const int uploadSize = 1 * 1024 * 1024; // 1 MB - Change this value
```

---

## API Reference

### Base URLs
| Environment | URL |
|-------------|-----|
| Local | `http://127.0.0.1:8080` |
| Real (AlwaysData) | `http://guemoune4.alwaysdata.net` |

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

## Running Tests

The project includes comprehensive unit tests for both the Python server and the Qt client.

### Python Server Tests

Located in `server/test_server.py`. Uses **pytest** framework.

```bash
cd server

# Install test dependencies
pip install -r requirements.txt

# Run all tests
pytest test_server.py -v

# Run specific test class
pytest test_server.py::TestPingEndpoint -v

# Run with coverage report
pytest test_server.py --cov=server --cov-report=html
```

**Test Coverage:**
| Test Class | Description |
|------------|-------------|
| `TestPingEndpoint` | Tests `/ping` endpoint response and latency |
| `TestDownloadSpeedEndpoint` | Tests download payload size and format |
| `TestUploadSpeedEndpoint` | Tests upload handling and byte counting |
| `TestRootEndpoint` | Tests server info endpoint |
| `TestErrorHandling` | Tests 404 and method not allowed errors |

### Qt Client Tests

Located in `tests/` folder. Uses **Qt Test** framework.

```bash
cd tests

# Build all tests
qmake tests.pro
make  # or nmake on Windows

# Run individual tests
./tst_apiclient
./tst_networkmonitor
./tst_speedgaugewidget
```

**Or build individual tests:**
```bash
# Build ApiClient test
qmake tst_apiclient.pro
make
./tst_apiclient

# Build NetworkMonitor test
qmake tst_networkmonitor.pro
make
./tst_networkmonitor

# Build SpeedGaugeWidget test
qmake tst_speedgaugewidget.pro
make
./tst_speedgaugewidget
```

**Test Files:**
| Test File | Tests |
|-----------|-------|
| `tst_apiclient.cpp` | Ping, download, upload signal emission and error handling |
| `tst_networkmonitor.cpp` | Connection type detection (WiFi/Ethernet/Unknown) |
| `tst_speedgaugewidget.cpp` | Value clamping, range setting, rendering |

**Note:** ApiClient tests require the backend server to be running for full integration testing.

### Running All Tests

```bash
# 1. Start the backend server (in one terminal)
cd server
python server.py

# 2. Run Python tests (in another terminal)
cd server
pytest test_server.py -v

# 3. Run Qt tests (in another terminal)
cd tests
# Build and run each test
```

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

### AlwaysData Server Returns Bad Gateway
**Cause:** The uWSGI configuration is incorrect or Flask is not installed.

**Solution:**
1. Check the AlwaysData site config:
   - Type: `Python WSGI`
   - Application path: `/home/guemoune4/server/real_server.py`
   - uWSGI parameter: `callable = app`
2. Verify Flask is installed via SSH:
   ```bash
   /usr/alwaysdata/python/3.14/bin/python3 -c "from real_server import app; print('OK')"
   ```
3. Check AlwaysData logs:
   ```bash
   cat ~/admin/logs/sites/2026/sites-*.log | tail -20
   ```

### Database Not Connected (`db_connected: false`)
**Cause:** The `DB_PASSWORD` environment variable is not set or contains extra characters.

**Solution:**
1. On the AlwaysData panel, go to Web > Sites > your site > Variables d'environnement
2. Set `DB_PASSWORD=YourPasswordHere` (no spaces, no quotes)
3. Restart the site

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

**Developed with Qt 6 and Python Flask — Hosted on AlwaysData**
