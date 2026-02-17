#ifndef __LISType_H
#define __LISType_H
 
template <class Type> class TList;
template <class Type> class TListCursor;

template <class Type> class TListNode
{
	friend class TList<Type>;
	friend class TListCursor<Type>;

	Type *_data;
	TListNode *_next;
	TListNode *_prev;
public:
	TListNode(Type *obj) : _data(obj) {}
	~TListNode() {}
};

template <class Type> class TList
{
	friend class TListCursor<Type>;

	TListNode<Type> *_first;
	TListNode<Type> *_last;
	int _count;

protected:

	inline TListNode<Type> *_find(Type *p) const;
	inline void __del(TListNode<Type> *l);
public:
	TList() : _first((TListNode<Type> *) 0),_last((TListNode<Type> *) 0),_count(0) {}
	~TList() {}
	inline void add(Type *p);
	inline void del(Type *p);
	inline void push(Type *p)
		{ add(p);}
	inline Type * popQueue();
	inline Type * popStack();
	inline Type *nth(int) const ;
	inline int isEmpty() const ;
	int count()  const { return _count;}
};


template <class Type> class TListCursor 
{
	TListNode<Type> *_cur;
	const TList<Type>  &_l;
public:
	TListCursor(const TList<Type> &l) : _l(l) {}
	~TListCursor() {}

	Type * first() {_cur=_l._first; return current();}
	Type * last() {_cur=_l._last; return current();}
	Type * current()	{return _cur ? _cur->_data : (Type *) 0;} 
	inline Type * next() ;
	inline Type * prev() ;
	inline Type * set(Type *obj) ;
};


#include <ifound/tlist.icc>

#ifdef name2
#undef name2
#endif

#define name2(a,b)	a##b

#define declareTList(a)						\
	class name2(a,TList)  : public TList<a> {	\
	public: 								\
		name2(a,TList)() : TList<a>() {} 		\
		~name2(a,TList)() {} ;				\
	}; \
	class name2(a,TListNode)  : public TListNode<a> {	\
	public: 										\
		name2(a,TListNode)(a *o) : TListNode<a>(o) {}	\
		~name2(a,TListNode)() {} ;					\
	}; 												\
	class name2(a,TListCursor)  : public TListCursor<a> {				\
	public: 														\
		name2(a,TListCursor)(const TList<a> &l) : TListCursor<a>(l) {}	\
		~name2(a,TListCursor)() {} ;									\
	};
		
#endif
