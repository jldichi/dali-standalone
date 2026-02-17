if [ $# -ne 1 ]
then
	echo "Uso: dropschema.sh <schema>"
	exit 1
fi

SCHEMANAME=$1

RemoveSchemaFromDB()
{

SQLFILE=$$.sql

> $SQLFILE;


echo "begin work;" >> $SQLFILE
echo " " >> $SQLFILE
echo "delete from metadata:schemas where extent='$SCHEMANAME';"		>> $SQLFILE
echo "delete from metadata:tables where sch = '$SCHEMANAME';"		>> $SQLFILE
echo "delete from metadata:tabmembers where sch = '$SCHEMANAME';"	>> $SQLFILE
echo "delete from metadata:indices where sch='$SCHEMANAME';"		>> $SQLFILE
echo "delete from metadata:indexattr where sch='$SCHEMANAME';"		>> $SQLFILE
echo "delete from metadata:enumattrs where sch='$SCHEMANAME';"		>> $SQLFILE
echo "delete from metadata:enums where sch='$SCHEMANAME';"			>> $SQLFILE
echo "delete from metadata:refattrs where sch='$SCHEMANAME';"		>> $SQLFILE
echo "delete from metadata:references where sch='$SCHEMANAME';"		>> $SQLFILE
echo "delete from metadata:schperms where sch='$SCHEMANAME';"		>> $SQLFILE
echo "delete from metadata:tabperms where sch='$SCHEMANAME';"		>> $SQLFILE
echo ""																>> $SQLFILE
echo "drop database $SCHEMANAME;"									>> $SQLFILE
echo "commit;"														>> $SQLFILE

# dbaccess -qc $SQLFILE 
dbaccess metadata@$INFORMIXSERVER < $SQLFILE > /dev/null 2>&1

rm $SQLFILE;
}

RemoveSchemaFromDB;
exit 0;
