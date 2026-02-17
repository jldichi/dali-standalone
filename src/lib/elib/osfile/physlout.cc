/********************************************************************
* Copyright (c) 1995 InterSoft Co.  All Rights Reserved
* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft Co.
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
* $Id: physlout.cc,v 1.12 2003/05/29 22:37:09 albertoi Exp $
*
* DESCRIPTION
*
*********************************************************************/

#ifdef __GNUC__
#	pragma implementation
#endif

#include <ifound.h>
#include <ifound/rexp.h>
#include <ifound/stdc.h>
#include <ifound/itypes.h>

#include <local/physlout.h>
#include <ifound/bufffile.h>

#include <errno.h>

declare(PtrArray, PhysLayoutEntry);
implement(PtrArray, PhysLayoutEntry);
declare(PtrArrayCursor, PhysLayoutEntry);
implement(PtrArrayCursor, PhysLayoutEntry);
declare(PtrArray, String);
implement(PtrArray, String);
declare(PtrArrayCursor, String);
implement(PtrArrayCursor, String);
declare(PtrArray, Range);
implement(PtrArray, Range);
declare(PtrArrayCursor, Range);
implement(PtrArrayCursor, Range);

#define MIN_FIRST_RANGED	2048

#define PACKED_DESC		"PACKED"
#define RANGED_DESC		"RANGED"
#define ROUNDROBIN_DESC	"ROUNDROBIN"

enum {
	PACKED,
	RANGED,
	ROUNDROBIN,
};

#define MAX_LINE		4096
#define COMMENT_CHAR	'#'
#define FIELD_SEPAR		"\t"
#define STORE_SEPAR		","
#define SIZE_SEPAR		":"

#define KBYTES			'K'
#define MBYTES			'M'
#define GBYTES			'G'

char buffer[MAX_LINE];

inline String rangedRanges(const String &line)
{
	return line.field(2, FIELD_SEPAR);
}

inline String rangedBytes(const String &line)
{
	return line.field(1, SIZE_SEPAR);
}

inline String rangedDir(const String &line)
{
	return line.field(0, SIZE_SEPAR);
}
inline String rRobinDirs(const String &line)
{
	return line.field(3, FIELD_SEPAR);
}

inline String rRobinBFactor(const String &line)
{
	return line.field(2, FIELD_SEPAR);
}

inline String packedDir(const String &line)
{
	return line.field(2, FIELD_SEPAR);
}

inline Int entryType(const String &et)
{
	String ET = et.toUpper();
	if (ET == RANGED_DESC)
		return RANGED;
	if (ET == ROUNDROBIN_DESC)
		return ROUNDROBIN;
	if (ET == PACKED_DESC)
		return PACKED;
	return ERR;	
}

static Int nBytes(String bytes)
{
	bytes.fullTrim();

	if (bytes == NULL_STRING)
		return INT_NULL;

	char lastChar = bytes[bytes.length()-1];
	
	Num nbytes;
	
	if (isdigit(lastChar))
		nbytes = toNum(bytes) * 1024;		
	else {
		switch (toupper(lastChar)) {
		case KBYTES:
			nbytes = toNum(bytes(0, bytes.length()-1)) * 1024;
			break;
		case MBYTES:
			nbytes = toNum(bytes(0, bytes.length()-1)) * 1024 * 1024;
			break;
		case GBYTES:
			nbytes = toNum(bytes(0, bytes.length()-1)) * 1024 * 1024 * 1024;
			break;
		default:
			error(libMsg("BADUNIT"), lastChar); 
			break;
		}
	}
	if (nbytes > INT_MAX-1)
		error(libMsg("BADAMOUNT"), INT_MAX-1);

	return toInt(nbytes);
}

// PhysLayoutEntry class

PhysLayoutEntry::PhysLayoutEntry(const LRegExp &r)
:	re_d(*tr_new LRegExp(r))
{ 
}

