#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Chaos/ChaosGameplayEventDispatcher.h"
#include "PDA_ChaosBreakable.h"
#include "ManiDestructibleInterface.h"
#include "BreakableObject.generated.h"

UCLASS()
class MYTH_API ABreakableObject : public AActor, public IManiDestructibleInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABreakableObject();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Destruction")
	void BulletImpact(UPrimitiveComponent* OtherComp, const FHitResult& HitInfo, const FVector& ImpactPoint, bool OverrideRadius, float Radius);
	virtual void BulletImpact_Implementation(UPrimitiveComponent* OtherComp, const FHitResult& HitInfo, const FVector& ImpactPoint, bool OverrideRadius, float Radius) override;
	//virtual void IManiDestructibleInterface::BulletImpact_Implementation(UPrimitiveComponent* OtherComp, const FHitResult& HitInfo, const FVector& ImpactPoint, bool OverrideRadius, float Radius ) override;
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnChaosPhysicsCollision(const FChaosPhysicsCollisionInfo& CollisionInfo);

	UFUNCTION()
	void OnBulletImpact(const FHitResult& HitInfo, float Radius);

	UFUNCTION()
	void TriggerBreakEvent();

	UFUNCTION()
	void ApplyDestructionField(const FVector& FieldLocation);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	URadialForceComponent* DestructionField;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Chaos Destruction")
	TSubclassOf<AActor> DestructionSphereToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UGeometryCollectionComponent* GeometryCollection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	float TorqueMult = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	float RadialMagnitude = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	float FieldRadius = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chaos", meta = (AllowPrivateAccess = "true"))
	float MinDamageRadius = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chaos", meta = (AllowPrivateAccess = "true"))
	float DefaultRadius = 100.0f;

	/*     UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	 TSubclassOf<UDestructionParticle> BreakingEffects;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundBase* BreakingSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chaos", meta = (AllowPrivateAccess = "true"))
    UPDA_ChaosBreakable* DataAsset; 

	void InitializeFromDataAsset();

	//void CalculateDirectionVector(const FHitResult& HitInfo);
};