#pragma once
#include "Net/Manager.h"

class CServerCars : public Net::CManager::IObserver
{
public:
  CServerCars();
  virtual ~CServerCars();

  void Step();

  // Net::CManager::IObserver
  virtual void dataPacketReceived(Net::CPacket* packet);
  virtual void connexionPacketReceived(Net::CPacket* packet);
  virtual void disconnexionPacketReceived(Net::CPacket* packet);
private:
  Net::CManager* m_pManager = nullptr;
};

