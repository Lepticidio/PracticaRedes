//---------------------------------------------------------------------------
// conexion.h
//---------------------------------------------------------------------------

/**
 * @file conexion.h
 *
 * Representa una conexi�n con un peer.
 *
 *
 * @author Juan A. Recio-Garc�a, David Llans�
 * @date Octubre, 2006
 */

#ifndef __CONEXION_H
#define __CONEXION_H

#define DEBUG_CONEXION 1

#include <string>

namespace Net {

	/** 
	ID de identificaci�n en la red.
	*/
	typedef unsigned int NetID;

class CConnection {

public:

	virtual int getAddress()=0;
	virtual short getPort()=0;
	virtual void setId(NetID id)=0;
	virtual NetID getId()=0;
};


} // namespace Net

#endif // __CONEXION_H