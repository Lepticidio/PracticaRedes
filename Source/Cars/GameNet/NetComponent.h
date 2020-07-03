// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NetComponent.generated.h"

namespace Net
{
	class CManager;
}
class CGameBuffer;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CARS_API UNetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNetComponent();

	void SetInput(const FVector2D& _vInput);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

  void SimulateCarMovement(float DeltaTime);
  void SimulateOwnCarMovement(float DeltaTime);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SerializeData();
	void DeserializeData(CGameBuffer* pData);

private:
	FVector2D m_vMovementInput = FVector2D::ZeroVector;
	Net::CManager* m_pManager;
	bool m_bSendCommand = true;
	float m_fTimeToNextSnapshot = 0.f;
	FVector m_vVelocity = FVector::ZeroVector;
	FVector m_vError = FVector::ZeroVector;
};
