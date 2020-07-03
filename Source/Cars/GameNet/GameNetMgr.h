#pragma once

#include "Net/Manager.h"

class UCarsGameInstance;
class ACar;

class CGameNetMgr : public Net::CManager::IObserver
{
public:
  CGameNetMgr();
  CGameNetMgr(UCarsGameInstance* _pOwner);
  virtual ~CGameNetMgr();

  virtual void dataPacketReceived(Net::CPacket* packet);
  virtual void connexionPacketReceived(Net::CPacket* packet);
  virtual void disconnexionPacketReceived(Net::CPacket* packet);

  ACar* GetOwnCar() const;
  unsigned int GetCarID(ACar* _pCar) const;

private:
  void CreateCar(unsigned int _uClient, FVector _vPos);
public:
  void CreateTrap(ACar* _pCar);

private:
  Net::CManager* m_pManager = nullptr;
  UCarsGameInstance* m_pCarsGameInstance;
  unsigned int m_uMapLoadedNotifications = 0u;
  std::map<unsigned int, ACar*> m_vPlayers;
  std::map<ACar*, unsigned int> m_tPlayerIDs;
};