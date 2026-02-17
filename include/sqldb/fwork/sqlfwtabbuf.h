//- Copyright Notice
// -----------------------------------------------------------------------
// (C) Copyright 1998 InterSoft S.A.  All Rights Reserved
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF InterSoft S.A.
// The copyright notice above does not evidence any actual or intended 
// publication of such source code.
//
// $Id: sqlfwtabbuf.h,v 1.3 2004/09/16 21:35:30 damianm Exp $
// -----------------------------------------------------------------------

#ifndef SQLFWTABBUF_H
#define  SQLFWTABBUF_H

#include <ifound.h>
#include <sqldb/metadata/tablemd.h>

class BufferedTable;

#define	SQLFW_BYTE		0
#define SQLFW_CHAR		1
#define SQLFW_SHORT		2
#define SQLFW_LONG		3
#define SQLFW_DATE		4
#define SQLFW_TIME		5
#define SQLFW_DOUBLE	6
#define SQLFW_NUM		7

// Ojo habria que guardar si el campo pertenece o no a la pk.
struct s_bufferCFIX {
	UShort	offset;		// Desplazamiento en el Buffer CFIX
	int		type;
	int		f_itype;
	UShort	len;		// Longitud del dato
	UChar	ndec;		// Cantidad de decimales
	UShort	intLen;		// tamanio del tipo de dato interno.
	void	*nullValue;	// Valor nulo dummy para campos de la pk
};

class SQLFWTableBuffer {

public:
	SQLFWTableBuffer(TableMetadata *tableMetadata, schema sc, dbtable tab);
	~SQLFWTableBuffer();

	void setFld(int colno, bool isNull, char *data = NULL);
	void setBFld(int colno, bool isNull, char data = _MIN_BYTE);
	void setIFld(int colno, bool isNull, int data = NULL_SHORT);
	void setLFld(int colno, bool isNull, long data = NULL_LONG);
	void setFFld(int colno, bool isNull, double data = NULL_DOUBLE);
	void setNFld(int colno, bool isNull, Num *data = NULL);
	void setDFld(int colno, bool isNull, char *data = NULL);
	void setTFld(int colno, bool isNull, char *data = NULL);
	void setTFld(int colno, bool isNull, unsigned short data);


	// Modificaciones echas para adaptarlas al driver nativo de PostgreSql	
	void setFld(int colno, int pos, bool isNull, char *data = NULL);
	void setBFld(int colno, int pos, bool isNull, char data = _MIN_BYTE);
	void setIFld(int colno, int pos, bool isNull, int data = NULL_SHORT);
	void setLFld(int colno, int pos, bool isNull, long data = NULL_LONG);
	void setFFld(int colno, int pos, bool isNull, double data = NULL_DOUBLE);
	void setNFld(int colno, int pos, bool isNull, Num *data = NULL);
	void setDFld(int colno, int pos, bool isNull, char *data = NULL);
	void setTFld(int colno, int pos, bool isNull, char *data = NULL);
	void setTFld(int colno, int pos, bool isNull, unsigned short data);

	void initBuffer();
	int	getType(type ty, UShort len);

	inline struct s_bufferCFIX* cfixBuffer() { return cfixBuffer_d; };

protected:
	struct s_bufferCFIX *cfixBuffer_d;
	char *buffer;
	TableMetadata *tableMD;
	schema scId_d;
	dbtable tabId_d;

private:
	void createBufferDescriptor();
};

#endif // SQLFWTABBUF_H
