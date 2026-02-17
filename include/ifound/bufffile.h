#ifndef BUFFFILE_H
#define BUFFFILE_H

#include <errno.h>
#include <ifound/stdc.h>
#include <ifound/macros.h>
#include <stdio.h>
#ifdef __NT__
#	include <io.h>
#endif
#include <fcntl.h>

#ifndef __NT__
	#include <unistd.h>
#endif

#include <string.h>

#define BUFFER_SIZE	4096
#define READ_MODE	0x1
#define WRITE_MODE	0x2
#define NOT_OPENED_FILE	-1

class BufferedFile {
	int fildes;
	char name[512];
	char buff[BUFFER_SIZE];
	int buffsize;
	int ptr;
	int mod;
	int dirty;

	bool isFile();
	void cleanBuffer();

public:
	BufferedFile(const char* file, int mode);
	~BufferedFile();

	// Open file.
	int open();
	// Close file.
	int close();

	// Write a line without new line at the end.
	int write(const char* s);
	// Write a line with new line at the end.
	int writeLine(const char* s);

	// Read "l" bytes into buffer.
	char* read(char* s, int l);
	// Read a line into "s buffer.
	char* readLine(char* s);
	char* readLine(char* s, int len);

	// Flush the buffer to disk.
	void flush();

	void setName(const char* n) { strcpy(name, n); };
	void setMode(int m) { mod = m; };

	const char* fileName() { return name; };
	int fileNo() { return fildes; };
};

#endif // BUFFFILE_H
