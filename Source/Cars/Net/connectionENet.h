//---------------------------------------------------------------------------
// conexionENet.h
//---------------------------------------------------------------------------

/**
 * @file conexionENet.h
 *
 * Representa una conexi�n con un peer implementada con ENet.
 *
 *
 * @author Juan A. Recio-Garc�a, David Llans�
 * @date Octubre, 2006
 */

#ifndef __CONEXIONENET_H
#define __CONEXIONENET_H

#define DEBUG_CONEXION 1

#include "enet.h"
#include "connection.h"

namespace Net {

class CConnectionENet: public CConnection{

public:
	CConnectionENet():_peer(0), _id(0xFFFFFFFF){}

	int getAddress();
	short getPort();

	void setId(NetID id);

	NetID getId();

    void setENetPeer(ENetPeer* p);

	ENetPeer* getENetPeer();
private:
	ENetPeer* _peer;
	NetID _id;
};


} // namespace Net

#endif // __CONEXION_H