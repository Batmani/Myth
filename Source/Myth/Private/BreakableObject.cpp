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
	static ConstructorHelpers::FClassFinder<AActor> DestructionFieldBPClass(TEXT("/Game/Characters/ManiTestBlueprint.ManiTestBlueprint_C"));
	if (DestructionFieldBPClass.Succeeded())
	{
		DestructionFieldClass = DestructionFieldBPClass.Class;
		UE_LOG(LogTemp, Warning, TEXT("Field class worked!"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Field class nope "));

	}
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetNotifyRigidBodyCollision(true);

	/*   DestructionField = CreateDefaultSubobject<URadialForceComponent>(TEXT("DestructionField"));
	   DestructionField->SetupAttachment(RootComponent);
	   DestructionField->Radius = FieldRadius;
	   DestructionField->ForceStrength = RadialMagnitude;
	   DestructionField->DestructibleDamage = 0.0f;*/

	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	GeometryCollection->SetupAttachment(RootComponent);
	GeometryCollection->SetNotifyBreaks(true);

	//FieldSystemComponent = CreateDefaultSubobject<UFieldSystemComponent>(TEXT("FieldSystemComponent"));
	//FieldSystemComponent.SetupAttachment(RootComponent);
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
void ABreakableObject::ApplyDestructionField(const FVector& ImpactPoint)
{
	/*if (bHasBroken)
		return;*/
	URadialFalloff* RadialFalloff = NewObject<URadialFalloff>();
	RadialFalloff->SetRadialFalloff(RadialMagnitude, 0.f, 1.f, 0.f, FieldRadius, ImpactPoint, EFieldFalloffType::Field_FallOff_None);
	/*FieldSystemComponent->ApplyPhysicsField(true, Field_ExternalClusterStrain, nullptr, RadialFalloff);

	RadialVector = RadialVector->SetRadialVector(ForceMagnitude, GetActorLocation());
	CullingField = CullingField->SetCullingField(RadialFalloff, RadialVector, Field_Culling_Outside);
	GetFieldSystemComponent()->ApplyPhysicsField(true, Field_LinearVelocity, nullptr, CullingField);*/

		// Create a new Field System Component if not already created
		if (!FieldSystemComponent)
		{
			FieldSystemComponent = NewObject<UFieldSystemComponent>(this, UFieldSystemComponent::StaticClass(), TEXT("FieldSystemComponent"));
			FieldSystemComponent->SetupAttachment(RootComponent);
			FieldSystemComponent->RegisterComponent();
			FieldSystemComponent->ApplyPhysicsField(true, EFieldPhysicsType::Field_ExternalClusterStrain, nullptr, RadialFalloff);
		}

		//UUniformVector* UniformVector = NewObject<UUniformVector>();
		//UniformVector->SetUniformVector(DirectionalVector, Magnitude);


		////TObjectPtr<URadialFalloff> ExternalStrain = RadialFalloff->SetRadialFalloff(10000.0f, 0.0f, 1.0f, 0.0f, Collision->GetScaledSphereRadius(), Collision->GetComponentLocation(), EFieldFalloffType::Field_FallOff_None);
		////TObjectPtr<URadialVector> LinearVelocity = RadialVector->SetRadialVector(ExplodeVelocity, Collision->GetComponentLocation());
		////// Create a Radial Falloff to represent the strain field
		////URadialFallOff* RadialFalloff = NewObject<URadialFalloff>();
		////RadialFalloff->Magnitude = 1000.0f;  // Strength of the strain
		////RadialFalloff->MinRange = 0.0f;
		////RadialFalloff->MaxRange = 100.0f;  // Radius of the field
		////RadialFalloff->Default = 0.0f;
		////RadialFalloff->Radius = 500.0f;  // Range in world units
		////RadialFalloff->Position = ImpactPoint;

		//// Create a strain field using the radial falloff
		//UFieldNodeFloat* StrainField = NewObject<UFieldNodeFloat>(RadialFalloff, UFieldNodeFloat::StaticClass());
		//UFieldSystemMetaData* MetaData = NewObject<UFieldSystemMetaData>();
		//MetaData->Type = EFieldPhysicsType::Field_ExternalClusterStrain;

		//// Apply the field to the Geometry Collection Component
		//GeometryCollectionComponent->ApplyPhysicsField(true, EFieldPhysicsType::Field_ExternalClusterStrain, nullptr, StrainField);
	//bHasBroken = true; // Mark the object as broken
}
void ABreakableObject::BeginPlay()
{
	Super::BeginPlay();
	MeshComponent->OnComponentHit.AddDynamic(this, &ABreakableObject::OnComponentHit);
	if (GeometryCollection)
	{
		GeometryCollection->OnChaosBreakEvent.AddDynamic(this, &ABreakableObject::HandleChaosBreakEvent);
	}
	InitializeFromDataAsset();



	// Bind Chaos Physics collision if available
  /*  if (UBodySetup* BodySetup = MeshComponent->GetBodySetup())
	{
		BodySetup->On OnChaosPhysicsCollision.AddDynamic(this, &ABreakableObject::OnChaosPhysicsCollision);
	}*/
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

	if (OtherActor && OtherActor != this)
	{
		UE_LOG(LogTemp, Warning, TEXT("hit  field working"));
		//Check if the other actor implements the Destructible interface
		/*if (OtherActor->GetClass()->ImplementsInterface(UManiDestructibleInterface::StaticClass()))
		{*/
			//IManiDestructibleInterface::Execute_BulletImpact(OtherActor, OtherComp, Hit, Hit.ImpactPoint, true, DefaultRadius);
		ApplyDestructionField(Hit.ImpactPoint);

		//}
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
	//if (DestructionFieldClass)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Destruction field working"));

	//	FActorSpawnParameters SpawnParams;
	//	SpawnParams.Owner = this;
	//	SpawnParams.Instigator = GetInstigator();

	//	if (!MasterField)
	//	{
	//		UGameplayStatics::BreakHitResult(HitInfo, HitInfo.ImpactPoint, HitInfo.ImpactNormal, Radius, 1, TArray<FHitResult>(), true);
	//		MasterField = GetWorld()->SpawnActor<AActor>(
	//			DestructionFieldClass,
	//			HitInfo.ImpactPoint,
	//			FRotator::ZeroRotator,
	//			SpawnParams
	//		);
	//	}
	//	// Spawn the destruction field
	//	//MasterField->FindFunction(TEXT("CalculateNoise"));
	//	UFunction* findTrigger = MasterField->FindFunction(TEXT("CE_Trigger"));
	//	if (findTrigger)
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("Function '%s' found successfully in class '%s'!"), *findTrigger->GetName(), *MasterField->GetClass()->GetName());
	//		MasterField->AllFunctions;
	//		MasterField->Al
	//		MasterField->ProcessEvent(findTrigger, nullptr);
	//	}
	//	UE_LOG(LogTemp, Warning, TEXT("still Destruction field working"));

	//	//UFunction* functio = MasterField->FindFunction(TEXT("ForceMultiplier"));
	//	//ProcessEvent(functio, nullptr);
	//	//ProcessEvent(Func, nullptr);
	//	if (MasterField)
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("aa Destruction field working"));


	//		//// Set parameters on the destruction field
	//		MasterField->SetTorqueMultiplier(TorqueMultiplier);
	//		MasterField->SetTorqueVectorOverride(TorqueVectorOverride);
	//		MasterField->SetRadialMagnitude(RadialMagnitude);
	//		MasterField->SetDirectionalMagnitude(DirectionalMagnitude);
	//		MasterField->SetDirectionalVelocityVector(DirectionalVelocityVector);
	//		
	//		//// Calculate and apply direction vector
	//		CalculateDirectionVector(HitInfo);

	//		//// Trigger the destruction logic
	//		DestructionField->TriggerDestruction();
	//	}
	//}
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
 