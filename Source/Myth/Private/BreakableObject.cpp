#include "BreakableObject.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "PhysicsEngine/BodySetup.h"
#include "Engine/Engine.h"
#include "Field/FieldSystemComponent.h"
#include "Field/FieldSystemNodes.h"
#include "UObject/ConstructorHelpers.h"
#include "ManiDestructibleInterface.h"
#include "UObject/Object.h"
//#include "Engine/EditorResources/FieldNodes/FS_MasterField/FS_MasterField.h";
#include "GeometryCollection/GeometryCollectionComponent.h"

// Sets default values
ABreakableObject::ABreakableObject()
{

	PrimaryActorTick.bCanEverTick = true;

	//MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	//RootComponent = MeshComponent;
	//MeshComponent->SetSimulatePhysics(true);
	//MeshComponent->SetNotifyRigidBodyCollision(true);

 

	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	GeometryCollection->SetupAttachment(RootComponent);
	GeometryCollection->SetNotifyBreaks(true);

	//FieldSystemComponent = CreateDefaultSubobject<UFieldSystemComponent>(TEXT("FieldSystemComponent"));
	//FieldSystemComponent.SetupAttachment(RootComponent);
}

void ABreakableObject::BulletImpact_Implementation(UPrimitiveComponent* OtherComp, const FHitResult& HitInfo, const FVector& ImpactPoint, bool OverrideRadius, float Radius)
{

}
void ABreakableObject::ApplyDestructionField(const FHitResult& ImpactPoint)
{

	URadialFalloff* RadialFalloff = NewObject<URadialFalloff>();
	RadialFalloff->SetRadialFalloff(RadialMagnitude, 0.f, 1.f, 1.f, FieldRadius, ImpactPoint.ImpactPoint, EFieldFalloffType::Field_FallOff_None);

	UE_LOG(LogTemp, Warning, TEXT("destruction trying"));
	// Create a new Field System Component if not already created
	if (!FieldSystemComponent)
	{
		FieldSystemComponent = NewObject<UFieldSystemComponent>(this, UFieldSystemComponent::StaticClass(), TEXT("FieldSystemComponent"));
		FieldSystemComponent->SetupAttachment(RootComponent);
		FieldSystemComponent->RegisterComponent();
		FieldSystemComponent->ApplyPhysicsField(true, EFieldPhysicsType::Field_ExternalClusterStrain, nullptr, RadialFalloff);
	}
	GeometryCollection->ApplyPhysicsField(true, EGeometryCollectionPhysicsTypeEnum::Chaos_LinearVelocity, nullptr, RadialFalloff);
	UGameplayStatics::ApplyPointDamage(this, 100.0f, GetActorLocation(), ImpactPoint, nullptr, this, UDamageType::StaticClass());

}
void ABreakableObject::BeginPlay()
{
	Super::BeginPlay();
	//InitializeFromDataAsset();
	//MeshComponent->OnComponentHit.AddDynamic(this, &ABreakableObject::OnComponentHit);
	if (GeometryCollection)
	{
		GeometryCollection->OnComponentHit.AddDynamic(this, &ABreakableObject::OnComponentHit);
		GeometryCollection->OnChaosBreakEvent.AddDynamic(this, &ABreakableObject::HandleChaosBreakEvent);
	}


}

void ABreakableObject::HandleChaosBreakEvent(const FChaosBreakEvent& BreakEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("Object fractured at location: %s"), *BreakEvent.Location.ToString());

	// Example: Use the break event data to trigger additional effects or logic
	// You can access the data provided by the event, such as:
	FVector BreakLocation = BreakEvent.Location;
	FVector BreakVelocity = BreakEvent.Velocity;
	int32 BrokenPieceIndex = BreakEvent.Index;

}

void ABreakableObject::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	UE_LOG(LogTemp, Warning, TEXT("hit the  field working"));
	if (OtherActor && OtherActor != this)
	{
		UE_LOG(LogTemp, Warning, TEXT("hit  field working"));
		ApplyDestructionField(Hit);

	}
}


void ABreakableObject::OnChaosPhysicsCollision(const FChaosPhysicsCollisionInfo& CollisionInfo)
{
	UE_LOG(LogTemp, Warning, TEXT("chaos  field working"));
	// Filter out collisions with other breakable objects
	UPrimitiveComponent* OtherComponent = CollisionInfo.Component.Get();
	if (OtherComponent && OtherComponent->GetOwner() && OtherComponent->GetOwner()->IsA(ABreakableObject::StaticClass()))
	{
		return;
	}

	FVector ImpulseDir = CollisionInfo.Normal.GetSafeNormal();
	float ImpulseMag = CollisionInfo.AccumulatedImpulse.Size();

	//MeshComponent->AddTorqueInRadians(ImpulseDir.Cross(FVector::UpVector) * TorqueMult * ImpulseMag);

	if (ImpulseMag > 200.0f)  // Threshold for triggering break event
	{
		TriggerBreakEvent();
	}
}

void ABreakableObject::OnBulletImpact(const FHitResult& HitInfo, float Radius)
{
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