PhysLayoutEntry::PhysLayoutEntry(const String &r)
:       re_d(*tr_new LRegExp((const String &)r))
{
}

PhysLayoutEntry::~PhysLayoutEntry()
{ 
	tr_delete(&re_d);
}

bool PhysLayoutEntry::belongsTo(const String &file)
{
	return re_d.match(file);
}

const String &PhysLayoutEntry::pattern()
{
	return re_d.pattern();
}

void PhysLayoutEntry::dirSuffix(const String &ds)
{
	dirSuffix_d = ds;
}

// PackedEntry class

PackedEntry::PackedEntry(const LRegExp &r, const String &d)
:	PhysLayoutEntry(r),
	dir_d(d)
{ 
}

PackedEntry::PackedEntry(const String &r, const String &d)
:       PhysLayoutEntry(r),
        dir_d(d)
{
}

PackedEntry::~PackedEntry()
{ 
}

Int PackedEntry::nParts()
{ 
	return 1;		
}

String PackedEntry::part(Int )
{ 
	if (dirSuffix_d != NULL_STRING)
		return dir_d + PATH_SLASH + dirSuffix_d;	
	return dir_d;
}

void PackedEntry::store(BufferedFile *fp)
{
	sprintf(buffer, "%S%s%s%s%S\n", &re_d.pattern(), FIELD_SEPAR, PACKED_DESC,
								FIELD_SEPAR, &dir_d);
	fp->write(buffer);
}

// RoundRobinEntry class

RoundRobinEntry::RoundRobinEntry(const LRegExp &r, const String &d, 
								 const String &ps)
:	PhysLayoutEntry(r),
	dirs_d(*tr_new PtrArray(String)), 
	strSuggPageSize_d(ps),
	suggPageSize_d(nBytes(ps))
{ 
	for (StrCursor cur(d, STORE_SEPAR); cur; ++cur)
		dirs_d.add(tr_new String(cur.value()));
}

RoundRobinEntry::RoundRobinEntry(const String &r, const String &d,
                                                                 const String &ps)
:       PhysLayoutEntry((const String &)r),
        dirs_d(*tr_new PtrArray(String)),
        strSuggPageSize_d(ps),
        suggPageSize_d(nBytes(ps))
{
        for (StrCursor cur(d, STORE_SEPAR); cur; ++cur)
                dirs_d.add(tr_new String(cur.value()));
}

RoundRobinEntry::~RoundRobinEntry()
{ 
	for (PtrArrayCursor(String) cur(dirs_d); cur; ++cur)
		tr_delete(*cur);
	tr_delete(&dirs_d);
}

Int RoundRobinEntry::nParts()
{
	return dirs_d.dim();
}

Int RoundRobinEntry::part(Long &offset, Int bs)
{
	assert(bs > 0);

	Int pageSize = suggPageSize_d > 2 * bs ? suggPageSize_d / bs * bs
										   : 2 * bs;
	Int nparts	= dirs_d.dim();
	assert(nparts != 0);
	Long blkN	= offset / pageSize;
	offset		= blkN / nparts * pageSize + (offset % pageSize);

	return Int(blkN % nparts);
}

String RoundRobinEntry::part(Int i)
{
	if (dirSuffix_d != NULL_STRING)
		return *dirs_d[i] + PATH_SLASH + dirSuffix_d;	
	return *dirs_d[i];
}

void RoundRobinEntry::store(BufferedFile *fp)
{
	char *p = buffer;
	sprintf(p, "%S%s%s%s%S%s", &re_d.pattern(), FIELD_SEPAR, ROUNDROBIN_DESC, 
								FIELD_SEPAR, &strSuggPageSize_d, FIELD_SEPAR);
	String dirs;
	for (PtrArrayCursor(String) cur(dirs_d); cur; ++cur) {
		if (dirs == NULL_STRING)
			dirs = **cur;
		else
			dirs += String(STORE_SEPAR) + **cur;
	}
	p += strlen(p);
	sprintf(p, "%S\n", &dirs);
	fp->write(buffer);
}

