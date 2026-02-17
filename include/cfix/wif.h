#ifndef _WIF_H
#define _WIF_H

#define EMPTY_FILE		"/dev/null"
#define LOCALPRINTER	"localprinter"
#define DEFAULT_PORT	"LPT1"

#ifdef __cplusplus
  extern "C"
#else
  extern 
#endif
void SetWIFProperty(char *name, char *value);

#endif
