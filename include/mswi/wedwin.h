#ifndef INC_WEDWIN_H
#define INC_WEDWIN_H

#ifndef __cplusplus
#error C++ mode required
#endif

#include <ideafix.h>
#include <mswi/iwwidefs.h>
#include <ideafix/priv/opers.h>
#include <mswi/estruct.h>
#include <mswi/edef.h>

#ifdef private
#undef private
#endif

extern "C"
  {

PWINDOW new_window(PBUFFER bp);
unsigned get_window_text_length( PWINDOW wp );

bool copy_buf( PBUFFER dst, PBUFFER src );
void set_curbp( PBUFFER bp );
//void update_buf( PBUFFER bp );
void set_fname( PBUFFER bp, char * fname );
char *get_fname( PBUFFER bp );

  }

#endif
