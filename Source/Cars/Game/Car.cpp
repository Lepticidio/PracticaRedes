// Fill out your copyright notice in the Description page of Project Settings.

#include "Car.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "ConstructorHelpers.h"
#include "CarMovementComponent.h"
#include "GameNet/NetComponent.h"


// Sets default values
ACar::ACar()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
  UBoxComponent* BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
  RootComponent = BoxComponent;
  m_pMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
  m_pMesh->SetupAttachment(RootComponent);
  static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxVisualAsset(TEXT("/Engine/BasicShapes/Cube"));
  if (BoxVisualAsset.Succeeded())
  {
    m_pMesh->SetStaticMesh(BoxVisualAsset.Object);
    static ConstructorHelpers::FObjectFinder<UMaterial> CarMaterial(TEXT("Material'/Game/Textures/Car'"));
    m_pMesh->SetMaterial(0, CarMaterial.Object);
  }
  SetActorScale3D(FVector(0.2f, 0.1f, 0.05f));
  SetActorRotation(FRotator(0.f, 270.f, 0.f));
  AutoPossessAI = EAutoPossessAI::Disabled;

  m_pCarMovement = CreateDefaultSubobject<UCarMovementComponent>(TEXT("CarMovement"));
  bReplicates = false;

  m_pNet = CreateDefaultSubobject<UNetComponent>(TEXT("Net"));
}

// Called when the game starts or when spawned
void ACar::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

//  m_pCarMovement->SetInput(m_vMovementInput);
  m_pNet->SetInput(m_vMovementInput);
}

void ACar::Move(float AxisValue)
{
  m_vMovementInput.Y = FMath::Clamp<float>(AxisValue, -1.f, 1.f);
}

void ACar::Turn(float AxisValue)
{
  m_vMovementInput.X = FMath::Clamp<float>(AxisValue, -1.f, 1.f);
}

void ACar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
  Super::SetupPlayerInputComponent(PlayerInputComponent);
  PlayerInputComponent->BindAxis("Move", this, &ACar::Move);
  PlayerInputComponent->BindAxis("Turn", this, &ACar::Turn);

}

float ACar::GetVelocityMagnitude() const
{
  return m_pCarMovement->GetVelocityMagnitude();
}