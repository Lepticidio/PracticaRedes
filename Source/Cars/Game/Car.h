// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Car.generated.h"

class UCarMovementComponent;
class UNetComponent;
class CGameNetMgr;
class ATrap;

UCLASS()
class CARS_API ACar : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACar();
  // Called every frame
  virtual void Tick(float DeltaTime) override;

  virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

  float GetVelocityMagnitude() const;

  void SetInput(const FVector2D& _vInput) { m_vMovementInput = _vInput; }
  void SetManager(CGameNetMgr* _newValue);
  void SetTrap(ATrap* _pTrap) { m_pTrap = _pTrap; }
  FVector GetTrapPosition();

  UNetComponent* GetNetComponent() const { return m_pNet; }
  UCarMovementComponent* GetCarMovement() { return m_pCarMovement; }

  ATrap* GetTrap() { return m_pTrap; }

protected:
	FVector m_vTrapPosition;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

  //Input
  void Move(float AxisValue);
  void Turn(float AxisValue);
  void CreateTrap();

protected:
  //Mesh
  UPROPERTY(EditAnywhere)
  UStaticMeshComponent* m_pMesh;

  UPROPERTY(EditAnywhere)
  UCarMovementComponent* m_pCarMovement;

  UPROPERTY(EditAnywhere)
  UNetComponent* m_pNet;

  CGameNetMgr* m_pManager;

  ATrap* m_pTrap;


  //Input
  FVector2D m_vMovementInput = FVector2D::ZeroVector;
};