// RangedEntry class

RangedEntry::RangedEntry(const LRegExp &r, const String &d)
:	PhysLayoutEntry(r), 
	ranges_d(*tr_new PtrArray(Range))
{
	String nbytes;
	Int i = 0;
	StrCursor cur(d, STORE_SEPAR);
	for (; cur; ++cur, i++) {
		nbytes = rangedBytes(cur.value());
		ranges_d.add(tr_new Range(rangedDir(cur.value()), nbytes));
		if (nbytes == NULL_STRING)
			break;
	}
	if (nbytes != NULL_STRING || ++cur)
		error(libMsg("LASTMUSTBENULL"), &re_d.pattern(), &d);
}

RangedEntry::RangedEntry(const String &r, const String &d)
:       PhysLayoutEntry((const String &)r),
        ranges_d(*tr_new PtrArray(Range))
{
        String nbytes;
        Int i = 0;
        StrCursor cur(d, STORE_SEPAR);
        for (; cur; ++cur, i++) {
                nbytes = rangedBytes(cur.value());
                ranges_d.add(tr_new Range(rangedDir(cur.value()), nbytes));
                if (nbytes == NULL_STRING)
                        break;
        }
        if (nbytes != NULL_STRING || ++cur)
                error(libMsg("LASTMUSTBENULL"), &re_d.pattern(), &d);
}

RangedEntry::~RangedEntry()
{
	for (PtrArrayCursor(Range) cur(ranges_d); cur; ++cur)
		tr_delete(*cur);
	tr_delete(&ranges_d);
}

Int RangedEntry::nParts()
{
	return ranges_d.dim();
}

Int RangedEntry::nBytes(Int n, Int bs, Int nbytes)
{
	// in the first part I have to have room for at least 2 blocks.
	if (n == 0)
		return  nbytes > 2 * bs ? nbytes / bs * bs : 2 * bs;
	// in other partitions I need room only for at least 1 block.
	return nbytes < bs ? bs : nbytes / bs * bs;
}

Int RangedEntry::part(Long &offset, Int bs)
{
	Int i = 0, nbytes;
	for (PtrArrayCursor(Range) cur(ranges_d); cur->nbytes!=INT_NULL; ++cur, i++) {
		nbytes = nBytes(i, bs, cur->nbytes);
		if (offset < nbytes)
			break;
		offset -= nbytes;
	}
	return i;
}

String RangedEntry::part(Int i)
{
	if (dirSuffix_d != NULL_STRING)
		return ranges_d[i]->dir + PATH_SLASH + dirSuffix_d;	
	return ranges_d[i]->dir;
}

void RangedEntry::store(BufferedFile *fp)
{
	char *p = buffer;
	sprintf(p, "%S%s%s%s", &re_d.pattern(), FIELD_SEPAR, RANGED_DESC, 
							FIELD_SEPAR);
	String dirs;
	PtrArrayCursor(Range) cur(ranges_d);
	for (; cur; ++cur) {
		if (dirs == NULL_STRING) {
			dirs = cur->dir;
		}
		else {
			dirs += String(STORE_SEPAR) + cur->dir;
		}
		if (cur->amount == NULL_STRING) {
			break;
		}
		dirs += String(SIZE_SEPAR) + cur->amount;
	}
	assert(!(++cur));
	p += strlen(p);
	sprintf(p, "%S\n", &dirs);
	fp->write(buffer);
}

Int RangedEntry::partSize(int part) {
	return ranges_d[part]->nbytes;
}

Range::Range(const String &d, const String &a)
:	dir(d), 
	amount(a),
	nbytes(nBytes(a))
{ 
}

Range::~Range()
{ 
}	

// PhysLayout class

PhysLayout::PhysLayout(const String &file)
:	parts_d(*tr_new PtrArray(PhysLayoutEntry)),
	fileName_d(file),
	dirty_d(false),
	ok_d(true)
{
	load();
}

