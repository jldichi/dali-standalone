#ifndef __DBCHOICE_H
#define __DBCHOICE_H


class IsoftDb;

IsoftDb *getDbClass(schema sc);
IsoftDb *getDbClass(char * );
void setDbType(schema sc, int dbtype);
void setSlotType(int slot , int dbtype);
schema _getSchema(struct s_schema *sc);
bool haveSomethingOpen(int dbtype);
void printSArray();
IsoftDb *getSqlFWDbClass();




#endif
