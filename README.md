# DALI Standalone Editor

DALI standalone build - a multi-window text editor extracted from the IdeaFix610
framework to run as an independent executable on any Linux system.

## Prerequisites

- Linux (any modern distribution)
- GCC/G++ (version 5 or higher recommended)
- CMake 3.10+
- make

On Debian/Ubuntu:
```bash
sudo apt install build-essential cmake
```

On RHEL/Fedora:
```bash
sudo dnf install gcc gcc-c++ cmake make
```

## Build Instructions

### Step 1: Copy source files from IdeaFix610

```bash
chmod +x setup.sh
./setup.sh /path/to/ideafix610
```

This copies all required source files and headers from the IdeaFix610 tree
into `src/` and `include/` directories.

### Step 2: Build

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

The resulting executable will be at `build/dali`.

### Step 3 (Optional): Static build for maximum portability

```bash
mkdir build-static && cd build-static
cmake -DSTATIC_BUILD=ON ..
make -j$(nproc)
```

### Step 4: Install

```bash
sudo make install    # Installs to /usr/local/bin/dali
```

Or just copy the binary:
```bash
cp build/dali /usr/local/bin/
```

## Terminal Map Files

DALI reads terminal key definitions from `map/$TERM.map` files. You need to
copy the map directory from IdeaFix610:

```bash
cp -r /path/to/ideafix610/etc/map ~/.dali/map
```

Or set the search path via the DALI configuration.

## How It Works

The original IdeaFix610 uses a client-server architecture where:
- A **Window Manager (WM)** process manages the terminal
- **DALI** connects to the WM via System V IPC (message queues + semaphores)

This standalone build uses the WM's **standalone mode** (`wmalone.cc`) which
embeds the window management directly in the DALI process, eliminating the
need for IPC and the separate WM server.

The key change: `WiConnect()` from `wmalone.cc` calls `w_init()` directly
(which calls `WmSetStdAlone()`) instead of opening IPC channels.

## Architecture

```
dali (executable)
  |
  +-- dali_editor (DALI-specific editor logic)
  |     command/, dsensor/, dtext/, dview/, edhelp/, misc/, flock/
  |
  +-- wapp (WinApp - window application framework)
  |     winobj/, window/, views/, wincmd/
  |
  +-- wm (Window Manager - embedded in-process)
  |     lib/ (wmalone.cc, wmsetfun.cc, wmoutput.cc, wmgetc.cc, ...)
  |
  +-- ixwi (WI interface - function wrappers)
  |     wirut.cc (calls ExecWm -> WmStdAlone -> direct functions)
  |
  +-- idea (Foundation library)
        String, List, RegExp, OsFile, types, utilities
```