PhysLayout::~PhysLayout()
{
	store();
	
	PtrArrayCursor(PhysLayoutEntry) cur(parts_d);
	for (; cur; ++cur)
		tr_delete(*cur);

	tr_delete(&parts_d);
}

void PhysLayout::load()
{
	BufferedFile fp(toCharPtr(fileName_d), READ_MODE);

	if (fp.open() < 0) {
		if (errno != ENOENT)
			ok_d = false;
		else {
			fp.setName(toCharPtr(fileName_d));
			fp.setMode(WRITE_MODE);
			if (fp.open() < 0)
				ok_d = false;
			else
				fp.close();
		}
		return;
	}

	for (PtrArrayCursor(PhysLayoutEntry) cur(parts_d); cur; ++cur)
		tr_delete(*cur);
	parts_d.reAlloc(0);
	
	char buff[MAX_LINE];
	String line, defaultDir;
	Int lineNo = 0;

	while (fp.readLine(buff) != NULL) {
		lineNo++;
		String line = buff;
		Int ncomm = line.index(COMMENT_CHAR);
		if (ncomm != ERR) {
			if (ncomm == 0)
				continue;
			line = line(0, ncomm);
		}
		PhysLayoutEntry *entry = NULL;
		const String rexp = line.field(0, FIELD_SEPAR);
		if (rexp == NULL_STRING)
			error(libMsg("CNFMALFORMED2"), &fileName_d, lineNo);
		switch (entryType(line.field(1, FIELD_SEPAR))) {
		case RANGED:
			{
				String ranges = rangedRanges(line);
				if (ranges == NULL_STRING)
					error(libMsg("CNFMALFORMED2"), &fileName_d, lineNo);
				entry = tr_new RangedEntry((const String &)rexp, ranges);
			}
			break;
		case ROUNDROBIN:
			{
				String dirs		= rRobinDirs(line);
				String bfactor	= rRobinBFactor(line);
				if (dirs == NULL_STRING || bfactor == NULL_STRING)
					error(libMsg("CNFMALFORMED2"), &fileName_d, lineNo);
				entry = tr_new RoundRobinEntry((const String &)rexp, dirs, bfactor);
			}
			break;
		case PACKED:
			{
				String dir = packedDir(line);
				if (dir == NULL_STRING)
					error(libMsg("CNFMALFORMED2"), &fileName_d, lineNo);
				entry = tr_new PackedEntry((const String &)rexp, dir);
			}
			break;
		default:
			error(libMsg("CNFMALFORMED2"), &fileName_d, lineNo);
			break;
		}
		parts_d.add(entry);
	}

	fp.close();
}

void PhysLayout::store()
{
	if (!dirty_d || !ok_d)
		return;

	BufferedFile fp(toCharPtr(fileName_d), WRITE_MODE);
	if (fp.open() < 0)
		error(libMsg("CANTOPENFILE"), &fileName_d);
	
	for (PtrArrayCursor(PhysLayoutEntry) cur(parts_d); cur; ++cur)
		cur->store(&fp);		

	fp.close();
	dirty_d = false;
}

PhysLayoutEntry *PhysLayout::find(const String &file, Int from)
{
	for (PtrArrayCursor(PhysLayoutEntry) cur(parts_d, from); cur; ++cur)
		if (cur->belongsTo(file))
			return *cur;
	return NULL;
}

void PhysLayout::addPackedEntry(const String &ln, const String &d, 
								const String &ds)
{
	dirty_d = true;
	PhysLayoutEntry *ple = tr_new PackedEntry((const String &)ln, d);
	if (ds != NULL_STRING)
		ple->dirSuffix(ds);
	parts_d.add(ple);
}

void PhysLayout::delEntry(Int i)
{
	parts_d.del(i);
	dirty_d = true;
}

Int PhysLayout::nEntries()
{
	return parts_d.dim();
}

PhysLayoutEntry *PhysLayout::entry(Int i)
{
	return parts_d[i];
}
	

