// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NycoAnimInstance.h"
#include "Character/NycoCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/Weapon.h"

void UNycoAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	NycoCharacter = Cast<ANycoCharacter>(TryGetPawnOwner());
}

void UNycoAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (NycoCharacter == nullptr)
	{
		NycoCharacter = Cast<ANycoCharacter>(TryGetPawnOwner());
	}

	if (NycoCharacter == nullptr)
	{
		return;
	}

	FVector Velocity = NycoCharacter->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();

	bIsInAir = NycoCharacter->GetCharacterMovement()->IsFalling();
	bIsAccelerating = NycoCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
	bWeaponEquipped = NycoCharacter->IsWeaponEquipped();
	EquippedWeapon = NycoCharacter->GetEquippedWeapon();
	bIsCrouched = NycoCharacter->bIsCrouched;
	bAiming = NycoCharacter->IsAiming();
	TurningInPlace = NycoCharacter->GetTurningInPlace();

	// Offset Yaw for Strafing
	FRotator AimRotation = NycoCharacter->GetBaseAimRotation();
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(NycoCharacter->GetVelocity());
	FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
	DeltaRototation = FMath::RInterpTo(DeltaRototation, DeltaRot, DeltaTime, 15.f);
	YawOffset = DeltaRototation.Yaw;

	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = NycoCharacter->GetActorRotation();
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
	const float Target = Delta.Yaw / DeltaTime;
	const float Interp = FMath::FInterpTo(Lean, Target, DeltaTime, 6.f);
	Lean = FMath::Clamp(Interp, -90.f, 90.f);

	AO_Yaw = NycoCharacter->GetAO_Yaw();
	AO_Pitch = NycoCharacter->GetAO_Pitch();

	if (bWeaponEquipped && EquippedWeapon && EquippedWeapon->GetWeaponMesh() && NycoCharacter->GetMesh())
	{
		LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("LeftHandSocket"), ERelativeTransformSpace::RTS_World);
		FVector OutPosition;
		FRotator OutRotation;
		NycoCharacter->GetMesh()->TransformToBoneSpace(FName("RightHand"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);
		LeftHandTransform.SetLocation(OutPosition);
		LeftHandTransform.SetRotation(FQuat(OutRotation));
	}
}