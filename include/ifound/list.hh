/********************************************************************
* Copyright (c) 1987 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* node.hh,v 1.1.1.1 2005/10/12 13:46:22 diegos Exp
*
* Fin: 
* Esta clase representa a una lista generica
*
*********************************************************************/

#ifndef __LIST_HH__
#define __LIST_HH__

#include "memdebug.hh"

// @DIEGOS
// @UPGRADE
// Si puede agregar que cuando agreguen un elemento en lugar de reservar
// memoria para un node, reservar para varios
// 
template <class C> class List {
  private:
    class node {
      friend class List<C> ;

      private:
        C     dato ;
        node* next ; 
        
      public:
        node() : next(0) {} ;
      
    } ;

    long   mCount      ;    // Cantidad de nodos
    node*  mHead   ;    // Cabeza de la lista
    node*  mCrt    ;    // Nodo corriente
    
  public:
    const static int FAIL ;
    const static int DONE ;
    
    List() ;
    ~List() ;

    /***
    * @DIEGOS
    *
    * Este metodo toma el dato corriente.  SI no hay corriente (eof() == true)
    * habra un assert
    *
    * @return Dato del corriente
    **/
    C get() ; 

    /***
    * @DIEGOS
    *
    * Este metodo toma el dato corriente.  SI no hay corriente (eof() == true)
    * habra un assert
    *
    * @return Referencia al dato del corriente
    **/
    C& rget() ; 

    /***
    * @DIEGOS
    * 
    * @return Este metodo devuelve 'true' si el corriente esta en el
    *         final, de lo contrario false
    *
    **/
    bool eof() { return mCrt == 0 ; }
    
    /***
    * @DIEGOS
    *
    * Este metodo se utiliza para agregar un elemento C a la lista.
    * Cuando la lista se destruye se borran los nodos (delete) pero no el 
    * contenido (esto se aplica al caso de que 'C' sean punteros, en cuyo
    * caso se debe hacer el delete por separado).
    * Sea la clase que sea 'C' debe tener definido e implementado el operador
    * '=' y el constructor de copia.
    * 
    * @param  e Elemento que agrego
    *
    **/
    void add( C e ) ;

    /***
    * @DIEGOS
    *
    * Este metodo sirve para moverse al siguiente elemento
    *
    * @return DONE, si se movio. FAIL si no hay siguiente
    *
    **/
    int  next() ;
    
    /***
    * @DIEGOS
    *
    * Este metodo sirve para moverse al primer elemento
    *
    * @return DONE, si se movio. FAIL la lista esta vacia 
    *
    **/
    int first() ;

    /***
    * @DIEGOS
    *
    * Este metodo borra el dato corrient. Elimina el nodo previo delete
    * del dato
    * 
    **/
    void del() ;

    /***
    * @DIEGOS
    *
    * Este metodo borra todos los nodos 
    * 
    **/
    void delAll() ;

    /***
    * @DIEGOS
    *
    * Este metodo devuelve la cantidad de elementos en la lista
    * 
    * @return Cantidad de elementos en la lista
    *
    **/
    long  count() ;
} ;

template <class C> const int List<C>::FAIL = -1 ;
template <class C> const int List<C>::DONE =  0 ;

template <class C> List<C>::List() : mCount(0), mHead(0), mCrt(0) { 
  // printf("List<C>::List\n");
} ;
template <class C> List<C>::~List() {
  delAll();
} ;

template <class C> C List<C>::get() { 
  if ( eof() )
    assert(0) ;

  return mCrt->dato ; 
}

template <class C> C& List<C>::rget() { 
  if ( eof() )
    assert(0) ;

  return mCrt->dato ; 
}

template <class C> void List<C>::add( C e ) {
  node* n = _new<node>() ;
	assert( n ) ;
  node* tmpCrt = mHead ;
  n->dato = e ;
  n->next = 0  ;
  
  ++mCount ;

  // Si la lista esta vacia
  if ( mHead == 0 ) {
    // WiMsg( "List<C>::add | mHead == 0\n");
    mHead = mCrt = n ;
  } else {
    // WiMsg( "List<C>::add | Busco ultimo.  Head(%d), Corriente(%d).\n", mHead, mCrt );
    while ( tmpCrt->next != 0 ) 
      tmpCrt = tmpCrt->next ;
  
    tmpCrt->next = n ; 
  }

  mCrt = n ;

  // printf("List<C>::add | mHead %d  mCrt %d  Ultimo %d\n", mHead, mCrt, tmpCrt ) ;
  return ;
} ;

template <class C> int List<C>::next() {
  // printf("List<C>::next\n");
  // SI hay corriente
  if ( mCrt == 0 )
    return FAIL ;

  mCrt = mCrt->next ;

  return DONE ;
} ; 
    
template <class C> int List<C>::first() {
  if ( mHead == 0 ) 
    return FAIL ;
 
  mCrt = mHead ;

  return DONE ;
} ;

template <class C> void List<C>::delAll() {
  // printf("List<C>::delAll | Head %d\n", mHead);
  // Si no hay cabeza
  if ( mHead == 0 ) 
    return ;

  // printf("List<C>::delAll | Voy al 1\n");
  first() ;

  // printf("List<C>::delAll | Borro datos\n");
  // Borro los datos
  while ( mHead != 0 ) 
    del() ;
  
}

template <class C> void List<C>::del() {
  if ( mCount != 0 ) 
    --mCount ;
  else 
    return ; 

  // Corriente == Cabeza
  if ( mHead == mCrt ) {
    // printf("List<C>::del | mHead == mCrt \n") ;
    // printf("List<C>::del | mHead %d  mCrt %d\n", mHead, mCrt ) ;
    node* tmpHead = mHead ;

    // Hago que la cabeza apunte al proximo
    tmpHead = mHead->next ;

    // Muevo el corriente a la nueva cabeza
    mCrt = tmpHead ;
    
    _delete<node>( mHead );
    mHead = tmpHead ;
    // printf("List<C>::del | mHead %d  mCrt %d\n", mHead, mCrt ) ;
    return ;
  } // Final 'Corriente == Cabeza

  node* anterior   = 0 ;
  node* siguiente  = mCrt->next ;

  // Buso el anterior al nodo que querio borrar
  for ( anterior = mHead; anterior->next != 0; anterior = anterior->next ) 
    if ( anterior->next == mCrt ) 
      break ;

  _delete<node>( mCrt ) ;
  anterior->next = siguiente ;
  mCrt            = anterior ;

  
}

template <class C> long List<C>::count() {
  return mCount ;
}
#endif // __LIST_HH__ 
