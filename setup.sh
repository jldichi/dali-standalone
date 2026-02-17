#!/bin/bash
#
# DALI Standalone Editor - Setup Script
# Copies required source files from IdeaFix610 and prepares the build
#
# Usage: ./setup.sh /path/to/ideafix610
#

set -e

if [ -z "$1" ]; then
    echo "Usage: $0 /path/to/ideafix610"
    echo "  Copies required source files from IdeaFix610 to build DALI standalone"
    exit 1
fi

ISRC="$1"
DEST="$(cd "$(dirname "$0")" && pwd)"

if [ ! -d "$ISRC/dali/editor" ]; then
    echo "ERROR: $ISRC does not appear to be an IdeaFix610 source directory"
    echo "  (missing dali/editor/)"
    exit 1
fi

echo "=== DALI Standalone Setup ==="
echo "Source: $ISRC"
echo "Destination: $DEST"
echo ""

# ---------------------------------------------------------------------------
# Create directory structure
# ---------------------------------------------------------------------------
echo "[1/7] Creating directory structure..."

mkdir -p "$DEST/src/dali/command"
mkdir -p "$DEST/src/dali/dsensor"
mkdir -p "$DEST/src/dali/dtext"
mkdir -p "$DEST/src/dali/dview"
mkdir -p "$DEST/src/dali/edhelp"
mkdir -p "$DEST/src/dali/misc"
mkdir -p "$DEST/src/dali/flock"
mkdir -p "$DEST/src/winapp/winobj"
mkdir -p "$DEST/src/wm/lib"
mkdir -p "$DEST/src/ixwi"
mkdir -p "$DEST/src/lib/itypes"
mkdir -p "$DEST/src/lib/basic"
mkdir -p "$DEST/src/lib/basic/gif"
mkdir -p "$DEST/src/lib/misc"
mkdir -p "$DEST/src/lib/rexp"
mkdir -p "$DEST/src/lib/array"
mkdir -p "$DEST/src/lib/hash"
mkdir -p "$DEST/src/lib/oparser"
mkdir -p "$DEST/src/lib/elib/list"
mkdir -p "$DEST/src/lib/elib/ilist"
mkdir -p "$DEST/src/lib/elib/osfile"
mkdir -p "$DEST/src/lib/message"
mkdir -p "$DEST/src/lib/msgtable"
mkdir -p "$DEST/src/lib/output"
mkdir -p "$DEST/src/lib/except"
mkdir -p "$DEST/src/lib/syslog"
mkdir -p "$DEST/src/lib/enum"
mkdir -p "$DEST/src/lib/tstamp"
mkdir -p "$DEST/src/lib/compress"
mkdir -p "$DEST/src/lib/expr"
mkdir -p "$DEST/src/lib/conn"
mkdir -p "$DEST/src/lib/cracker"
mkdir -p "$DEST/src/lib/bobject"
mkdir -p "$DEST/src/lib/guiconn"
mkdir -p "$DEST/src/lib/image"

# Copy entire include directory tree
echo "[2/7] Copying headers..."
cp -a "$ISRC/include" "$DEST/"

# Copy DALI-specific headers
cp -a "$ISRC/dali/editor/include" "$DEST/src/dali/"

# ---------------------------------------------------------------------------
# Copy DALI editor sources
# ---------------------------------------------------------------------------
echo "[3/7] Copying DALI editor sources..."

cp "$ISRC/dali/editor/dali.cc" "$DEST/src/dali/"

for f in bascomm.cc blkcomm.cc command.cc commdef.cc debbox.cc debcomm.cc \
         drawcomm.cc edfsel.cc edlbox.cc filecomm.cc grepcomm.cc grpmcomm.cc \
         history.cc oscmdbox.cc poscomm.cc schcomm.cc schmcomm.cc syscomm.cc; do
    cp "$ISRC/dali/editor/command/$f" "$DEST/src/dali/command/"
done

for f in dsenbind.cc dsencmd.cc dsensor.cc; do
    cp "$ISRC/dali/editor/dsensor/$f" "$DEST/src/dali/dsensor/"
done

