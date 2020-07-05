// Fill out your copyright notice in the Description page of Project Settings.


#include "Trap.h"
#include "Game/Car.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "ConstructorHelpers.h"

// Sets default values
ATrap::ATrap() : m_vOutsidePosition(FVector(99999999, 999999999, 99999999)), m_fEffectDuration(1)
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
	m_pMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	m_pMesh->OnComponentBeginOverlap.AddDynamic(this, &ATrap::OnOverlapBegin);
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
void ATrap::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACar* pCar = Cast<ACar>(OtherActor);
	if (pCar != nullptr)
	{
		if (pCar != m_pCar)
		{
			pCar->SetInputBlocked(true);
			SetActorLocation(m_vOutsidePosition);
			m_pCar->SetTrapPosition(m_vOutsidePosition);
		}
	}
}