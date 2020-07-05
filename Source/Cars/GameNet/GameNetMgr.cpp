#include "GameNetMgr.h"
#include "GameNet/GameBuffer.h"
#include "Net/packet.h"
#include "Kismet/GameplayStatics.h"
#include "Game/CarsGameInstance.h"
#include "Engine/LevelStreaming.h"
#include "Game/Car.h"
#include "Game/Trap.h"
#include "GameNet/NetComponent.h"

CGameNetMgr::CGameNetMgr() : m_pCarsGameInstance(nullptr)
{
  Net::CManager::Init();
  m_pManager = Net::CManager::getSingletonPtr();
  m_pManager->addObserver(this);
}


CGameNetMgr::CGameNetMgr(UCarsGameInstance* _pOwner) : m_pCarsGameInstance(_pOwner)
{
  Net::CManager::Init();
  m_pManager = Net::CManager::getSingletonPtr();
  m_pManager->addObserver(this);
}

CGameNetMgr::~CGameNetMgr()
{
  m_pManager->removeObserver(this);
  Net::CManager::Release();
}

void CGameNetMgr::dataPacketReceived(Net::CPacket* packet)
{
  CGameBuffer oData;
  oData.write(packet->getData(), packet->getDataLength());
  oData.reset();
  Net::NetMessageType xType;
  oData.read(xType);
  switch (xType)
  {
  case Net::LOAD_MAP:
  {
    char sLevel[32];
    oData.read(sLevel);
    UGameplayStatics::OpenLevel(m_pCarsGameInstance->GetWorld(), sLevel);
    Net::NetMessageType xLoadedType = Net::MAP_LOADED;
    m_pManager->send(&xLoadedType, sizeof(xLoadedType), true);
  }
    break;
  case Net::MAP_LOADED:
  {
    ++m_uMapLoadedNotifications;
    if (m_uMapLoadedNotifications >= m_pManager->getConnections().size())
    {
      for (auto client : m_pManager->getConnections())
      {
        CGameBuffer oCreateCar;
        Net::NetMessageType xCreateCarType = Net::LOAD_PLAYER;
        oCreateCar.write(xCreateCarType);
        oCreateCar.write(client.first);
        FVector vPos(220.f, -310.f + client.first * 40.f, 0.f);
        oCreateCar.write(vPos);
        m_pManager->send(oCreateCar.getbuffer(), oCreateCar.getSize(), true);
        CreateCar(client.first, vPos);
      }
    }
  }
  break;
  case Net::LOAD_PLAYER:
  {
    unsigned int uClient;
    oData.read(uClient);
    FVector vPos;
    oData.read(vPos);
    CreateCar(uClient, vPos);
    if (uClient == m_pManager->getID())
    {
      APlayerController* pPC = GEngine->GetFirstLocalPlayerController(m_pCarsGameInstance->GetWorld());
      if (pPC)
      {
        pPC->Possess(m_vPlayers.at(uClient));
      }
    }
  }
  break;
  case Net::ENTITY_MSG:
  {
    unsigned int uClient;
    oData.read(uClient);

    ACar* pCar = m_vPlayers.at(uClient);
    pCar->GetNetComponent()->DeserializeData(&oData);
  }
  break;
  default:
    break;
  }
}

void CGameNetMgr::connexionPacketReceived(Net::CPacket* packet)
{
  if (m_pManager->getID() == Net::ID::SERVER)
  {
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "A client has been connected!");
  }
}

void CGameNetMgr::disconnexionPacketReceived(Net::CPacket* packet)
{

}

void CGameNetMgr::CreateCar(unsigned int _uClient, FVector _vPos)
{
  FActorSpawnParameters oSpawnInfo;
  oSpawnInfo.Name = FName("Car", _uClient);
  oSpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
  ACar* pCar = m_pCarsGameInstance->GetWorld()->SpawnActor<ACar>(_vPos, FRotator::ZeroRotator, oSpawnInfo);
  if (pCar)
  {
    pCar->SetManager(this);
    m_vPlayers[_uClient] = pCar;
    m_tPlayerIDs[pCar] = _uClient;
    CreateTrap(pCar);
    
  }
}

void CGameNetMgr::CreateTrap(ACar* _pCar)
{

    unsigned int uID  = m_tPlayerIDs[_pCar];

    FActorSpawnParameters oSpawnInfo;
    oSpawnInfo.Name = FName("Trap", uID);
    oSpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    ATrap* pTrap = m_pCarsGameInstance->GetWorld()->SpawnActor<ATrap>(FVector(9999999999, 999999999, 999999), FRotator::ZeroRotator, oSpawnInfo);
    _pCar->SetTrap(pTrap);
    pTrap->SetCar(_pCar);

}

ACar* CGameNetMgr::GetOwnCar() const
{
  if (m_pManager->getID() != Net::ID::SERVER)
  {
    return m_vPlayers.at(m_pManager->getID());
  }
  return nullptr;
}

unsigned int CGameNetMgr::GetCarID(ACar* _pCar) const
{
  return m_tPlayerIDs.at(_pCar);
}
