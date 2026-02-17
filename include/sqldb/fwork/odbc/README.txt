Esta es una lista de preguntas que quedan sin responder:

- La clase Connection no deberia tener un metodo Execute()?

- Quien tiene una conexion?

- Por que para hacer un query SQL, el StatementBuilder debe recibir un IndexMeta
data en vez de un Index?


Comentarios:
-----------

Los que tienen la conexion son SchemaImpl y TableImpl. En realidad tienen un 
puntero a la conexion, ya que el encargado de administrar las conexiones es
ImplementationManager.

La conexion se crea/destruye a nivel de esquema.

Cada vez que una ODBCTableImpl tiene que ejecutar un statement le debe pedir a la referencia a su miembro ConnectionImpl. Este miembro en realidad no lo tiene el, sino que lo hereda de TableImpl (deberia ser una ODBCConnectionImpl). A esta conexion le va a pasar el statement preparado y debe bindear
los campos de su TableValue para que el resultado de la consulta quede ahi.
Es responsabilidad de Table obtener los datos de ahi.

Clases:
------

ImplementationManager: es responsable de crear las conexiones de los tipos correspondientes(ODBCConnectioInpl, XXXXConnectionImpl, etc), asi como tambien de las
implementaciones de las tablas(ODBCTableImpl, XXXTableImpl, etc).

StatementBuilder: es el encargado de armar las consultas a partir de los datos 
que le pasan (la tabla, el indice, el conjunto de valores). Por ahora es una clase estatica, ya que solo encapsula funcionalidad.

ODBCTableImpl: tiene un TableValue para almacenar los resultados de las consultas. Sabe comodialogar con ODBCConnectionImpl para obtener los resultados de la
consulta.



Eduardoc:
--------
Seria buebo tener el ODBCConnectionManager. De esa manera se podrian empezar
a hacer pruebas.


-----------------------------------------
Cosas que vi cuando edu estuvo de vacaciones:

* En el constructor de IndexArray se piden new IndexMetadata (tantos como indices haya en el IndexArray). Sin embargo estos IndexMetadata se inicializan con la
direccion de un elemento de un vector de s_index y un dbindex = ERR>
En getTablesTableMetadata se crean muchas cosas, y entre ellas un IndexArray 
y un nuevo IndexMetadata.
Por lo tanto hay dos IndexMetadatas apuntando a la misma posisicion dentro del
array de s_index.

* Algo parecido pasa con el constructor de IndexFieldArray, que crea IndexFieldMetadata.

* Cual es la diferencia entre IndexFieldArray e IndexFieldList?? Me parece que ninguna. Por las dudas no voy a cambiar nada...
// van los 2. Son distintos: IndexFieldList lo usan los constraints...

* Cuando se construye un SQLFWIndex se le pasa un dbindex como parametro. Sin
embargo ese dbindex no se almacena en ningun lado: entonces para que se lo paso?
(Me parece que el dbindex se guarda adentro del IndexMetadata, sin embargo en el
constructor el IndexMD se setea en NULL. Quizas deba hacer un new...)
//vamos a darle una chance...

* En un momento se construye un TableMetada local. Se hace asi para que no quede una tablemetadata que nadie lo va a usar... Sin embargo cuando estoy cargando la tabla desde la BD Informix, y estoy preparando el select query, necesito saber los nombres de los campos de la tabla. Para obtenerlos necesito el tablemetadata....

* En la funcion findSchSlot se empieza a buscar desde el slot 1 en vez del 0. Por que?????
Lo mismo pasa en newSchSlot...
Puede ser qye en el slot 0 este el esquema corriente??? Es decir que en el lugar 0 siempre haya un esquema que esta tambien en alguno de los otros slots??
// ya esta documentado.... current y el puntero (o un indice) a current y a 
// esquema metadata.

* Estoy teniendo problemas en setSchPtr, porque cuando hace 

	opened_schemas[slot] = schptr

no esta seteando el valor. Me parece que el problema puede venir por el hecho de
que opened_schemas es un arreglo de punteros estatico...
// ya esta solucionado

* Por lo que veo OpenedSchemas no es una clase estatica, pero que tiene
un miembro estatico. Por lo tanto no se deberian instanciar 2 OpendSchemas
nunca.
// ya esta solucionado (lo comente :)

* Tengo un gran problema. Cuando estoy tratando de acceder a la tabla de tablas
del esquema metadata, tengo que generar un query de select. Para obtener los
nombres de los campos necesito obtener el tableMd de la tabla metadata.tables.
Para esto le pido al schemaMD que me de el tableMD. el problema es que nunca
se registro el tableMd en el schemaMd. (Recordar que cuando se instancia un 
nuevo SchemaMD no se cargan sus tablas).
Por lo tanto tengo que asociar el tableMD al schemaMD, o la otra posibilidad
es utilizar un nuevo metodo que se especialize en obtener esa tabla en 
particular....
// listo. Ya lo hice....
