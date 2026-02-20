#!/bin/bash
# Wrapper script for DALI standalone editor
# Sets up environment variables so DALI can find its data files

DALI_DATADIR="/usr/share/dali"

# Message files and config
export DATADIR="${DATADIR:-$DALI_DATADIR}"

# Terminal map files (FopenPath searches in PATH)
case ":$PATH:" in
    *":$DALI_DATADIR:"*) ;;
    *) export PATH="$DALI_DATADIR:$PATH" ;;
esac

# Detect language from locale if not set
if [ -z "$LANGUAGE" ]; then
    _locale="${LC_ALL:-${LC_MESSAGES:-${LANG:-en}}}"
    case "$_locale" in
        es*) export LANGUAGE="spanish" ;;
        *)   export LANGUAGE="english" ;;
    esac
fi

exec /usr/lib/dali/dali "$@"
