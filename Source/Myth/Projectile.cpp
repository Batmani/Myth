#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Field/FieldSystemComponent.h"
#include "Field/FieldSystemObjects.h"
#include "Field/FieldSystemTypes.h"
#include "GeometryCollection/GeometryCollectionActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Engine/DamageEvents.h"
#include "Chaos/ChaosGameplayEventDispatcher.h"

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
        if (UGeometryCollectionComponent* GeometryCollectionComp = Cast<UGeometryCollectionComponent>(OtherComp))
        {
            FVector ImpactPoint = Hit.ImpactPoint;

            // Create a Radial Falloff field to apply localized damage
         //   URadialFalloff* RadialFalloff = NewObject<URadialFalloff>();
         //   RadialFalloff->Magnitude = FalloffMagnitude;// 10000.0f;
         //   RadialFalloff->Radius = SphereRadius;// 500.0f;
         //   RadialFalloff->Position = ImpactPoint;
         //   RadialFalloff->Falloff = EFieldFalloffType::Field_FallOff_None;

         //   // Apply the Radial Falloff field to the Geometry Collection
         ///*   GeometryCollectionComp->ApplyPhysicsField(true, EGeometryCollectionPhysicsTypeEnum::Chaos_ExternalClusterStrain, nullptr, RadialFalloff);
         //   UE_LOG(LogProjectile, Warning, TEXT("Physicsfield newests  applied "));*/

         //   // Generic Metadata
         //   TObjectPtr<UFieldSystemMetaDataFilter> MetaData = NewObject<UFieldSystemMetaDataFilter>();
         //   MetaData->SetMetaDataFilterType(
         //       EFieldFilterType::Field_Filter_All,
         //       EFieldObjectType::Field_Object_Destruction,
         //       EFieldPositionType::Field_Position_CenterOfMass);

         //   // Apply uniform external strain
         //   TObjectPtr<UUniformScalar> UniformScalar = NewObject<UUniformScalar>();
         //   UniformScalar->Magnitude = 9999999999.0;
 /*           GeometryCollectionComp->ApplyPhysicsField(
                true, EGeometryCollectionPhysicsTypeEnum::Chaos_ExternalClusterStrain, MetaData.Get(), UniformScalar.Get());*/

            //// Apply radial velocity
    /*        TObjectPtr<URadialVector> RadialVector = NewObject<URadialVector>();
            RadialVector->Magnitude = 50.0;
            RadialVector->Position = GetActorLocation();
            GeometryCollectionComp->ApplyPhysicsField(
                true, EGeometryCollectionPhysicsTypeEnum::Chaos_AngularVelocity, MetaData.Get(), RadialVector.Get());*/

            //// Apply A Uniform up Vector
            /*TObjectPtr<UUniformVector> UniformVector = NewObject<UUniformVector>();
            UniformVector->Magnitude = 100;
            UniformVector->Direction = FVector(0, 0, 1);
            GeometryCollectionComp->ApplyPhysicsField(true, EGeometryCollectionPhysicsTypeEnum::Chaos_LinearVelocity, MetaData.Get(), UniformVector.Get());*/
            URadialVector* RadialVector = NewObject<URadialVector>();
            UCullingField* CullingField = NewObject<UCullingField>();
            URadialFalloff* RadialFalloff = NewObject<URadialFalloff>();
            RadialVector->SetRadialVector(300, GetActorLocation());
            RadialFalloff->SetRadialFalloff(23, 0, 1, 0, 50, GetActorLocation(), EFieldFalloffType::Field_FallOff_None);
            CullingField->SetCullingField(RadialFalloff, RadialVector, EFieldCullingOperationType::Field_Culling_Outside);
            GeometryCollectionComp->ApplyPhysicsField(true, EGeometryCollectionPhysicsTypeEnum::Chaos_ExternalClusterStrain, nullptr, RadialFalloff);
            GeometryCollectionComp->ApplyPhysicsField(true, EGeometryCollectionPhysicsTypeEnum::Chaos_LinearVelocity, nullptr, CullingField);
        
        }
    }
    //if (OtherComp)
    //{
    //    AGeometryCollectionActor* DestructibleComp = Cast<AGeometryCollectionActor>(OtherComp);
    //    if (DestructibleComp)
    //    {
    //        FVector ImpactPoint = Hit.ImpactPoint;
    //        FVector ImpactDirection = NormalImpulse.GetSafeNormal();
    //        float ImpulseStrength = 10000.0f; // Adjust this value as needed
    //        // Apply impulse at the point of impact
    //        float damageTaken = DestructibleComp->TakeDamage(ImpulseStrength, FDamageEvent(),
    //            GetInstigatorController(), this);// The last value is the radius of damage, adjust it as needed
    //        UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(),
    //            ExplosiveProjectileInfo->BaseDamage, ExplosiveProjectileInfo->MinimumDamage,
    //            Hit.ImpactPoint, ExplosiveProjectileInfo->DamageInnerRadius, 
    //            ExplosiveProjectileInfo->DamageOuterRadius, 2.0f, DamageType, 
    //            IgnoreActors, GetOwner(), GetInstigatorController(), ECollisionChannel::ECC_Visibility);
    //        UE_LOG(LogProjectile, Warning, TEXT("Actor damage taken: %f"), damageTaken);

    //    }
    //}
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

