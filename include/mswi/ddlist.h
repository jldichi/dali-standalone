/*
 * DDLIST.H
 *
 * Menu identifiers, control identifiers, global variables, and function
 * prototypes for DDLIST.
 * $Id: 
 * Copyright (c)1992 Kraig Brockschmidt, All Right Reserved
 * Internet  :  kraigb@microsoft.com
 * Compuserve:  70750,2344
 */


#if !defined  (WIN32) || defined(__VC__)
//Private drive types not covered in Win3.x GetDriveType but in Win32
#define DRIVE_CDROM                 5
#define DRIVE_RAM                   6

//We also need this export from Kernel, normally in windows.inc
void PASCAL DOS3Call(void);
#endif




//Resource identifiers
#define IDD_DDLIST                  21

//Resource bitmaps:  uses GetDriveType IDs for the drive type bitmaps.
#define IDB_DRIVEMIN                DRIVE_REMOVABLE
#define IDB_DRIVEFLOPPY             DRIVE_REMOVABLE
#define IDB_DRIVEHARD               DRIVE_FIXED
#define IDB_DRIVENETWORK            DRIVE_REMOTE
#define IDB_DRIVECDROM              DRIVE_CDROM
#define IDB_DRIVERAM                DRIVE_RAM
#define IDB_DRIVEMAX                DRIVE_RAM

#define IDB_FOLDERMIN               10
#define IDB_FOLDERCLOSED            10
#define IDB_FOLDEROPEN              11
#define IDB_FOLDEROPENSELECT        12
#define IDB_FOLDERMAX               12


//Control identifiers
#define ID_NULL                     -1
#define ID_DRIVELIST                100
#define ID_DIRECTORYLIST            101
#define ID_TEMPLIST                 102


//Function prototypes

//DDLIST.C
static BOOL CALLBACK DDListDialogProc(HWND, UINT, UINT, LONG);
extern "C" void WINAPI RunDDListDlg(HWND);


void                 DriveListInitialize(HWND, HWND);
UINT                 DriveType(UINT);
void                 DirectoryListInitialize(HWND, HWND, LPSTR);
void                 DriveDirDrawItem(LPDRAWITEMSTRUCT, BOOL);
void                 TransparentBlt(HDC, UINT, UINT, HBITMAP, COLORREF);
void 				FreeDriveMaps(void);

//Special ROP code for TransparentBlt.
#define ROP_DSPDxax  0x00E20746
