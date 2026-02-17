#ifndef	CURSOR_CACHE_CHECKER
#define	CURSOR_CACHE_CHECKER

#include <ideafix.h>
#include <ifound.h>

/**
 * These are the cursor possible directions.
 */
enum CursorDirection {
	NO_DIRECTION = -1,
	ASCENDING = 0,
	DESCENDING = 1
};

/**
 * This class is used to encapsulate the state of a cursor respect to
 * the cache of it. Sometimes records are read from cache without moving
 * the cursor pointer and we have to make sure that at the next neccesary
 * physical read, we place in the cache, the rigth records and not the
 * ones read from cache.
 */
class CacheChecker {
	dbcursor cur_d;
	CursorDirection	 lastDir_d;
	bool			 noRecordsLeft_d;
public:
	CacheChecker(dbcursor cur, CursorDirection cd);
	~CacheChecker();

	inline void setLastDirection(CursorDirection cd);

	inline CursorDirection getLastDirection() const;
	inline dbcursor cursorId() const;

	inline void setNoRecordsLeft(bool rl);
	inline bool getNoRecordsLeft() const;
};

declare(PtrArray, CacheChecker);

/**
 * This class is used to manager the CacheCheckers for all the created
 * CFIX cursors.
 */
class CursorCacheCheckerManager {
	gtype(PtrArray, CacheChecker) *cacheCheckerCursorList_d;

public:
	CursorCacheCheckerManager();
	~CursorCacheCheckerManager();
	void add(CacheChecker *cc);
	void del(dbcursor cur);
	CacheChecker *getCacheChecker(dbcursor cur) const;
};

#include <sqldb/cachechk.icc>

#endif	// CURSOR_CACHE_CHECKER
