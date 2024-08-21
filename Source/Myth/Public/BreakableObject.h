#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Chaos/ChaosGameplayEventDispatcher.h"
#include "BreakableObject.generated.h"

UCLASS()
class MYTH_API ABreakableObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABreakableObject();

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnChaosPhysicsCollision(const FChaosPhysicsCollisionInfo& CollisionInfo);

    UFUNCTION()
    void OnBulletImpact(const FVector& ImpactPoint, float ImpactRadius);

    UFUNCTION()
    void TriggerBreakEvent();

    UFUNCTION()
    void ApplyDestructionField(const FVector& FieldLocation);

private:
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(VisibleAnywhere)
    URadialForceComponent* DestructionField;

    UPROPERTY(EditAnywhere)
    float TorqueMult = 15.0f;

    UPROPERTY(EditAnywhere)
    float RadialMagnitude = 2000.0f;

    UPROPERTY(EditAnywhere)
    float FieldRadius = 150.0f;

    UPROPERTY(EditAnywhere)
    float MinDamageRadius = 10.0f;

   /* UPROPERTY(EditAnywhere)
    TSubclassOf<UDestructionParticle> BreakingEffects;*/

    UPROPERTY(EditAnywhere)
    USoundBase* BreakingSound;
};