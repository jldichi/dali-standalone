/*
 * DALI-STANDALONE: Stub implementations for functions that are part of
 * the full IdeaFix framework but not needed for the standalone editor.
 */

#include <ifound.h>
#include <ifound/str.h>
#include <essentia/sockconn.h>
// Force emission of inline functions from objectid.h
#pragma implementation "objectid.h"
#include <essentia/objectid.h>
#include <ideafix/priv/ixsystem.h>
#include <local/istream.h>
#include <local/dirview.h>

// --- IxSystem permission check ---
// Always return true: standalone editor doesn't need license checks
bool IxSystem::readPermission(const String &)
{
	return true;
}

// --- IxSCA / INA user identification ---
// gnopsys.cc declares these with C++ linkage (no extern "C")
int getIxScaIdUser(char *) { return 0; }
char *getIxScaUserName(Int) { static char user[] = "user"; return user; }

// c_ina.h declares this with implicit C++ linkage
char *InaUserLogin(int) { static char user[] = "user"; return user; }

// --- Environment file loader ---
void setEnvironment(const char *, bool) { }

// --- SocketConnection stubs ---
// Network IPC is not used in standalone mode

SocketConnection::SocketConnection(const String &, bool)
	: status_(NO_SERVER), sockDescr_(-1), convType(NO_CONVERT), fDescr_(-1) { }

SocketConnection::SocketConnection(const String &, const String &, bool)
	: status_(NO_SERVER), sockDescr_(-1), convType(NO_CONVERT), fDescr_(-1) { }

SocketConnection::SocketConnection(SOCKET, ConvType ct)
	: status_(NO_SERVER), sockDescr_(-1), convType(ct), fDescr_(-1) { }

SocketConnection::~SocketConnection() { }

Int SocketConnection::send(Int, const char *) { return ERR; }
Int SocketConnection::receive(Int, char *) { return ERR; }

// --- Stream classes ---
// Base implementations missing from codebase (originally in IdeaFix core lib)

Stream::Stream(short n)
	: size(n), cnt(n), state(0)
{
	base = ptr = new char[n];
	offset = 0;
}

Stream::~Stream()
{
	delete[] base;
}

void Stream::reset()
{
	ptr = base;
	cnt = size;
	state = 0;
	offset = 0;
}

void Stream::clearError()
{
	state = 0;
}

InputStream &InputStream::get(String &, char)
{
	state |= END_OF_INPUT;
	return *this;
}

InputStream &InputStream::unget(char)
{
	return *this;
}

char InputStream::fill()
{
	state |= END_OF_INPUT;
	return 0;
}

InputStream &InputStream::operator>>(Num &)
{
	return *this;
}

OutputStream &OutputStream::operator<<(Int)
{
	return *this;
}

OutputStream &OutputStream::operator<<(const char *s)
{
	if (s) while (*s) { if (--cnt < 0) flush(); *ptr++ = *s++; }
	return *this;
}

OutputStream &OutputStream::operator<<(const void *)
{
	return *this;
}

OutputStream &OutputStream::operator<<(const Num &)
{
	return *this;
}

OutputStream &OutputStream::operator<<(const String &s)
{
	return *this << toCharPtr(s);
}

OutputStream &OutputStream::operator<<(Date)
{
	return *this;
}

OutputStream &OutputStream::operator<<(Time)
{
	return *this;
}

OutputStream &OutputStream::operator<<(OutputStreamFunc f)
{
	return f(*this);
}

void OutputStream::flush() { }
void OutputStream::endMsg() { }
void OutputStream::closeMsg() { }
Int OutputStream::pause() { return 0; }
Int OutputStream::pause(const String &, const String &) { return 0; }

OutputStream &flush(OutputStream &s) { s.flush(); return s; }
OutputStream &endl(OutputStream &s) { s << '\n'; s.flush(); return s; }
OutputStream &endMsg(OutputStream &s) { s.endMsg(); return s; }

// --- DirView stubs ---
DirView::DirView() { }
DirView::~DirView() { }

// --- libwm library stamp ---
class libwm {
public:
	static void foo();
};
void libwm::foo() { }

// --- WM globals ---
bool have_slave_printer = false;

void KillChildren() { }
