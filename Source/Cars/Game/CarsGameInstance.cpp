// Fill out your copyright notice in the Description page of Project Settings.


#include "CarsGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Net/Manager.h"
#include "Kismet/GameplayStatics.h"
#include "GameNet/GameBuffer.h"

UCarsGameInstance::UCarsGameInstance(const FObjectInitializer& oi)
: Super(oi)
, m_oGameNetMgr(this)
{
  m_pManager = Net::CManager::getSingletonPtr();
}

void UCarsGameInstance::OnStart()
{
  Super::OnStart();
  ChangeMenuWidget(StartingWidgetClass);
}

void UCarsGameInstance::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
  if (CurrentWidget)
  {
    CurrentWidget->RemoveFromViewport();
    CurrentWidget = nullptr;
  }
  if (NewWidgetClass)
  {
    CurrentWidget = CreateWidget<UUserWidget>(this, NewWidgetClass);
    if (CurrentWidget)
    {
      CurrentWidget->AddToViewport();
    }
  }
}


void UCarsGameInstance::OnServerButtonClick(FString sPort)
{
  GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, *FString("Server"));
  m_pManager->activateAsServer(FCString::Atoi(*sPort));
}

void UCarsGameInstance::OnClientButtonClick(FString sIP, FString sPort)
{
  GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, *FString("Client"));
  m_pManager->activateAsClient();
  m_pManager->connectTo(TCHAR_TO_ANSI(*sIP), FCString::Atoi(*sPort));
}

void UCarsGameInstance::OnServerStartButtonClick()
{
  const char* sLevel = "Circuit1";
  UGameplayStatics::OpenLevel(GetWorld(), sLevel);
  CGameBuffer oData;
  Net::NetMessageType xID = Net::LOAD_MAP;
  oData.write(xID);
  oData.write(sLevel);
  m_pManager->send(oData.getbuffer(), oData.getSize(), true);
}
