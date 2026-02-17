#ifndef __WINPRINT_H
#define __WINPRINT_H 1


#define MAXLINES             (200)
#define MAXCHARS             (500)
#define END_OF_TEXT           (-2)
#define TABSIZE                 4

#define MAX_FONTS            (50)

#define SETSIZE                 1
#define SETATTR                 2
#define SETORIENT               3

#define SETPORTRAIT             1
#define SETLANDSCAPE            2

#define SETBOLD                 1
#define SETSTANDOUT             2
#define SETUNDERLINE            4
#define SETITALIC               5
#define SETNLQ                  6
#define SETREVERSE              7

#ifdef __cplusplus
extern "C" {
#endif

int winpm(char *);

#ifdef __cplusplus
}
#endif

typedef struct tagFont {
	int size;
	int bold;
	HFONT hFont;
	TEXTMETRIC	tm;
}Font;

typedef struct tagGraphChar{
	UChar gchar;
	int pos;
	struct tagGraphChar * prev;
}GraphChar;

typedef struct tagFrase {
	UChar *data;
        int font;
        int vPixel;     // begin from this vertical Pixel
        int hPixel;     // begin from this horizontal Pixel
	int len;
	int begin;	// Position of Begin of Frase
	struct tagFrase *next;
	GraphChar * graph;
}Frase;

typedef struct tagFontStock {
        HFONT        hFont;
        LOGFONT      lf;
        TEXTMETRIC   tm;
}FontStock;

class WinPrint{
	friend class TextPage;

        char *file;
        HDC  pDC;
        char szPrinter[80];
        char *szDevice;
        char *szDriver;
        char *szOutput;
        FILE *fp;
        LOGFONT lf;
        Font dev_font;
        int numPage;                    // Page number
        short xPage;                    // width of page in pixels = tmHeight+tmExternalLeading
        short yPage;                    // height of page in pixels = tmHeight+tmExternalLeading
        int fLength;                    // Number of lines per page for compare with Ix FLENGTH
        FontStock fStock[MAX_FONTS];
        int curF;                       // Current font slot
        HFONT hOriginalFont;
        BYTE defPitchAndFamily;
        HANDLE hDriver;
        LPFNDEVMODE ExtDeviceMode;
        char szDriverFile[16];
        DEVMODE dmIn;
        char dumb[200];
        DEVMODE dmOut;
        char dumber[200];

// Private
        int initFonts();                        // Gets the font to use
        void getDefaultPrinterInfo();           // Gets the default printer for windows
        void getDriverFunction();               // Gets the address of ExtDeviceMode()
        int driverGet();                        // Gets the printer driver data

        int changeFont(int, int);               // Changes the current font
        void _setSize(int);                     // Modifies the size of the font
        void _setAttr(int);                     // Sets or resets attribs in lf
        int fontExists();                       // Returns the font slot if it exists (-1 otherwise)
        int _addFont();                         // Adds a new font to FontStock
        void _setCurrFont(int);                 // Sets one of the fonts in FontStock as current
        void _checkOrientation();               // Looks for the orientation seq. in the first 200 chars of the file
        int _setOrientation(int);               // Changes printing orientation

public:
	WinPrint(char *);
	~WinPrint();
	int print();
	int open();
};

class TextPage
{
	int lastpage;
	int nLines; 			// Counter of Lines
	int nPixels;			// Counter of Pixels
	int pos;
	int lHeight;
	WinPrint  *wp;
	Frase * firstFrase;
	Frase * currentFrase;
	Frase * prevFrase;
	UChar buffer[MAXCHARS + 1];
	BOOL decode();
	void newfrase(BOOL);
	void printGChar(UChar,int,int,const TEXTMETRIC *);
	int isGraph(UChar);

public:
	TextPage(const WinPrint *);
	~TextPage();
	int read();
	int print();
};
#endif
