
#include <ideafix.h>
#include <cfix.h>
#include <perms.sch>
#include "c_ixscaina.h"

/*
 * Esta funcion antes seteaba a los miembros 'idUser' y 'login'
 * Dentro de la libreria de Ideafix no existe ninguna llamada a esta funcion,
 * por lo que se saco el seteo de los miembros aca, porque no corresponde.
 * Si esta funcion se llama externamente (por ejemplo desde un programa CFix),
 * no tiene efecto el seteo de esos miembros, ya que son de uso interno.
 */
String IxscaIna::getUserName(int uid)
{
	schema curr;
	curr = CurrentSchema(); // Se agrego esta llamada y el SwitchToSchema porque
									// al ejecutarse por GIF+INA habia un bug de error
									// de buffers internos en la conexion, porque buscaba
									// campos del esquema en uso, pero en perms, entonces
									// saltaba un assert y el programa en GIF pinchaba
	bool abrio = false;
	schema perms = FindSchema("perms");
	if (perms == ERROR)
	{
		perms = OpenSchema("perms", IO_EABORT);
		abrio = true;
	}

	String name;
	SetIFld(PERUSER_IDUSER, uid);
	if (GetRecord(PERUSERbyIDUSER, THIS_KEY, IO_NOT_LOCK) != ERROR) 
	{
		name = String(SFld(PERUSER_LOGIN));
	}
	else
	{
		char temp[5];
		sprintf(temp, "%ld", uid);
		name = String(temp);
	}
	if (abrio)
	{
		CloseSchema(perms);
	}
	SwitchToSchema(curr);

	return name;
}

