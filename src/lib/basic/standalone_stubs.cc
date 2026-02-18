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

// --- libwm library stamp ---
class libwm {
public:
	static void foo();
};
void libwm::foo() { }

// --- WM globals ---
bool have_slave_printer = false;

void KillChildren() { }
