#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Field/FieldSystemComponent.h"
#include "Field/FieldSystemObjects.h"
#include "Field/FieldSystemTypes.h"
#include "Kismet/GameplayStatics.h"
#include "GeometryCollection/GeometryCollectionActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Engine/DamageEvents.h"
#include "Chaos/ChaosGameplayEventDispatcher.h"
#include "ManiDestructibleInterface.h"

DEFINE_LOG_CATEGORY(LogProjectile);
AProjectile::AProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
    RootComponent = ProjectileMesh;

    // Enable simulation generates hit events
    ProjectileMesh->SetNotifyRigidBodyCollision(true);
    ProjectileMesh->SetSimulatePhysics(true);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->InitialSpeed = 2000.0f;
    ProjectileMovement->MaxSpeed = 2000.0f;
    UE_LOG(LogProjectile, Warning, TEXT("Its projectile mesh "));


}
void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    UE_LOG(LogProjectile, Warning, TEXT("Its projectileaww maybe"));
    if (OtherActor && OtherActor != this)
    {
        // Check if the other actor implements the Destructible interface
        if (OtherActor->GetClass()->ImplementsInterface(UManiDestructibleInterface::StaticClass()))
        {
            // Apply the bullet impact or damage logic
            IManiDestructibleInterface::Execute_BulletImpact(OtherActor, Hit.ImpactPoint, 55);
        }

        // Destroy the bullet after impact
        Destroy();
    }
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
        UE_LOG(LogProjectile, Warning, TEXT("projectile begin play"));

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

