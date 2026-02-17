#include <stdlib.h>
#include <ifound.h>
#include <ifound/bufffile.h>
#include <ifound/str.h>
#include <sys/stat.h>

#ifdef HAVE_UNDERSCORE_NAMES
#	define	stat _stat
#endif

void BufferedFile::cleanBuffer()
{
	for (int i=0; i < buffsize; i++)
		buff[i] = '\0';
}

bool BufferedFile::isFile()
{
	struct stat st;
	if (stat(name, &st) < 0) {
		return false;
	}
	return (st.st_mode & S_IFMT) == S_IFREG;
}

BufferedFile::BufferedFile(const char* file, int mode)
: fildes(NOT_OPENED_FILE), buffsize(BUFFER_SIZE), ptr(0), mod(mode), dirty(false)
{
	strcpy(name, file);
	if (mod != READ_MODE && mod != WRITE_MODE) {
		warning("This kind of file can be use: READ_MODE or WRITE_MODE as mode argument.");
		_global_exit(-1);
	}
	cleanBuffer();
}

BufferedFile::~BufferedFile()
{
	if (fildes != NOT_OPENED_FILE) {
		warning("File '%s' should have been closed with 'BufferedFile::close' function.");
		_global_close(fildes);
	}
}

// Open file routine. It returns the file descriptor returned by the OS
// file routine "open". errno is set by this OS funtion.
int BufferedFile::open()
{
	errno = 0;
	fildes = _global_open(name,  mod == READ_MODE ? O_RDONLY : O_CREAT|O_WRONLY|O_TRUNC, 0666);
	// Check it is really a file (a regular file).
	if (fildes > 0 && !isFile()) {
		error(String(name) + " " + libMsg("NOTREGFILE"));	
	}
	return fildes;
}

// Close file. This method calls OS close funtion. If the buffer hasn't been
// flushed, flush it NOW !!!
int BufferedFile::close()
{
	// Check the file has been opened. Previous open should have failed.
	if (fildes == NOT_OPENED_FILE) {
		warning("Trying to close a not opened file.");
		_global_exit(-1);
	}

	// If the buffer hasn't been flush. Do it !!!
	if (ptr > 0)
		flush();

	// Close the file.
	if (_global_close(fildes) < 0) {
		warning("Could not close file '%s'\n", name);
		_global_exit(-1);
	}
	fildes = NOT_OPENED_FILE;
	return 0;
}

// Write everything pointed by "s". It fills the buffer and flush it every
// time it is full.
int BufferedFile::write(const char* s)
{
	int l = strlen(s);
	int i = 0;

	assert(mod == WRITE_MODE);
	assert(fildes >= 0);
  	while (i < l) {
		dirty = true;
		// We loop until the buffer is full.
		while (i < l && ptr < buffsize) {
			buff[ptr] = s[i];
			ptr++;
			i++;
		}
		// If we reached the end of the buffer => FLUSH IT !!!
		if (ptr == buffsize)
			flush();
	}
	return i;
}

// Write a line with new line at the end.
int BufferedFile::writeLine(const char* s)
{
	assert(mod == WRITE_MODE);
	assert(fildes >= 0);
	int res = write(s);
	// I have to insert a new line at the end of the string.
	write("\n");
	return (res+1);

}


// Read "l" bytes into buffer.
char* BufferedFile::read(char* s, int l)
{
	assert(mod == READ_MODE);
	assert(fildes >= 0);
	int r = _global_read(fildes, s, l);
	assert(r > 0 && r <= l);
	return s;
}

// Read a line into "l" buffer up to len bytes or '\n' (new line).
char* BufferedFile::readLine(char* s, int len)
{
	assert(mod == READ_MODE);
	assert(fildes >= 0);
	return 0;
}

// Read a line. Characters until a new line '\n'.
char* BufferedFile::readLine(char* l)
{
	static int lastpos = 0;
	int i = 0;
	l[0] = '\0';

	assert(mod == READ_MODE);
	assert(fildes >= 0);

	// We have to fill in the buffer if ptr == 0;
	if (ptr == 0) {
		lastpos = _global_read(fildes, buff, buffsize);
		assert (lastpos >= 0 && lastpos <= buffsize);
	}

	if (lastpos == 0 || (ptr == lastpos && lastpos < buffsize))
		return NULL;

	assert (ptr >=0 && ptr < buffsize);
	while (buff[ptr] != '\n') {
		while (ptr < lastpos && buff[ptr] != '\n') {
			l[i] = buff[ptr];
			i++;
			ptr++;
		}

		// I've found the new line mark.
		if (buff[ptr] == '\n') {
			// I have to add the end of string.
			l[i] = '\0';
			break;
		}

		// If I haven't found the new line I have to fill in the buffer.
		if (ptr == buffsize) {
			lastpos = _global_read(fildes, buff, buffsize);
			assert (lastpos >= 0 && lastpos <= buffsize);
			ptr = 0;
		}
	}
	// Skip the new line and return the line.
	ptr = (ptr+1) == buffsize ? 0 : ptr+1;
	return l;
}

// Flush the buffer to disk.
void BufferedFile::flush()
{
	int res = 0;
	// Flush the buffer to file only if file was modified.
	if (dirty)
		res = _global_write(fildes, buff, ptr);
	// Set read/write pointer to the beginning of the buffer.
	dirty = false;
	ptr = 0;
}
