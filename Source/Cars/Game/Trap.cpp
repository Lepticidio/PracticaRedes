// Fill out your copyright notice in the Description page of Project Settings.


#include "Trap.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "ConstructorHelpers.h"

// Sets default values
ATrap::ATrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	UBoxComponent* BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
	RootComponent = BoxComponent;
	m_pMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	m_pMesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxVisualAsset(TEXT("/Engine/BasicShapes/Cube"));
	if (BoxVisualAsset.Succeeded())
	{
		m_pMesh->SetStaticMesh(BoxVisualAsset.Object);
		static ConstructorHelpers::FObjectFinder<UMaterial> TrapMaterial(TEXT("Material'/Game/Textures/Trap'"));
		m_pMesh->SetMaterial(0, TrapMaterial.Object);
	}
	SetActorScale3D(FVector(0.1f, 0.1f, 0.1f));

}

// Called when the game starts or when spawned
void ATrap::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
unsigned int ATrap::GetClient()
{
	return m_uClient;
}
void ATrap::SetClient(unsigned int _uNewValue)
{
	m_uClient = _uNewValue;
}