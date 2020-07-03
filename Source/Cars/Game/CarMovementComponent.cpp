// Fill out your copyright notice in the Description page of Project Settings.


#include "CarMovementComponent.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UCarMovementComponent::UCarMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCarMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCarMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector vAccel = CalculateAcceleration();
	FVector vAverageVelocity = CalculateAverageVelocity(vAccel, DeltaTime);
	MoveActor(vAverageVelocity, DeltaTime);
}

void UCarMovementComponent::SetInput(const FVector2D& _vInput)
{
	m_vMovementInput = _vInput;
}

FVector UCarMovementComponent::CalculateAcceleration() const
{
	FVector vAccel;
	if (m_vMovementInput.Y > 0.f)
	{
		vAccel = m_fAccel * m_vMovementInput.Y * GetOwner()->GetActorForwardVector();
	}
	else if (m_vMovementInput.Y < 0.f)
	{
		vAccel = m_fBrakeAccel * m_vMovementInput.Y * GetOwner()->GetActorForwardVector();
	}
	else
	{
		vAccel = -m_fDrag * GetOwner()->GetActorForwardVector();
	}

	vAccel += m_RotationFactor * m_vMovementInput.X * m_vVelocity.Size() * GetOwner()->GetActorRightVector();

	return vAccel;
}


FVector UCarMovementComponent::CalculateAverageVelocity(const FVector& _vAccel, float DeltaTime)
{
	FVector vAverageVelocity = m_vVelocity + 0.5f * _vAccel * DeltaTime;
	ClampVelocity(vAverageVelocity);

	m_vVelocity += _vAccel * DeltaTime;
	ClampVelocity(m_vVelocity);

	return vAverageVelocity;
}

void UCarMovementComponent::ClampVelocity(FVector& _vVelocity_) const
{
	if (_vVelocity_ != FVector::ZeroVector &&
		FVector::DotProduct(_vVelocity_, GetOwner()->GetActorForwardVector()) < 0.f )
	{
		_vVelocity_ = FVector::ZeroVector;
	}
	if (_vVelocity_.SizeSquared() > m_fMaxVelocity * m_fMaxVelocity)
	{
		_vVelocity_ *= m_fMaxVelocity / _vVelocity_.Size();
	}
}

void UCarMovementComponent::MoveActor(const FVector& _vAverageVelocity, float DeltaTime)
{
	FTransform oTrans = GetOwner()->GetActorTransform();
	oTrans.AddToTranslation(_vAverageVelocity * DeltaTime);
	GetOwner()->SetActorTransform(oTrans);
	if (_vAverageVelocity != FVector::ZeroVector)
	{
		FRotator oRot = FRotationMatrix::MakeFromX(_vAverageVelocity).Rotator();
		GetOwner()->SetActorRotation(oRot);
	}
}
