#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

DEFINE_LOG_CATEGORY(LogProjectile);
AProjectile::AProjectile()
{
    PrimaryActorTick.bCanEverTick = true;

    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
    RootComponent = ProjectileMesh;

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->InitialSpeed = 3000.0f;
    ProjectileMovement->MaxSpeed = 3000.0f;

}
void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherComp  )
    {
        UGeometryCollectionComponent* DestructibleComp = Cast<UGeometryCollectionComponent>(OtherComp);
        if (DestructibleComp)
        {
            FVector ImpactPoint = Hit.ImpactPoint;
            FVector ImpactDirection = NormalImpulse.GetSafeNormal();
            float ImpulseStrength = 10000.0f; // Adjust this value as needed

            // Apply impulse at the point of impact
            //DestructibleComp->ApplyDamage(ImpulseStrength, ImpactPoint, ImpactDirection, 200.0f); // The last value is the radius of damage, adjust it as needed
            DestructibleComp->AddImpulseAtLocation(ImpactDirection * ImpulseStrength, ImpactPoint);

            UE_LOG(LogProjectile, Warning, TEXT("Its projectile"));

        }
    }

    // Destroy the projectile after it hits something
    Destroy();
}
void AProjectile::BeginPlay()
{
    Super::BeginPlay();
    ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

void AProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AProjectile::FireInDirection(const FVector& ShootDirection)
{
    ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
}

