# Technical Notes - DALI Standalone Build

## Key Design Decisions

### 1. Standalone Mode via wmalone.cc

The WM library already includes a standalone mode (`wm/lib/wmalone.cc`) that
provides `WiConnect()` without IPC. The function pointer `ExecWm` is set to
`WmStdAlone` (in `wmsetfun.cc`) which dispatches all WI commands as direct
function calls instead of sending them over message queues.

**Files excluded from build (IPC-only):**
- `lib/ixwi/wirem.cc` - Contains `WmRemote()` and IPC version of `WiConnect()`
- `lib/ixwi/wiipc.cc` - Contains `WiOpenChannel()` and `WiOpenSem()`
- `winapp/window/wicod.cc` - WM server param encoding (uses winmgr.h)
- `winapp/window/wiipc.cc` - WM server IPC channels
- `winapp/window/winmgr.cc` - WM server launcher
- `winapp/window/winmsg.cc` - WM IPC messaging
- `winapp/window/windos.cc` - DOS-only code
- `winapp/wincmd/histcmd.cc` - Requires missing headers

### 2. imain.cc Bootstrap

The IdeaFix build system compiles `imain.cc` with special defines:
- `-DRootClass=dali` - The ISApp subclass name
- `-DiRootClass=<dali.h>` - The header to include
- `-DCOMPILE_USER` and `-DCOMPILE_HOST` - Build metadata

The CMakeLists.txt reproduces this with a separate OBJECT library.

### 3. No ncurses Dependency

The WM implements its own terminal I/O layer using:
- Raw terminal I/O via termios (`wmstty.cc`)
- Termcap/terminfo for terminal capabilities (`wmtcap.cc`)
- Virtual screen buffer (`wmvs.cc`)
- Direct ANSI escape sequence output (`wmoutput.cc`)

## Patches Applied

### osdep.h - GCC 9+ Support

Changed the GCC version detection from exact match (`__GNUC__ == 8`) to range
(`__GNUC__ >= 8`) so GCC 9, 10, 11, 12, 13, 14+ all use `linuxgcc8.h`.

### config.h - Stub Header

Created a minimal `include/config.h` stub. In the original IdeaFix build,
this was likely generated. WinApp's views and filecmd use it.

### Include Paths

Added additional include paths that the original build used implicitly:
- `include/winapp/` - WinApp source files use bare `<window.h>`, `<cell.h>`
- `include/local/` - For `<osfile.h>`, `<strfile.h>`
- `include/ifound/` - For `<main.h>`, `<stdc.h>`

## Compilation Simulation Results

### Verified OK:
- All critical framework functions (Start, Stop, Alloc, Free, ReadEnv,
  SetMessager, OnStop, etc.) have definitions in the source tree
- `crackerClient()` is defined in `lib/misc/config.cc`
- No duplicate symbol definitions between wmalone.cc and other files
- All headers referenced by source files exist (after exclusions)
- Transitive header includes from `.h` files are either present or
  protected by `#ifdef` for unused subsystems (wxWidgets, Xerces, ODBC)

### Potential Issues at Compile Time:
- Legacy C++ patterns may need `-fpermissive` (already set)
- `-fno-for-scope` needed for old for-loop variable scoping
- Some implicit conversions between `char*` and `const char*`
- `#pragma interface` in some headers (GCC-specific, harmless)

### Missing at Runtime:
- Terminal map files (`map/$TERM.map`) - copy from IdeaFix610
- Message files (`.msg`) - for localized error messages

## File Counts (After Exclusions)

| Component           | Files | Notes                              |
|--------------------:|------:|:-----------------------------------|
| DALI editor         |    36 | command, dsensor, dtext, dview, etc|
| WinApp winobj       |    18 | menus, dialogs, panels             |
| WinApp window       |     1 | window.cc only                     |
| WinApp views        |    13 | text, view, cursor classes         |
| WinApp wincmd       |    11 | block, edit, file, pos commands    |
| WinApp winapp       |     3 | desktop, winapp, filecmd           |
| WinApp log          |     1 | undo/redo log                      |
| WM library          |    22 | standalone mode, terminal I/O      |
| IXWI interface      |    20 | WI function wrappers               |
| libidea foundation  |   208 | String, List, RegExp, types, etc   |
| **Total**           | **333**|                                   |

## Future Improvements

1. **Remove unused libidea components**: DALI doesn't need DBM, forms,
   reports, dbconn, ixsystem, ix3cpp. Removing these would reduce the source
   base by ~50%.

2. **Replace custom types with C++ standard**: `String` -> `std::string`,
   `List` -> `std::vector/std::list` (major refactor).

3. **Add ncurses support**: Replace the raw terminal I/O with ncurses for
   better terminal compatibility.

4. **Unicode/UTF-8 support**: The current code is ASCII/ISO-8859-1 based.
