#include "BreakableObject.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ABreakableObject::ABreakableObject()
{
    PrimaryActorTick.bCanEverTick = true;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;
    MeshComponent->SetSimulatePhysics(true);
    MeshComponent->SetNotifyRigidBodyCollision(true);

    DestructionField = CreateDefaultSubobject<URadialForceComponent>(TEXT("DestructionField"));
    DestructionField->SetupAttachment(RootComponent);
    DestructionField->Radius = FieldRadius;
    DestructionField->ForceStrength = RadialMagnitude;
    DestructionField->DestructibleDamage = 0.0f;
}

void ABreakableObject::BeginPlay()
{
    Super::BeginPlay();

    MeshComponent->OnComponentHit.AddDynamic(this, &ABreakableObject::OnChaosPhysicsCollision);
}

void ABreakableObject::OnChaosPhysicsCollision(const FChaosPhysicsCollisionInfo& CollisionInfo)
{
    // Filter out collisions with other breakable objects
    if (CollisionInfo.OtherActor->IsA(ABreakableObject::StaticClass()))
    {
        return;
    }

    FVector ImpulseDir = CollisionInfo.Normal.GetSafeNormal();
    float ImpulseMag = CollisionInfo.AccumulatedImpulse.Size();

    MeshComponent->AddTorqueInRadians(ImpulseDir.Cross(FVector::UpVector) * TorqueMult * ImpulseMag);

    if (ImpulseMag > 200.0f)  // Threshold for triggering break event
    {
        TriggerBreakEvent();
    }
}

void ABreakableObject::OnBulletImpact(const FVector& ImpactPoint, float ImpactRadius)
{
    SetActorEnableCollision(true);  // Force dynamic state
    ApplyDestructionField(ImpactPoint);

    float DamageRadius = FMath::Max(ImpactRadius, MinDamageRadius);
    FVector SpawnLocation = ImpactPoint + FVector(0.0f, 0.0f, DamageRadius);

    // Spawn destruction field (particle effect)
  /*  if (BreakingEffects)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BreakingEffects, SpawnLocation, FRotator::ZeroRotator, FVector(DamageRadius));
    }*/
}

void ABreakableObject::TriggerBreakEvent()
{
    // Switch material if relevant
    // Spawn breaking particle effect
   /* if (BreakingEffects)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BreakingEffects, GetActorLocation());
    }*/

    // Play breaking sound
    if (BreakingSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, BreakingSound, GetActorLocation());
    }

    // Additional break logic (e.g., spawning debris, changing mesh, etc.)
}

void ABreakableObject::ApplyDestructionField(const FVector& FieldLocation)
{
    DestructionField->SetWorldLocation(FieldLocation);
    DestructionField->FireImpulse();
}