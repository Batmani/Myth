#include "BreakableObject.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "PhysicsEngine/BodySetup.h"
#include "ManiDestructibleInterface.h"
//#include "Engine/EditorResources/FieldNodes/FS_MasterField/FS_MasterField.h";
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

void ABreakableObject::BulletImpact_Implementation(UPrimitiveComponent* OtherComp, const FHitResult& HitInfo, const FVector& ImpactPoint, bool OverrideRadius, float Radius)
{

    // Example logic for handling a bullet impact.
    //FVector ImpactLocation = HitInfo.ImpactPoint;

    //// If OverrideRadius is true, use the provided Radius; otherwise, use a default value.
    //float ActualRadius =   DefaultRadius;
    //FVector SpawnLocation = HitInfo.ImpactPoint;
    //FRotator SpawnRotation = FRotator::ZeroRotator;
    //FActorSpawnParameters SpawnParams;
    //SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    //ABP_DestructionField* DestructionField = GetWorld()->SpawnActor<ABP_DestructionField>(DestructionFieldClass, SpawnLocation, SpawnRotation, SpawnParams);

    //if (DestructionField)
    //{
    //    // Set properties of the DestructionField based on the Blueprint logic
    //    DestructionField->TorqueMult = 10.0f;
    //    DestructionField->TorqueVectorOverride = FVector(0.0f, 0.0f, 0.0f);  // Use your actual calculation here
    //    DestructionField->RadialMagnitude = 200.0f;
    //    DestructionField->DirectionalMagnitude = 250.0f;
    //    DestructionField->DirectionalVelocityVector = FVector(0.0f, 0.0f, 0.0f);  // Use your actual calculation here

    //    // Trigger the Destruction Field logic
    //    DestructionField->Trigger();
    //}
    //// Apply the destruction field at the impact location
    //ApplyDestructionField(ImpactLocation);
}

void ABreakableObject::BeginPlay()
{
    Super::BeginPlay();
    MeshComponent->OnComponentHit.AddDynamic(this, &ABreakableObject::OnComponentHit);
    InitializeFromDataAsset();
    // Bind Chaos Physics collision if available
  /*  if (UBodySetup* BodySetup = MeshComponent->GetBodySetup())
    {
        BodySetup->On OnChaosPhysicsCollision.AddDynamic(this, &ABreakableObject::OnChaosPhysicsCollision);
    }*/
}

void ABreakableObject::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

    if (OtherActor && OtherActor->Implements<UManiDestructibleInterface>())
    {
        IManiDestructibleInterface::Execute_BulletImpact( OtherActor, OtherComp, Hit, Hit.ImpactPoint, true, DefaultRadius);
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
//void ABreakableObject::BulletImpact_Implementation(const FHitResult& HitInfo, bool OverrideRadius, float Radius)
//{
//    // Example logic for handling a bullet impact.
//    FVector ImpactLocation = HitInfo.ImpactPoint;
//
//    // If OverrideRadius is true, use the provided Radius; otherwise, use a default value.
//    float ActualRadius = OverrideRadius ? Radius : DefaultRadius;
//
//    // Apply the destruction field at the impact location
//    ApplyDestructionField(ImpactLocation);
//
//    // Play a sound or trigger other effects as needed
//    /*if (BreakingSound)
//    {
//        UGameplayStatics::PlaySoundAtLocation(this, BreakingSound, GetActorLocation());
//    }*/
//
//    // Additional logic can be added here based on the specifics of your game
//}
void ABreakableObject::OnBulletImpact(const FHitResult& HitInfo, float Radius)
{
    if (DestructionSphereToSpawn)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = GetInstigator();

        // Spawn the destruction field
        AActor* DestructionField = GetWorld()->SpawnActor<AActor>(
            DestructionSphereToSpawn,
            HitInfo.ImpactPoint,
            FRotator::ZeroRotator,
            SpawnParams
        );

        //if (DestructionField)
        //{
        //    FS_MasterField* DestructionFieldMaster = Cast<FS_Master>(DestructionField);
        //    // Set parameters on the destruction field
        //    DestructionField->SetTorqueMultiplier(TorqueMultiplier);
        //    DestructionField->SetTorqueVectorOverride(TorqueVectorOverride);
        //    DestructionField->SetRadialMagnitude(RadialMagnitude);
        //    DestructionField->SetDirectionalMagnitude(DirectionalMagnitude);
        //    DestructionField->SetDirectionalVelocityVector(DirectionalVelocityVector);

        //    // Calculate and apply direction vector
        //    CalculateDirectionVector(HitInfo);

        //    // Trigger the destruction logic
        //    DestructionField->TriggerDestruction();
        //}
    }
}
//void ABreakableObject::CalculateDirectionVector(const FHitResult& HitInfo)
//{
//    // Calculate a directional vector based on the impact hit result
//    FVector CrossProduct = FVector::CrossProduct(HitInfo.Normal, FVector::UpVector);
//    FVector DirectionVector = CrossProduct * DirectionalMagnitude;
//
//    // Apply the calculated direction vector
//    DirectionalVelocityVector = DirectionVector;
//}
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

void ABreakableObject::InitializeFromDataAsset()
{
    if (DataAsset)
    {
        // Set the Geometry Collection
        if (DataAsset->GeometryCollection)
        {
            GeometryCollection->SetRestCollection(DataAsset->GeometryCollection);
        }

        // Set removal on sleep if required

        // Set the break sound

        // Set the override materials
        for (const auto& Elem : DataAsset->OverrideMaterials)
        {
            int32 MaterialIndex = Elem.Key;
            UMaterialInterface* Material = Elem.Value;
            GeometryCollection->SetMaterial(MaterialIndex, Material);
        }

        // Set damage thresholds

        // Set override damage thresholds

        // Set damage radius

        // Set physical material
        if (DataAsset->PhysicalMaterial)
        {
            GeometryCollection->BodyInstance.SetPhysMaterialOverride(DataAsset->PhysicalMaterial);
        }
    }
}
void ABreakableObject::ApplyDestructionField(const FVector& FieldLocation)
{
    DestructionField->SetWorldLocation(FieldLocation);
    DestructionField->FireImpulse();
}