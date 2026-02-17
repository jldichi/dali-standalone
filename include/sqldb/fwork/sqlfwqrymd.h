#ifndef SQLFWQRYMD_H
#define SQLFWQRYMD_H

#include <sqldb/fwork/object.h>
#include <sqldb/fwork/status.h>
#include <sqldb/fwork/fnamelist.h>
#include <sqldb/metadata/indexmd.h>
#include <ifound.h>
#include <ifound/itypes.h>
#include <cfix.h>
#include <sqldb/fwork/condbuil.h>


class SQLQueryColumnMetadata {

public:
	inline SQLQueryColumnMetadata(String name, Int type, Int len, Int precision)
		{ name_d = name;type_d = type; len_d = len; precision_d = precision; }
	inline String &name() { return name_d; }
	inline Int &type() { return type_d; }
	inline Int &len() { return len_d; }
	inline Int &precision() { return precision_d; }
private:
	String name_d;
	Int type_d;
	Int len_d;
	Int precision_d;
	
};


class gtype(PtrArray, SQLQueryColumnMetadata); 

class SQLQueryMetadata {
	private:

		gtype (PtrArray, SQLQueryColumnMetadata) *arr;
		Int columnCount;
		Int current;
	public:
		SQLQueryMetadata();
		~SQLQueryMetadata();

		Int getColumnCount()  const;

		String getColumnName(int column) const;

		Int getPrecision(int column) const;

		Int getLen(int column) const;

		Int getColumnType(int column) const;

		void setColumnCount(int count);
		void addColumnMetadata(String name, int type, int len, int precision);
		inline bool isComplete()  { return current == columnCount; };

};


#endif
