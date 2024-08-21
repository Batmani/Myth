// Copyright Carter Rennick, 2024. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Field/FieldSystemActor.h"
#include "CombatFieldSystemActor.generated.h"

//Field System Actor specifically for use in combat actions
UCLASS(meta = (DisplayName = "Projectile Field System"))
class MYTH_API ACombatFieldSystemActor : public AFieldSystemActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<class USphereComponent> Sphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<class URadialFalloff> RadialFalloff;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UOperatorField> OperatorField;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<class URadialVector> RadialVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<class URandomVector> RandomVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<class UUniformVector> UniformVector;

	// Functions
public:
	ACombatFieldSystemActor();

	// Variables
public:
	UPROPERTY(BlueprintReadOnly, Category = "Default")
	float StrainMagnitude = 50000.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Default")
	float ForceMagnitude = 5000.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Default")
	float TorqueMagnitude = 1000000.0f;
};