// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Trap.generated.h"

class ACar;
UCLASS()
class CARS_API ATrap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	//Mesh
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* m_pMesh;
	UPROPERTY(EditAnywhere)
	FVector m_vOutsidePosition;
	UPROPERTY(EditAnywhere)
	float m_fEffectDuration;

	ACar* m_pCar;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:

	FVector GetOutsidePosition() { return m_vOutsidePosition; }
	float GetEffectDuration() { return m_fEffectDuration; }

	void SetCar(ACar* _pCar) { m_pCar = _pCar; }

};
