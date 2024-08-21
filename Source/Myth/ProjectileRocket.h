//#pragma once
//
//#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"
//#include "GameFramework/ProjectileMovementComponent.h"
//#include "Field/FieldSystemActor.h"
//#include "Components/SphereComponent.h"
//#include "Field/FieldSystemActor.h"
//#include "Field/FieldSystemObjects.h"
//#include "Field/FieldSystemComponent.h"
//#include "ProjectileRocket.generated.h"
//
//UCLASS()
//class MYTH_API AProjectileRocket : public AFieldSystemActor
//{
//	GENERATED_BODY()
//
//public:
//	// Sets default values for this actor's properties
//	AProjectileRocket();
//
//
//	UPROPERTY(EditAnywhere)
//	float Damage = 20;
//
//	UPROPERTY(EditAnywhere)
//	USphereComponent* ExplosionRadius;
//
//	UPROPERTY(EditAnywhere)
//	URadialFalloff* RadialFalloff;
//
//	UPROPERTY(EditAnywhere)
//	UCullingField* CullingField;
//
//	UPROPERTY(EditAnywhere)
//	URadialVector* RadialVector;
//
//	UPROPERTY(EditAnywhere)
//	float ExplosionStrain = 500000;
//
//	UPROPERTY(EditAnywhere)
//	float ExplosionVelocity = 2000;
//
//protected:
//	// Called when the game starts or when spawned
//	virtual void BeginPlay() override;
//
//public:
//	// Called every frame
//	virtual void Tick(float DeltaTime) override;
//
//	// The Rocket Mesh
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", Replicated)
//	UStaticMeshComponent* RocketMesh;
//
//	// The Projectile Movement Component
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", Replicated)
//	UProjectileMovementComponent* ProjectileMovementComponent;
//
//	// Reference to the owner class
//	AActor* Owner;
//
//	// Function called when the Rocket hits an object and explodes
//	UFUNCTION()
//	void OnRocketHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector Normal, const FHitResult& HitResult);
//
//	// Function that will start firing the rocket in a provided direction
//	void FireInDirection(const FVector& ShootDirection);
//
//};