// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructorFieldSystem.h"
#include "Field/FieldSystemComponent.h"
#include "Field/FieldSystemNodes.h"

ADestructorFieldSystem::ADestructorFieldSystem()
{
	static ConstructorHelpers::FClassFinder<AActor> DestructionFieldBPClass(TEXT("/Game/NextGenDestruction/Blueprints/Actors/BP_DestructionField.BP_DestructionField_C"));
	if (DestructionFieldBPClass.Succeeded())
	{
		DestructionFieldClass = DestructionFieldBPClass.Class;
		UE_LOG(LogTemp, Warning, TEXT("Field class worked!"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Field class nope "));

	}

	PrimaryActorTick.bCanEverTick = true;
	SphereRadius = 100.0f;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetSphereRadius(SphereRadius, true);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndProbe);
	CollisionSphere->SetupAttachment(RootComponent);
	RootComponent = CollisionSphere;
	CollisionSphere->SetCollisionProfileName(TEXT("NoCollision"));
	CollisionSphere->SetGenerateOverlapEvents(false);

	FieldSystem = CreateDefaultSubobject<UFieldSystemComponent>(TEXT("FieldSystem"));
	FieldSystem->SetupAttachment(RootComponent);

	FalloffMagnitude = 1000000.0f;
	VectorMagnitude = 10000.0f;
}

void ADestructorFieldSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADestructorFieldSystem::BeginPlay()
{
	Super::BeginPlay();
	CollisionSphere->SetSphereRadius(SphereRadius);

}

void ADestructorFieldSystem::Explode()
{



	if (DestructionFieldClass)
	{
		MasterField = GetWorld()->SpawnActor<AActor>(DestructionFieldClass, GetActorLocation(), FRotator::ZeroRotator);
		if (MasterField)
		{
			UE_LOG(LogTemp, Warning, TEXT("master class worked!"));
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Field class nope "));

		}

	}
	struct FDynamicArgs
	{
		float Magnitude;                             // Equivalent to Parms.Magnitude
		EFieldFalloffType FalloffType;               // Equivalent to Parms.FalloffType
		FVector2D Param_FalloffMinMax;               // Equivalent to Parms.Param_FalloffMinMax
		UFieldNodeBase* OperatorField = nullptr;     // Equivalent to Parms.OperatorField
		UCullingField* CullingField = nullptr;			  // Equivalent to Parms.CullingField
	};

	FDynamicArgs Args;

	// Assign the parameters
	Args.Magnitude = FalloffMagnitude;  // Set the magnitude
	Args.FalloffType = Field_Falloff_Linear;  // Set the falloff type
	Args.Param_FalloffMinMax = FVector2D(0, 1);  // Set the falloff min-max vector
	Args.OperatorField = nullptr;  // Set the operator field


	UFunction* findTrigger = MasterField->FindFunction(TEXT("FalloffAndCullSwitch_Main"));
	if (findTrigger)
	{
		UE_LOG(LogTemp, Warning, TEXT("Function '%s' found successfully in class '%s'!"), *findTrigger->GetName(), *MasterField->GetClass()->GetName());
		FStructOnScope FuncParam(findTrigger);
		void* StructMemory = FuncParam.GetStructMemory();

		// Set 'Magnitude' property
		FFloatProperty* MagnitudeProp = FindFProperty<FFloatProperty>(findTrigger, TEXT("Magnitude"));
		if (MagnitudeProp)
		{
			MagnitudeProp->SetPropertyValue_InContainer(StructMemory, Args.Magnitude);
		}

		// Set 'FalloffType' property
		FByteProperty* FalloffTypeProp = FindFProperty<FByteProperty>(findTrigger, TEXT("FalloffType"));
		if (FalloffTypeProp)
		{
			FalloffTypeProp->SetPropertyValue_InContainer(StructMemory, static_cast<uint8>(Args.FalloffType));
		}

		// Set 'Param_FalloffMinMax' property
		FStructProperty* FalloffMinMaxProp = FindFProperty<FStructProperty>(findTrigger, TEXT("Param_FalloffMinMax"));
		if (FalloffMinMaxProp)
		{
			FVector2D* FalloffMinMaxPtr = FalloffMinMaxProp->ContainerPtrToValuePtr<FVector2D>(StructMemory);
			if (FalloffMinMaxPtr)
			{
				*FalloffMinMaxPtr = Args.Param_FalloffMinMax;
			}
		}

		// Set 'OperatorField' property
		FObjectProperty* OperatorFieldProp = FindFProperty<FObjectProperty>(findTrigger, TEXT("OperatorField"));
		if (OperatorFieldProp)
		{
			OperatorFieldProp->SetObjectPropertyValue_InContainer(StructMemory, Args.OperatorField);
		}

		// Set 'CullingField' property
		FObjectProperty* CullingFieldProp = FindFProperty<FObjectProperty>(findTrigger, TEXT("CullingField"));
		if (CullingFieldProp)
		{
			CullingFieldProp->SetObjectPropertyValue_InContainer(StructMemory, Args.CullingField);
		}

		// Call the function
		MasterField->ProcessEvent(findTrigger, StructMemory);
		FObjectProperty* RetCullingFieldProp = CastField<FObjectProperty>(findTrigger->GetReturnProperty());
		UCullingField* ReturnedCullingField = nullptr;

		if (RetCullingFieldProp)
		{
			ReturnedCullingField = Cast<UCullingField>(RetCullingFieldProp->GetObjectPropertyValue_InContainer(StructMemory));
		}
		// Check if the returned CullingField is valid and log it
		if (ReturnedCullingField)
		{
			FString CullingFieldName = ReturnedCullingField->Culling ? ReturnedCullingField->Culling->GetName() : TEXT("None");
			FString FieldName = ReturnedCullingField->Field ? ReturnedCullingField->Field->GetName() : TEXT("None");
			FString CullingOperation = StaticEnum<EFieldCullingOperationType>()->GetNameStringByValue((int64)ReturnedCullingField->Operation);

			UE_LOG(LogTemp, Warning, TEXT("Returned UCullingField Info: CullingField: %s, Field: %s, Operation: %s"),
				*CullingFieldName,
				*FieldName,
				*CullingOperation);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Returned UCullingField is null."));
		}
	}
	//	/*MasterField->ProcessEvent(findTrigger, &Args);*/
	//	if (Args.CullingField != nullptr)
	//	{
	//		//FString CullingOperation = StaticEnum<EFieldCullingOperationType>()->GetNameStringByValue((int64)Args.CullingField->Operation);
	//		UE_LOG(LogTemp, Warning, TEXT("cull not the null "));
	//		FString CullingFieldName = Args.CullingField->Culling ? Args.CullingField->Culling->GetName() : TEXT("None");
	//		FString FieldName = Args.CullingField->Field ? Args.CullingField->Field->GetName() : TEXT("None");
	//		FString CullingOperation = StaticEnum<EFieldCullingOperationType>()->GetNameStringByValue((int64)Args.CullingField->Operation);
	//		  
	//		UE_LOG(LogTemp, Warning, TEXT("UCullingField Info: CullingField: %s, Field: %s, Operation: %s"),
	//			*CullingFieldName,
	//			*FieldName,
	//			*CullingOperation);
	//		/*		 UE_LOG(LogTemp, Warning, TEXT("UCullingField Info:   CullingOperation: %s"),
	//					 *CullingOperation); */
	//	}
	//}
	//FVector SphereLoc = CollisionSphere->GetComponentLocation();
	//URadialFalloff* RadialFalloff = NewObject<URadialFalloff>();

	//UFieldNodeBase* RadialFalloffNode = RadialFalloff->SetRadialFalloff(
	//	FalloffMagnitude,
	//	0.f,
	//	1.f,
	//	0.f,
	//	CollisionSphere->GetScaledSphereRadius(),
	//	SphereLoc,
	//	EFieldFalloffType::Field_Falloff_Linear
	//);

	////FieldSystem->ApplyPhysicsField(true, EFieldPhysicsType::Field_ExternalClusterStrain, nullptr, RadialFalloff);

	//URadialVector* RadialVector = NewObject<URadialVector>();

	//UFieldNodeBase* RadialVectorNode = RadialVector->SetRadialVector(VectorMagnitude, SphereLoc); 
	// 
	//UCullingField* CullingField = NewObject<UCullingField>();
	//UFieldNodeBase* CullingFieldNode = CullingField->SetCullingField(RadialFalloffNode, RadialVectorNode, Field_Culling_Outside);

	//// Apply the CullingField to the FieldSystemComponent
	//FieldSystem->ApplyPhysicsField(true, Field_ExternalClusterStrain, nullptr, CullingFieldNode);
	Destroy();
}

void ADestructorFieldSystem::SpawnAtLocation(const FVector& Location)
{
	SetActorLocation(Location);
	//Explode();
}