for f in datroff.cc datex.cc dattrib.cc dtext.cc dtextcur.cc; do
    cp "$ISRC/dali/editor/dtext/$f" "$DEST/src/dali/dtext/"
done

for f in dview.cc dviewmod.cc dviewpos.cc; do
    cp "$ISRC/dali/editor/dview/$f" "$DEST/src/dali/dview/"
done

for f in edhelp.cc help.cc; do
    cp "$ISRC/dali/editor/edhelp/$f" "$DEST/src/dali/edhelp/"
done

for f in dalirc.cc debugger.cc editor.cc; do
    cp "$ISRC/dali/editor/misc/$f" "$DEST/src/dali/misc/"
done

cp "$ISRC/dali/editor/flock/flock.cc" "$DEST/src/dali/flock/"

# Copy DALI preference file if it exists
[ -d "$ISRC/dali/editor/pref" ] && cp -a "$ISRC/dali/editor/pref" "$DEST/src/dali/" 2>/dev/null || true

# ---------------------------------------------------------------------------
# Copy WinApp sources
# ---------------------------------------------------------------------------
echo "[4/7] Copying WinApp sources..."

for f in menu.cc scrmenu.cc strhis.cc button.cc cell.cc dbox.cc dirsel.cc \
         filesel.cc label.cc mldbox.cc msgbox.cc panel.cc infopan.cc \
         slide.cc strmenu.cc textview.cc waitmsg.cc winobj.cc; do
    cp "$ISRC/winapp/winobj/$f" "$DEST/src/winapp/winobj/"
done

# Also copy the window core from winapp if DALI needs it
if [ -d "$ISRC/winapp/window" ]; then
    mkdir -p "$DEST/src/winapp/window"
    cp "$ISRC/winapp/window/"*.cc "$DEST/src/winapp/window/" 2>/dev/null || true
fi

if [ -d "$ISRC/winapp/views" ]; then
    mkdir -p "$DEST/src/winapp/views"
    cp "$ISRC/winapp/views/"*.cc "$DEST/src/winapp/views/" 2>/dev/null || true
fi

if [ -d "$ISRC/winapp/wincmd" ]; then
    mkdir -p "$DEST/src/winapp/wincmd"
    cp "$ISRC/winapp/wincmd/"*.cc "$DEST/src/winapp/wincmd/" 2>/dev/null || true
fi

if [ -d "$ISRC/winapp/winapp" ]; then
    mkdir -p "$DEST/src/winapp/winapp"
    cp "$ISRC/winapp/winapp/"*.cc "$DEST/src/winapp/winapp/" 2>/dev/null || true
fi

if [ -d "$ISRC/winapp/log" ]; then
    mkdir -p "$DEST/src/winapp/log"
    cp "$ISRC/winapp/log/"*.cc "$DEST/src/winapp/log/" 2>/dev/null || true
fi

# ---------------------------------------------------------------------------
# Copy WM library sources
# ---------------------------------------------------------------------------
echo "[5/7] Copying Window Manager sources..."

for f in wmalone.cc wmconf.cc wmgetc.cc wmgetpar.cc wmgets.cc wminit.cc \
         wmlbl.cc wmmov.cc wmoutput.cc wmprtscr.cc wmprtset.cc wmsetfun.cc \
         wmsig.cc wmtcap.cc wmvs.cc wmstty.cc wmwin.cc wmaplhlp.cc \
         wmcalc.cc wmcalen.cc wmget.cc wmicalf.cc; do
    cp "$ISRC/wm/lib/$f" "$DEST/src/wm/lib/" 2>/dev/null || true
done

# Copy WM headers
if [ -d "$ISRC/wm" ]; then
    cp "$ISRC/wm/"*.h "$DEST/src/wm/" 2>/dev/null || true
fi
if [ -d "$ISRC/wm/include" ]; then
    mkdir -p "$DEST/src/wm/include"
    cp -a "$ISRC/wm/include/"* "$DEST/src/wm/include/" 2>/dev/null || true
fi

# ---------------------------------------------------------------------------
# Copy IXWI sources
# ---------------------------------------------------------------------------
echo "[6/7] Copying IXWI interface sources..."

