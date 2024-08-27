// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Field/FieldSystemActor.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Field/FieldSystemComponent.h"
#include "DestructorFieldSystem.generated.h"

UCLASS()
class MYTH_API ADestructorFieldSystem : public AActor
{
	GENERATED_BODY()

public:
    ADestructorFieldSystem();

    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Destruction")
    void Explode();

    UFUNCTION(BlueprintCallable, Category = "Destruction")
    void SpawnAtLocation(const FVector& Location);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field Properties", meta = (AllowPrivateAccess = "true"))
    float FalloffMagnitude;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field Properties", meta = (AllowPrivateAccess = "true"))
    float VectorMagnitude;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field Properties", meta = (AllowPrivateAccess = "true"))
    float SphereRadius;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USphereComponent* CollisionSphere;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UFieldSystemComponent* FieldSystem;


};