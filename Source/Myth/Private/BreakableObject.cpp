#include "BreakableObject.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "PhysicsEngine/BodySetup.h"
#include "ManiDestructibleInterface.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

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

    GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
    GeometryCollection->SetupAttachment(RootComponent);
}

void ABreakableObject::BeginPlay()
{
    Super::BeginPlay();
    MeshComponent->OnComponentHit.AddDynamic(this, &ABreakableObject::OnComponentHit);

    // Bind Chaos Physics collision if available
  /*  if (UBodySetup* BodySetup = MeshComponent->GetBodySetup())
    {
        BodySetup->On OnChaosPhysicsCollision.AddDynamic(this, &ABreakableObject::OnChaosPhysicsCollision);
    }*/
}

void ABreakableObject::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

    if (OtherActor && OtherActor != this)
    {
        // Check if the other actor implements the Destructible interface
        if (OtherActor->GetClass()->ImplementsInterface(UManiDestructibleInterface::StaticClass()))
        {
            // Apply the bullet impact or damage logic
            IManiDestructibleInterface::Execute_BulletImpact(OtherActor, Hit , true, DefaultRadius);
        }
    }
    // Filter out collisions with other breakable objects
    //if (OtherActor && OtherActor->IsA(ABreakableObject::StaticClass()))
    //{
    //    return;
    //}

    //FVector ImpulseDir = NormalImpulse.GetSafeNormal();
    //float ImpulseMag = NormalImpulse.Size();

    //MeshComponent->AddTorqueInRadians(ImpulseDir.Cross(FVector::UpVector) * TorqueMult * ImpulseMag);

    //if (ImpulseMag > 200.0f)  // Threshold for triggering break event
    //{
    //    TriggerBreakEvent();
    //}
}


void ABreakableObject::OnChaosPhysicsCollision(const FChaosPhysicsCollisionInfo& CollisionInfo)
{
    // Filter out collisions with other breakable objects
    UPrimitiveComponent* OtherComponent = CollisionInfo.Component.Get();
    if (OtherComponent && OtherComponent->GetOwner() && OtherComponent->GetOwner()->IsA(ABreakableObject::StaticClass()))
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
void ABreakableObject::BulletImpact_Implementation(const FHitResult& HitInfo, bool OverrideRadius, float Radius)
{
    // Example logic for handling a bullet impact.
    FVector ImpactLocation = HitInfo.ImpactPoint;

    // If OverrideRadius is true, use the provided Radius; otherwise, use a default value.
    float ActualRadius = OverrideRadius ? Radius : DefaultRadius;

    // Apply the destruction field at the impact location
    ApplyDestructionField(ImpactLocation);

    // Play a sound or trigger other effects as needed
    /*if (BreakingSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, BreakingSound, GetActorLocation());
    }*/

    // Additional logic can be added here based on the specifics of your game
}
void ABreakableObject::OnBulletImpact(const FVector& ImpactPoint, float ImpactRadius)
{
    SetActorEnableCollision(true);  // Force dynamic state
    ApplyDestructionField(ImpactPoint);

 /*   float DamageRadius = FMath::Max(ImpactRadius, MinDamageRadius);
    FVector SpawnLocation = ImpactPoint + FVector(0.0f, 0.0f, DamageRadius);*/

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