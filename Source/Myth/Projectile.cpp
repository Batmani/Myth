#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GeometryCollection/GeometryCollectionActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Engine/DamageEvents.h"

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
    if (OtherComp)
    {
        AGeometryCollectionActor* DestructibleComp = Cast<AGeometryCollectionActor>(OtherComp);
        if (DestructibleComp)
        {
            FVector ImpactPoint = Hit.ImpactPoint;
            FVector ImpactDirection = NormalImpulse.GetSafeNormal();
            float ImpulseStrength = 10000.0f; // Adjust this value as needed
            // Apply impulse at the point of impact
            float damageTaken = DestructibleComp->TakeDamage(ImpulseStrength, FDamageEvent(),
                GetInstigatorController(), this);// The last value is the radius of damage, adjust it as needed

            UE_LOG(LogProjectile, Warning, TEXT("Actor damage taken: %f"), damageTaken);

        }
    }
    //if (OtherComp  )
    //{
    //    UGeometryCollectionComponent* DestructibleComp = Cast<UGeometryCollectionComponent>(OtherComp);
    //    if (DestructibleComp)
    //    {
    //        FVector ImpactPoint = Hit.ImpactPoint;
    //        FVector ImpactDirection = NormalImpulse.GetSafeNormal();
    //        float ImpulseStrength = 10000.0f; // Adjust this value as needed
    //        // Apply impulse at the point of impact
    //        DestructibleComp->ReceiveComponentDamage(ImpulseStrength, FDamageEvent(),
    //            GetInstigatorController(), this);// The last value is the radius of damage, adjust it as needed
    //        DestructibleComp-> AddImpulseAtLocation(ImpactDirection * ImpulseStrength, ImpactPoint);
    //         
    //        UE_LOG(LogProjectile, Warning, TEXT("Its new projectile"));

    //    }
    //}

    // Destroy the projectile after it hits something
    Destroy();
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

