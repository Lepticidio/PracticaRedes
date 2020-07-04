// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Trap.generated.h"

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
	unsigned int m_uClient;
	UPROPERTY(EditAnywhere)
	FVector m_vOutsidePosition;
public:

	FVector GetOutsidePosition() { return m_vOutsidePosition; }

	UFUNCTION()
		unsigned int GetClient();
	UFUNCTION()
		void SetClient(unsigned int _uNewValue);
};
