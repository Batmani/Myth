// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructorFieldSystem.h"
#include "Field/FieldSystemComponent.h"
#include "Field/FieldSystemNodes.h"

ADestructorFieldSystem::ADestructorFieldSystem()
{
    PrimaryActorTick.bCanEverTick = true;

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
    SphereRadius = 200.0f;
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
    FVector SphereLoc = CollisionSphere->GetComponentLocation();
    URadialFalloff* RadialFalloff = NewObject<URadialFalloff>();

    UFieldNodeBase* RadialFalloffNode = RadialFalloff->SetRadialFalloff(
        FalloffMagnitude,
        0.f,
        1.f,
        0.f,
        CollisionSphere->GetScaledSphereRadius(),
        SphereLoc,
        EFieldFalloffType::Field_FallOff_None
    );

    //FieldSystem->ApplyPhysicsField(true, EFieldPhysicsType::Field_ExternalClusterStrain, nullptr, RadialFalloff);

    URadialVector* RadialVector = NewObject<URadialVector>();
    
    UFieldNodeBase* RadialVectorNode = RadialVector->SetRadialVector(VectorMagnitude, SphereLoc);

	UCullingField* CullingField = NewObject<UCullingField>();
    UFieldNodeBase* CullingFieldNode = CullingField->SetCullingField(RadialFalloffNode, RadialVectorNode, Field_Culling_Inside);

    // Apply the CullingField to the FieldSystemComponent
    FieldSystem->ApplyPhysicsField(true, Field_ExternalClusterStrain, nullptr, CullingFieldNode);

	Destroy();
}

void ADestructorFieldSystem::SpawnAtLocation(const FVector& Location)
{
    SetActorLocation(Location);
    Explode();
}