for f in wicod.cc wiexmenu.cc wiexsys.cc wiexwcmd.cc wihelp.cc wiipc.cc \
         wimessge.cc wimsg.cc wipopup.cc wiprintf.cc wirem.cc wirut.cc \
         wisrch.cc widefpar.cc wigpas.cc; do
    cp "$ISRC/lib/ixwi/$f" "$DEST/src/ixwi/" 2>/dev/null || true
done

for f in wiglob.c widialog.c wiexpipe.c wiexsh.c wihelpf.c wioutput.c wicmdtab.c; do
    cp "$ISRC/lib/ixwi/$f" "$DEST/src/ixwi/" 2>/dev/null || true
done

# ---------------------------------------------------------------------------
# Copy Foundation library sources (libidea)
# ---------------------------------------------------------------------------
echo "[7/7] Copying Foundation library sources..."

# String, types
for f in boolean.cc date.cc datemisc.cc datenum.cc datestr.cc \
         num.cc numadd.cc numcmp.cc numcon.cc numconv.cc numdbl.cc \
         numdebug.cc numdiv.cc numfmt.cc numld.cc numldbuf.cc nummisc.cc \
         nummult.cc numround.cc numshft.cc numshift.cc numst.cc \
         numstbuf.cc numstr.cc numtoi.cc \
         strbase.cc strcapit.cc strcase.cc strchr.cc strcursr.cc strfmt.cc \
         strfname.cc strgenv.cc strhash.cc string.cc strldbuf.cc strnull.cc \
         strstbuf.cc strindex.cc substr.cc \
         time.cc timemisc.cc timenum.cc tmdtconn.cc tmdtstld.cc types.cc; do
    cp "$ISRC/lib/itypes/$f" "$DEST/src/lib/itypes/" 2>/dev/null || true
done

# Basic utilities
for f in critsect.cc stglob.c stchset.cc stnumtxt.c stprintf.cc stdecode.c \
         stdlen.c ststrto.c stctype.c strmatch.c ststrtxt.c ststrstr.c \
         sttrim.c stmask.cc sttmask.c stconf.c tmglob.c tmweek.c tmfday.c \
         tmmdiff.c tmconv.cc tmgdate.cc tmholli.c c_error.cc c_cmask.cc \
         c_crypt.cc c_locate.cc c_bkgrnd.cc c_start.cc c_basenm.cc \
         c_errmsg.cc c_stop.cc c_onstop.cc c_fopenp.cc c_interr.cc c_msg.cc \
         gnalloc.cc gnrenv.cc gntcheck.c gncheck.c gnbanner.c gnconv.c \
         gnprint.c gnopsys.cc gntyconv.c gnterm.cc gnrange.c stop.cc \
         ulicense.cc gnshidea.cc; do
    cp "$ISRC/lib/basic/$f" "$DEST/src/lib/basic/" 2>/dev/null || true
done

# Basic/gif subfolder
for f in c_ixscaina.cc c_ina.cc; do
    cp "$ISRC/lib/basic/gif/$f" "$DEST/src/lib/basic/gif/" 2>/dev/null || true
done

# Misc
for f in iddebug.cc umain.cc isapp.cc no_imain.cc ftrace.cc assert.cc \
         backgr.cc banner.cc basenm.cc bufffile.cc cmask.cc config.cc \
         _config.cc crypt.cc filelock.cc finpath.cc doformat.cc gpenv.cc \
         ifxctype.cc interr.cc intop.cc iopipes.cc locate.cc mask.cc \
         memcmpi.cc misc.cc opsys.cc ostime.cc stat.cc rename.cc stld.cc \
         ftstamp.cc strstr.cc swapint.cc terminal.cc tempnam.cc tocharptr.cc \
         toint.cc toulcase.cc unlink.cc resource.cc libinfo.cc infdummy.cc \
         wif.cc imain.cc libstamp.cc; do
    cp "$ISRC/lib/misc/$f" "$DEST/src/lib/misc/" 2>/dev/null || true
done

# RegExp
cp "$ISRC/lib/rexp/rexp.cc" "$DEST/src/lib/rexp/"

# Arrays
for f in parray.cc barray.cc; do
    cp "$ISRC/lib/array/$f" "$DEST/src/lib/array/" 2>/dev/null || true
