
#ifndef QRYCURIMPL_H
#define QRYCURIMPL_H

#include <sqldb/fwork/object.h>
#include <sqldb/fwork/status.h>
#include <sqldb/fwork/fnamelist.h>
#include <sqldb/metadata/indexmd.h>
#include <ifound.h>
#include <cfix.h>
#include <sqldb/fwork/condbuil.h>
#include <sqldb/fwork/sqlfwqrymd.h>

class ParamValue;

class QueryCursorImpl : public Object  {

public:
	QueryCursorImpl(String query, ConnectionImpl& conn);
	virtual ~QueryCursorImpl();

	void setParameterInfo(SQLQueryMetadata &qmd);

	virtual CursorOp::Status setParameters(const ParamValue &pv) = 0;
	
	virtual CursorOp::Status fetchNext() = 0;
	virtual CursorOp::Status execute() = 0;

	virtual CursorOp::Status restart() = 0;


	virtual String convertToString() const;

	virtual SQLQueryMetadata& getMetadata();
	virtual SQLQueryMetadata& getParametersInfo();

	// Debug
	virtual void dump() const;
	virtual Object *clone() const;



	virtual bool isNull(int fldIndex) = 0;
	virtual int getInt(int fldIndex) = 0;
	virtual long getLong(int fldIndex) = 0;
	virtual String getString(int fldIndex) = 0;
	virtual double getDouble(int fldIndex) = 0;
	virtual Date getDate(int fldIndex) = 0;
	virtual Time getTime(int fldIndex) = 0;
	virtual Num getNum(int fldIndex) = 0;

	virtual const String &getErrorDescr() const = 0;
protected:

	String query_d;
	SQLQueryMetadata *params_d;
	SQLQueryMetadata *metadata_d;
	ParamValue *paramValue_d;
	ConnectionImpl	*connection_d;


	const inline int paramsCount() { return params_d == NULL ? 0 : params_d->getColumnCount(); }

	const inline String &queryStr() { return query_d; }
	/**
	 * This method is used to get the connection associated to this table
	 * implementation.
	 *
	 * @return    the Connection used for the table implementation.
	 *
	 * @see ConnectionImpl
	 */
	const inline ConnectionImpl &connection() const { return *connection_d; }

	int getType(type ty, UShort len);
	QueryCursorImpl(const QueryCursorImpl &);
	QueryCursorImpl &operator=(const QueryCursorImpl &);


};


#endif
