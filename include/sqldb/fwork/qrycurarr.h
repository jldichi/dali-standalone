#include <ideafix.h>
#include <cfix/ixsqlcur.h>
#include <ifound.h>
#include <sqldb/fwork/sqlfwqrycur.h>
#include <sqldb/fwork/sqlfwqrymd.h>


class QueryCursorArray : public Object {

public:

	QueryCursorArray();
	~QueryCursorArray();
	static Int newQueryCurSlot();
	static void freeCursor(sqlcursor cur);
	static bool existsQueryCursor(sqlcursor cur);
	static SQLFWQueryCursor *getQueryCurPtr(sqlcursor cur);
	static SQLQueryMetadata *getQueryMDPtr(sqlcursor cur);
	static SQLQueryMetadata *getQueryMetadata(sqlcursor cur);
	static sqlcursor setQueryCurPtr(Int slot, SQLFWQueryCursor *schptr, SQLQueryMetadata *qMd);

protected:
	static void freeQueryCurSlot(Int slot);
	static Int slotsOccupied(void);
	static bool destroyAllCursors();
	void dump();
};