done

# Hash
for f in hash.cc hashcsr.cc phash.cc; do
    cp "$ISRC/lib/hash/$f" "$DEST/src/lib/hash/" 2>/dev/null || true
done

# Option parser
for f in oparser.cc opstart.cc wxcmdlin.cc; do
    cp "$ISRC/lib/oparser/$f" "$DEST/src/lib/oparser/" 2>/dev/null || true
done

# elib (list, osfile)
for f in list.cc lcursor.cc; do
    cp "$ISRC/lib/elib/list/$f" "$DEST/src/lib/elib/list/" 2>/dev/null || true
done
cp "$ISRC/lib/elib/ilist/ilist.cc" "$DEST/src/lib/elib/ilist/" 2>/dev/null || true

for f in osfile.cc osferror.cc osflink.cc osflock.cc osfstat.cc phyfile.cc physlout.cc; do
    cp "$ISRC/lib/elib/osfile/$f" "$DEST/src/lib/elib/osfile/" 2>/dev/null || true
done

# Message / msgtable
cp "$ISRC/lib/message/message.cc" "$DEST/src/lib/message/" 2>/dev/null || true
cp "$ISRC/lib/msgtable/msgtable.cc" "$DEST/src/lib/msgtable/" 2>/dev/null || true

# Output
for f in termout.cc strout.cc rportout.cc delimout.cc rmtout.cc c_output.cc \
         output.cc gridout.cc soutput.cc c_graout.cc graphout.cc graphfun.cc; do
    cp "$ISRC/lib/output/$f" "$DEST/src/lib/output/" 2>/dev/null || true
done

# Exception
cp "$ISRC/lib/except/except.cc" "$DEST/src/lib/except/" 2>/dev/null || true

# Syslog
cp "$ISRC/lib/syslog/slog.cc" "$DEST/src/lib/syslog/" 2>/dev/null || true

# Enum
cp "$ISRC/lib/enum/_enumobj.cc" "$DEST/src/lib/enum/" 2>/dev/null || true

# Timestamp
for f in tstamp.cc tsconn.cc; do
    cp "$ISRC/lib/tstamp/$f" "$DEST/src/lib/tstamp/" 2>/dev/null || true
done

# Compress
for f in _expand.cc _reduce.cc xstream.cc; do
    cp "$ISRC/lib/compress/$f" "$DEST/src/lib/compress/" 2>/dev/null || true
done

# Expr
for f in anyconn.cc anyldbuf.cc anyoper.cc anystbuf.cc anyvalue.cc anynull.cc \
         expldbuf.cc exprldf.cc exprstf.cc expr.cc exprbld.cc exprconn.cc \
         exprut.cc expstbuf.cc incheck.cc inconn.cc; do
    cp "$ISRC/lib/expr/$f" "$DEST/src/lib/expr/" 2>/dev/null || true
done

# Connection
for f in connect.cc netconn.cc compconn.cc; do
    cp "$ISRC/lib/conn/$f" "$DEST/src/lib/conn/" 2>/dev/null || true
done

# Cracker
for f in pserconn.cc cropsys.cc; do
    cp "$ISRC/lib/cracker/$f" "$DEST/src/lib/cracker/" 2>/dev/null || true
done

# Binary objects
for f in bobjcli.cc bobjid.cc binobjs.cc; do
    cp "$ISRC/lib/bobject/$f" "$DEST/src/lib/bobject/" 2>/dev/null || true
done

# GUI connection
cp "$ISRC/lib/guiconn/guiconn.cc" "$DEST/src/lib/guiconn/" 2>/dev/null || true

# Image
cp "$ISRC/lib/image/shimage.cc" "$DEST/src/lib/image/" 2>/dev/null || true

echo ""
echo "=== Setup complete ==="
echo ""
echo "Source files copied to: $DEST/src/"
echo "Include files copied to: $DEST/include/"
echo ""
echo "To build:"
echo "  cd $DEST"
echo "  mkdir build && cd build"
echo "  cmake .."
echo "  make -j\$(nproc)"
echo ""
echo "The resulting binary will be: build/dali"
