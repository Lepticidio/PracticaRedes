 // Fill out your copyright notice in the Description page of Project Settings.


#include "NetComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Game/Car.h"
#include "Game/CarsGameInstance.h"
#include "GameNet/GameBuffer.h"
#include "Net/Manager.h"
#include "Game/CarMovementComponent.h"

namespace
{
	float s_fSnapshotDelay = 0.5f;
}

// Sets default values for this component's properties
UNetComponent::UNetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UNetComponent::BeginPlay()
{
	Super::BeginPlay();

	m_pManager = Net::CManager::getSingletonPtr();
	
}

void UNetComponent::SetInput(const FVector2D& _vInput)
{
	if (_vInput != m_vMovementInput)
  {
    m_vMovementInput = _vInput;
		m_bSendCommand = true;
	}
}

// Called every frame
void UNetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UCarsGameInstance* pGameInstance = Cast<UCarsGameInstance>(GetWorld()->GetGameInstance());
	bool bOwner = pGameInstance->GetGameNetMrg()->GetOwnCar() == GetOwner();
	if (bOwner || m_pManager->getID() == Net::ID::SERVER)
	{
		m_fTimeToNextSnapshot -= DeltaTime;
		SerializeData();
	}

	if (m_pManager->getID() != Net::ID::SERVER)
	{
		if (bOwner)
		{
			SimulateOwnCarMovement(DeltaTime);
		}
		else
    {
      SimulateCarMovement(DeltaTime);
    }
	}
}

void UNetComponent::SerializeData()
{
	if (m_pManager->getID() == Net::ID::SERVER)
	{
		if (m_fTimeToNextSnapshot <= 0.f)
    {
			m_fTimeToNextSnapshot += s_fSnapshotDelay;
      CGameBuffer oData;
      oData.write(Net::ENTITY_MSG);
      UCarsGameInstance* pCarGI = Cast<UCarsGameInstance>(GetWorld()->GetGameInstance());
      ACar* pCar = Cast<ACar>(GetOwner());
      unsigned int uID = pCarGI->GetGameNetMrg()->GetCarID(pCar);
      oData.write(uID);
      oData.write(0); // SNAPSHOT;
      FVector vPos = GetOwner()->GetActorLocation();
      oData.write(vPos);
      FVector vDir = GetOwner()->GetActorForwardVector();
      oData.write(vDir);
			float fVelocity = pCar->GetCarMovement()->GetVelocityMagnitude();
			oData.write(fVelocity);
      m_pManager->send(oData.getbuffer(), oData.getSize(), false);
		}
	}
	else // Client
  {
		if (m_bSendCommand)
    {
			m_bSendCommand = false;
      CGameBuffer oData;
      oData.write(Net::ENTITY_MSG);
      oData.write(m_pManager->getID());
      oData.write(1); // COMMAND;
      oData.write(m_vMovementInput);
      m_pManager->send(oData.getbuffer(), oData.getSize(), true);
		}
  }
}

void UNetComponent::DeserializeData(CGameBuffer* _pData)
{
	int iLogicMessageType;
	_pData->read(iLogicMessageType);
	if (iLogicMessageType == 0) // SNAPSHOT
	{
		FVector vPos;
    _pData->read(vPos);
    FVector vDir;
    _pData->read(vDir);
    float fVelocity;
    _pData->read(fVelocity);
    m_vVelocity = vDir * fVelocity;
//		GetOwner()->SetActorLocation(vPos);
		FVector vCurrentPos = GetOwner()->GetActorLocation();
		m_vError = vPos - vCurrentPos;
		if (m_vError.SizeSquared() < 160000.f)
		{
			FVector vTargetPos = vPos + m_vVelocity * s_fSnapshotDelay;
			m_vVelocity = (vTargetPos - vCurrentPos) / s_fSnapshotDelay;
		}
		else
		{
      GetOwner()->SetActorLocation(vPos);
      FRotator oRot = FRotationMatrix::MakeFromX(vDir).Rotator();
      GetOwner()->SetActorRotation(oRot);
		}
    UCarsGameInstance* pGameInstance = Cast<UCarsGameInstance>(GetWorld()->GetGameInstance());
    bool bOwner = pGameInstance->GetGameNetMrg()->GetOwnCar() == GetOwner();
		if (bOwner && fVelocity > 0.f)
    {
      FRotator oRot = FRotationMatrix::MakeFromX(m_vVelocity).Rotator();
      GetOwner()->SetActorRotation(oRot);
		}
	}
	else // if(iLogicMessageType == 1) COMMAND
  {
    FVector2D vInput;
    _pData->read(vInput);
    ACar* pCar = Cast<ACar>(GetOwner());
    pCar->GetCarMovement()->SetInput(vInput);
  }
}

void UNetComponent::SimulateCarMovement(float DeltaTime)
{
	ACar* pCar = Cast<ACar>(GetOwner());
	pCar->GetCarMovement()->MoveActor(m_vVelocity, DeltaTime);
}


void UNetComponent::SimulateOwnCarMovement(float DeltaTime)
{
  ACar* pCar = Cast<ACar>(GetOwner());
	pCar->GetCarMovement()->SetInput(m_vMovementInput);
	FTransform oTrans = GetOwner()->GetActorTransform();
	FVector vReduction = m_vError * DeltaTime / s_fSnapshotDelay;
	oTrans.AddToTranslation(vReduction);
	GetOwner()->SetActorTransform(oTrans);
}
