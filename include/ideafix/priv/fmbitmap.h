#ifndef FMBITMAP_H
#define FMBITMAP_H               

#ifdef __GNUG__                                                                 
#pragma interface                                                              
#endif
                
class wxBitmap;
typedef wxBitmap FmBitmap;                

FmBitmap *fmLoadBitmap(const String &name);
                
#endif /* FMBITMAP_H */                
