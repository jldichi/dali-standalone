#ifndef __MEMDEBUG_H__
#define __MEMDEBUG_H__ 

#include <stdio.h>
#include "singleton.hh"

/***
* @DIEGOS
*
* Esta variable indica cuantos '_new' sin '_delete' existen en memoria
* Cuando el programa finalice tendria que mostrar:
*   $ _delete1 | 0
* 
* lo cual indica que hay 0 '_new' sin su '_delete'
*
* Esto lo hice solo a los efectos de debug.  Para detectar goteo de 
* memoria (Memory Leak)
*
**/
class MemDebug {
  public:
    long M ;         // Cuantos _new sin _delete se han echo

    MemDebug() : M(0) {}
    ~MemDebug() {} 
    void inc( long i = 1 ) { M += i ; }
    void dec( long i = 1 ) { M -= i ; }

    long get() { return M ; }
} ;

typedef Singleton<MemDebug> MEMDEBUG;

/***
* @DIEGOS
*
* Esta funcion equivale al 'new <type/class>'
*
**/
template <class T> T* _new() {
  MEMDEBUG::Instance()->inc() ;
  // printf("_new1 | %ld\n", MEMDEBUG::Instance()->get() ) ;
  return new T ;
} 

/***
* @DIEGOS
*
* Esta funcion equivale al 'new <type/class>[n]'
*
**/
template <class T, class I> T* _new( I cant ) {
  MEMDEBUG::Instance()->inc(cant) ;
  // printf("_newn | %ld\n", MEMDEBUG::Instance()->get() ) ;
  return new T[cant] ;
} 

/***
* @DIEGOS
*
* Esta funcion equivale al 'delete <type/class>'
*
**/
template <class T> void _delete( T* p ) {
  if ( p == 0 ) 
    return ;
    
  MEMDEBUG::Instance()->dec() ;
  // printf("_delete1 | %ld\n", MEMDEBUG::Instance()->get() ) ;
  delete p ;
  p = 0 ;
} 

/***
* @DIEGOS
*
* Esta funcion equivale al 'delete[] <type/class>'
* Le pasamos el parametro 'int' para saber cuantos elementos
* quitar y asi poder decrementar como corresponde la variable
* __TOTAL_ALLOC__ de debug.
*
**/
template <class T> void _delete( T* p, int i ) {
  if ( p == 0 ) 
    return ;
    
  MEMDEBUG::Instance()->dec(i) ;
  // printf("_deleten | %ld\n", MEMDEBUG::Instance()->get() ) ;
  delete[] p ;
} 


#endif

