cmake_minimum_required(VERSION 3.10)
project(dali-standalone C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)

# ---------------------------------------------------------------------------
# Compiler flags matching IdeaFix610 build
# ---------------------------------------------------------------------------
add_compile_options(
    -fno-for-scope
    -fpermissive
    -w                          # Suppress warnings (legacy code)
    -Wno-write-strings
    -Wno-narrowing
    -fPIC
)

add_compile_definitions(
    linux
    _GNU_SOURCE
    HAVE_BOOL=1
    DALI_STANDALONE=1
)

# ---------------------------------------------------------------------------
# Include paths - mirrors the IdeaFix build: -I. -Iinclude -I$ISRC/include
# ---------------------------------------------------------------------------
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/winapp
    ${CMAKE_SOURCE_DIR}/include/local
    ${CMAKE_SOURCE_DIR}/include/ifound
    ${CMAKE_SOURCE_DIR}/src/dali/include
    ${CMAKE_SOURCE_DIR}/src/dali
    ${CMAKE_SOURCE_DIR}/src/wm
    ${CMAKE_SOURCE_DIR}/src/wm/include
    ${CMAKE_SOURCE_DIR}/src
)

# ---------------------------------------------------------------------------
# Foundation library (libidea equivalent)
# ---------------------------------------------------------------------------
file(GLOB LIBIDEA_ITYPES   src/lib/itypes/*.cc)
file(GLOB LIBIDEA_BASIC_CC src/lib/basic/*.cc)
file(GLOB LIBIDEA_BASIC_C  src/lib/basic/*.c)
file(GLOB LIBIDEA_BASIC_GIF src/lib/basic/gif/*.cc)
file(GLOB LIBIDEA_MISC     src/lib/misc/*.cc)
file(GLOB LIBIDEA_REXP     src/lib/rexp/*.cc)
file(GLOB LIBIDEA_ARRAY    src/lib/array/*.cc)
file(GLOB LIBIDEA_HASH     src/lib/hash/*.cc)
file(GLOB LIBIDEA_OPARSER  src/lib/oparser/*.cc)
file(GLOB LIBIDEA_LIST     src/lib/elib/list/*.cc)
file(GLOB LIBIDEA_ILIST    src/lib/elib/ilist/*.cc)
file(GLOB LIBIDEA_OSFILE   src/lib/elib/osfile/*.cc)
file(GLOB LIBIDEA_MESSAGE  src/lib/message/*.cc)
file(GLOB LIBIDEA_MSGTABLE src/lib/msgtable/*.cc)
file(GLOB LIBIDEA_OUTPUT   src/lib/output/*.cc)
file(GLOB LIBIDEA_EXCEPT   src/lib/except/*.cc)
file(GLOB LIBIDEA_SYSLOG   src/lib/syslog/*.cc)
file(GLOB LIBIDEA_ENUM     src/lib/enum/*.cc)
file(GLOB LIBIDEA_TSTAMP   src/lib/tstamp/*.cc)
file(GLOB LIBIDEA_COMPRESS src/lib/compress/*.cc)
file(GLOB LIBIDEA_EXPR     src/lib/expr/*.cc)
file(GLOB LIBIDEA_CONN     src/lib/conn/*.cc)
file(GLOB LIBIDEA_CRACKER  src/lib/cracker/*.cc)
file(GLOB LIBIDEA_BOBJECT  src/lib/bobject/*.cc)
file(GLOB LIBIDEA_GUICONN  src/lib/guiconn/*.cc)
file(GLOB LIBIDEA_IMAGE    src/lib/image/*.cc)

# Exclude imain.cc and libstamp.cc (they have special compilation needs)
list(FILTER LIBIDEA_MISC EXCLUDE REGEX "imain\\.cc$")
list(FILTER LIBIDEA_MISC EXCLUDE REGEX "libstamp\\.cc$")

add_library(idea STATIC
    ${LIBIDEA_ITYPES}
    ${LIBIDEA_BASIC_CC}
    ${LIBIDEA_BASIC_C}
    ${LIBIDEA_BASIC_GIF}
    ${LIBIDEA_MISC}
    ${LIBIDEA_REXP}
    ${LIBIDEA_ARRAY}
    ${LIBIDEA_HASH}
    ${LIBIDEA_OPARSER}
    ${LIBIDEA_LIST}
    ${LIBIDEA_ILIST}
    ${LIBIDEA_OSFILE}
    ${LIBIDEA_MESSAGE}
    ${LIBIDEA_MSGTABLE}
    ${LIBIDEA_OUTPUT}
    ${LIBIDEA_EXCEPT}
    ${LIBIDEA_SYSLOG}
    ${LIBIDEA_ENUM}
    ${LIBIDEA_TSTAMP}
    ${LIBIDEA_COMPRESS}
    ${LIBIDEA_EXPR}
    ${LIBIDEA_CONN}
    ${LIBIDEA_CRACKER}
    ${LIBIDEA_BOBJECT}
    ${LIBIDEA_GUICONN}
    ${LIBIDEA_IMAGE}
)

# ---------------------------------------------------------------------------
# IXWI library (WI interface)
# In standalone mode we EXCLUDE wirem.cc and wiipc.cc (IPC client code)
# because wmalone.cc provides a standalone WiConnect() that calls w_init()
# directly without IPC.
# ---------------------------------------------------------------------------
file(GLOB IXWI_CC src/ixwi/*.cc)
file(GLOB IXWI_C  src/ixwi/*.c)

# Remove IPC-specific files - wmalone.cc provides standalone versions
list(FILTER IXWI_CC EXCLUDE REGEX "wirem\\.cc$")
list(FILTER IXWI_CC EXCLUDE REGEX "wiipc\\.cc$")

add_library(ixwi STATIC
    ${IXWI_CC}
    ${IXWI_C}
)

# ---------------------------------------------------------------------------
# WM library (Window Manager)
# ---------------------------------------------------------------------------
file(GLOB WM_LIB_SOURCES src/wm/lib/*.cc)

add_library(wm STATIC
    ${WM_LIB_SOURCES}
)

# ---------------------------------------------------------------------------
# WinApp library
# ---------------------------------------------------------------------------
file(GLOB WINAPP_WINOBJ  src/winapp/winobj/*.cc)
file(GLOB WINAPP_WINDOW  src/winapp/window/*.cc)
# windos.cc is DOS-only code, winmgr.cc is the IPC-based WM server launcher,
# wiipc.cc is IPC channel code for the WM server, winmsg.cc is WM IPC messaging.
# None are needed in standalone mode.
# Only keep window.cc from this directory. The rest are WM server / IPC / DOS code.
list(FILTER WINAPP_WINDOW INCLUDE REGEX "window\\.cc$")
file(GLOB WINAPP_VIEWS   src/winapp/views/*.cc)
file(GLOB WINAPP_WINCMD  src/winapp/wincmd/*.cc)
# histcmd.cc requires missing headers (winapp/histcmd.h, winapp/sensor.h)
list(FILTER WINAPP_WINCMD EXCLUDE REGEX "histcmd\\.cc$")
file(GLOB WINAPP_WINAPP  src/winapp/winapp/*.cc)
file(GLOB WINAPP_LOG     src/winapp/log/*.cc)

add_library(wapp STATIC
    ${WINAPP_WINOBJ}
    ${WINAPP_WINDOW}
    ${WINAPP_VIEWS}
    ${WINAPP_WINCMD}
    ${WINAPP_WINAPP}
    ${WINAPP_LOG}
)

# ---------------------------------------------------------------------------
# DALI editor library
# ---------------------------------------------------------------------------
file(GLOB DALI_COMMAND  src/dali/command/*.cc)
file(GLOB DALI_DSENSOR  src/dali/dsensor/*.cc)
file(GLOB DALI_DTEXT    src/dali/dtext/*.cc)
file(GLOB DALI_DVIEW    src/dali/dview/*.cc)
file(GLOB DALI_EDHELP   src/dali/edhelp/*.cc)
file(GLOB DALI_MISC     src/dali/misc/*.cc)
file(GLOB DALI_FLOCK    src/dali/flock/*.cc)

add_library(dali_editor STATIC
    ${DALI_COMMAND}
    ${DALI_DSENSOR}
    ${DALI_DTEXT}
    ${DALI_DVIEW}
    ${DALI_EDHELP}
    ${DALI_MISC}
    ${DALI_FLOCK}
)

# ---------------------------------------------------------------------------
# Main executable
#
# The imain.cc file needs special defines to bootstrap the ISApp subclass.
# In the original build system this is done via -DRootClass='dali'
# ---------------------------------------------------------------------------
add_library(imain_obj OBJECT src/lib/misc/imain.cc)
target_compile_definitions(imain_obj PRIVATE
    RootClass=dali
    COMPILE_USER="standalone"
    COMPILE_HOST="linux"
    iRootClass=<dali.h>
)

add_executable(dali
    src/dali/dali.cc
    $<TARGET_OBJECTS:imain_obj>
)

# Link order matters - most specific to most general
target_link_libraries(dali
    dali_editor
    wapp
    wm
    ixwi
    idea
    m
    crypt
    pthread
)

# Try static linking for portability
option(STATIC_BUILD "Build fully static binary" OFF)
if(STATIC_BUILD)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
    target_link_libraries(dali -static)
endif()

install(TARGETS dali DESTINATION bin)
