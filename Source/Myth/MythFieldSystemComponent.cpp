#include "MythFieldSystemComponent.h"
#include "Field/FieldSystemNodes.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

UMythFieldSystemComponent::UMythFieldSystemComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    FieldSystemComponent = CreateDefaultSubobject<UMythFieldSystemComponent>(TEXT("FieldSystemComponent"));
}

void UMythFieldSystemComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UMythFieldSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UMythFieldSystemComponent::ApplyDamageToGeometryCollection(float Damage, float Radius, const FVector& Position)
{
    if (!FieldSystemComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("FieldSystemComponent is null in UMythFieldSystemComponent"));
        return;
    }

    // Create a radial vector field
    URadialVector* RadialVector = URadialVector::RadialVector(Radius, Position);

    // Create a radial falloff field
    URadialFalloff* RadialFalloff = URadialFalloff::RadialFalloff(Damage, Radius, 1.0f, 0.0f, Position, EFieldFalloffType::Field_FallOff_None);

    // Combine the fields
    UOperatorField* DamageField = UOperatorField::OperatorField(RadialFalloff, RadialVector, EFieldOperationType::Field_Multiply);

    // Apply the field to cause damage
    FieldSystemComponent->ApplyPhysicsField(true, EFieldPhysicsType::Field_ExternalClusterStrain, nullptr, DamageField);

    // Find all GeometryCollectionComponents within the field
    TArray<UGeometryCollectionComponent*> GeometryCollectionComponents;
    GetOwner()->GetWorld()->GetComponentsByClass(UGeometryCollectionComponent::StaticClass(), GeometryCollectionComponents);

    for (UGeometryCollectionComponent* GeometryCollectionComponent : GeometryCollectionComponents)
    {
        if (GeometryCollectionComponent && FVector::Dist(GeometryCollectionComponent->GetComponentLocation(), Position) <= Radius)
        {
            // Apply damage to each GeometryCollectionComponent within the field
            GeometryCollectionComponent->ReceiveComponentDamage(Damage, FDamageEvent(), nullptr, GetOwner());
        }
    }
}