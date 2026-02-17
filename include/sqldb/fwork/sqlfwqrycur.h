#ifndef SQLFWQRYCUR_H
#define SQLFWQRYCUR_H

#include <sqldb/fwork/object.h>
#include <sqldb/fwork/status.h>
#include <sqldb/fwork/fnamelist.h>
#include <sqldb/metadata/indexmd.h>
#include <ifound.h>
#include <cfix.h>
#include <sqldb/fwork/condbuil.h>
#include <sqldb/fwork/sqlfwqrymd.h>
#include <sqldb/fwork/qrycurimpl.h>

class SQLFWQueryCursor : public Object {

public:
	// Constructors & destructors
	SQLFWQueryCursor(schema sc, QueryCursorImpl *qImpl);
	~SQLFWQueryCursor();


	// Description of parameter Types.
	void setParameterInfo(SQLQueryMetadata& paramInfo);

	// Setting parameter values
	void setInt(int fldIndex, int value);
	void setBool(int fldIndex, bool value); //Agregado por BZ 1891
	void setLong(int fldIndex, long value);
	void setString(int fldIndex, char *value);
	void setDouble(int fldIndex, double value);
	void setDate(int fldIndex, Date &value);
	void setTime(int fldIndex, Time &value);
	void setNum(int fldIndex, Num &value);


	// Prepare
	SQLQueryMetadata getMetadata();

	// Fetching
	CursorOp::Status fetchCursor();
	CursorOp::Status execute();
	CursorOp::Status restart();

	int getFldIndex(const char *fldName);
	String getFldName(int fldIndex);

	bool isNull(int fldIndex);
	int getInt(int fldIndex);
	long getLong(int fldIndex);
	String getString(int fldIndex);
	double getDouble(int fldIndex);
	Date getDate(int fldIndex);
	Time getTime(int fldIndex);
	Num getNum(int fldIndex);

	// Debug
	void dump() const;
	String convertToString() const;
	Object *clone() const;

	void addFieldTarget(int fldIndex, dbfield target, int column = -1);

	void defineLevel(int lvl, dbtable tab);
	CursorOp::Status fetchLevel(int lvl);

private:
	SQLFWQueryCursor(const SQLFWQueryCursor &);
	SQLFWQueryCursor &operator=(const SQLFWQueryCursor &);
	void processFieldTargets();
	bool processFieldTargets(dbtable tab, bool checkOnly);
	

	SQLQueryMetadata *parameterInfo_d;
	ParamValue *params_d;
	SQLQueryMetadata *queryInfo_d;
	QueryCursorImpl	*qryCursorImpl_d;
	bool resetParams;

	dbfield *fldMapping;
	schema *fldSchema;
	int *fldColumn;       //For vector fields
	dbtable *levelsDef;
	bool *fetchNeeded;
	CursorOp::Status fetchResult;
};

#endif
