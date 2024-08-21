#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Logging/LogMacros.h"
#include "Projectile.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogProjectile, Log, All);

UCLASS()
class MYTH_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent *ProjectileMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    class UProjectileMovementComponent *ProjectileMovement;

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


    void FireInDirection(const FVector& ShootDirection);
};
