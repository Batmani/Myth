#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Field/FieldSystemComponent.h"
#include "Field/FieldSystemObjects.h"
#include "BreakableObject.h"
#include "Field/FieldSystemTypes.h"
#include "Kismet/GameplayStatics.h"
#include "GeometryCollection/GeometryCollectionActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Engine/DamageEvents.h"
#include "Chaos/ChaosGameplayEventDispatcher.h"
#include "ManiDestructibleInterface.h"
#include "DestructorFieldSystem.h"

DEFINE_LOG_CATEGORY(LogProjectile);
AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ProjectileMesh;

	// Enable simulation generates hit events
	ProjectileMesh->SetNotifyRigidBodyCollision(true);
	ProjectileMesh->SetSimulatePhysics(true);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 2000.0f;
	ProjectileMovement->MaxSpeed = 2000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 0.3f;
	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
	//UE_LOG(LogProjectile, Warning, TEXT("Its projectile mesh "));


}
void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this)
	{
		if (ABreakableObject* BreakableObject = Cast<ABreakableObject>(OtherActor))
		{
			UE_LOG(LogProjectile, Warning, TEXT("Its onhit o maybe"));
			// Implement the break logic here
			//BreakableObject->ApplyDestructionField(Hit); // Assuming your breakable object class has a Break() method
			//UE_LOG(LogProjectile, Warning, TEXT("Its impul"));
			//OtherComp->AddImpulseAtLocation(ProjectileMovement->Velocity * 100.0f, Hit.ImpactPoint);
			//BreakableObject->BulletImpact(OtherComp, Hit, Hit.ImpactPoint, true, 55);
			FVector HitLocation = Hit.Location;
			FActorSpawnParameters SpawnParams;
			ADestructorFieldSystem* DestructorField = GetWorld()->SpawnActor<ADestructorFieldSystem>(
				ADestructorFieldSystem::StaticClass(),
				HitLocation,
				FRotator::ZeroRotator,
				SpawnParams
			);
			if (DestructorField)
			{
				DestructorField->FalloffMagnitude = FalloffMagnitude;
				DestructorField->VectorMagnitude = VectorMagnitude;
				DestructorField->SphereRadius = SphereRadius;
				UE_LOG(LogProjectile, Warning, TEXT("Its newest destr"));
				DestructorField->Explode();
				
			}

			// Destroy the projectile after the hit
			//Destroy();
		}


		//IManiDestructibleInterface::Execute_BulletImpact(OtherActor, OtherComp, Hit, Hit.ImpactPoint, true, 55);

		//FVector ImpactPoint = Hit.ImpactPoint;

		//// Create a Radial Falloff field to apply localized damage
		//URadialFalloff* RadialFalloff = NewObject<URadialFalloff>();
		//RadialFalloff->Magnitude = FalloffMagnitude;// 10000.0f;
		//RadialFalloff->Radius = SphereRadius;// 500.0f;
		//RadialFalloff->Position = ImpactPoint;
		//RadialFalloff->Falloff = EFieldFalloffType::Field_FallOff_None;


		////         Apply the Radial Falloff field to the Geometry Collection
		//UE_LOG(LogProjectile, Warning, TEXT("Physicsfield newests  applied "));
		//Check if the other actor implements the Destructible interface
		//if (OtherActor->GetClass()->ImplementsInterface(UManiDestructibleInterface::StaticClass()))
		//{
		//	// Apply the bullet impact or damage logic
	

		//	//if (OtherComp)
		//	//{
		//	//    if (UGeometryCollectionComponent* GeometryCollectionComp = Cast<UGeometryCollectionComponent>(OtherComp))
		//	//    {
		//	//        FVector ImpactPoint = Hit.ImpactPoint;

		//	//        // Create a Radial Falloff field to apply localized damage
		//	//        URadialFalloff* RadialFalloff = NewObject<URadialFalloff>();
		//	//        RadialFalloff->Magnitude = FalloffMagnitude;// 10000.0f;
		//	//        RadialFalloff->Radius = SphereRadius;// 500.0f;
		//	//        RadialFalloff->Position = ImpactPoint;
		//	//        RadialFalloff->Falloff = EFieldFalloffType::Field_FallOff_None;

		//	////         Apply the Radial Falloff field to the Geometry Collection
		//	//        GeometryCollectionComp->ApplyPhysicsField(true, EGeometryCollectionPhysicsTypeEnum::Chaos_ExternalClusterStrain, nullptr, RadialFalloff);
		//	//        UE_LOG(LogProjectile, Warning, TEXT("Physicsfield newests  applied "));

		//	//    
		//	//    }
		//	//}

		//	//Destroy();
		//	//UE_LOG(LogProjectile, Warning, TEXT("execute bullet maybe"));
		//}

		// Destroy the bullet after impact
	}

	Destroy();

	//if (OtherComp)
	//{
	//    if (UGeometryCollectionComponent* GeometryCollectionComp = Cast<UGeometryCollectionComponent>(OtherComp))
	//    {
	//        FVector ImpactPoint = Hit.ImpactPoint;

	//        // Create a Radial Falloff field to apply localized damage
	//        URadialFalloff* RadialFalloff = NewObject<URadialFalloff>();
	//        RadialFalloff->Magnitude = FalloffMagnitude;// 10000.0f;
	//        RadialFalloff->Radius = SphereRadius;// 500.0f;
	//        RadialFalloff->Position = ImpactPoint;
	//        RadialFalloff->Falloff = EFieldFalloffType::Field_FallOff_None;

	////         Apply the Radial Falloff field to the Geometry Collection
	//        GeometryCollectionComp->ApplyPhysicsField(true, EGeometryCollectionPhysicsTypeEnum::Chaos_ExternalClusterStrain, nullptr, RadialFalloff);
	//        UE_LOG(LogProjectile, Warning, TEXT("Physicsfield newests  applied "));

	//    
	//    }
	//}

	//Destroy();
}
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	if (ProjectileMesh)
	{
		ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
		//UE_LOG(LogProjectile, Warning, TEXT("projectile begin play"));

	}

}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
}

