
#ifndef IREPORT_H
#define IREPORT_H

#include <ifound/str.h>
#include <essentia/except.h>

extern void ExecReport(String repIn, String dbName, String sqlExpr, String repOut, String repSize, String repParam[], int cantParam, String outRepName);

extern void ExecReportXML(String repIn, String xmlName, String selExpr, String repOut, String repSize, String repParam[], int cantParam, String outRepName);

#endif // IREPORT_H
