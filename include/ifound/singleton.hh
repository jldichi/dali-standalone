/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* singleton.h,v 1.1.1.1 2005/10/13 13:46:22 diegos Exp
*
* Fin: 
* Esta clase crepresenta una unica instancia de una clase parametro del
* template singleton.  Esta clase solo se instancia un unica vez
*
*********************************************************************/

#ifndef __SINGLETON_HH__
#define __SINGLETON_HH__

template <class T> 
class Singleton {
  public:
    static T* Instance() ;

  private:
    Singleton() ;
    ~Singleton() ;
    
    static T* mInstance ;
    
} ;

template <class T> T* Singleton<T>::mInstance = 0 ;

template <class T> T* Singleton<T>::Instance() {
  if ( mInstance == 0 )
    mInstance = new T ;
  return mInstance ;
}

template <class T> Singleton<T>::Singleton() { }

template <class T> Singleton<T>::~Singleton() {
  if ( mInstance != 0 )
    delete mInstance ;
}

#endif
