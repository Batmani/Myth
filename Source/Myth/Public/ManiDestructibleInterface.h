// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ManiDestructibleInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UManiDestructibleInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IManiDestructibleInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Destruction")
    void BulletImpact(UPrimitiveComponent* OtherComp, const FHitResult& HitInfo, const FVector& ImpactPoint, bool OverrideRadius, float Radius);
};