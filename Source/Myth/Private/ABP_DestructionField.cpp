#include "ABP_DestructionField.h"
#include "Field/FieldSystemComponent.h"
#include "Components/SphereComponent.h"

ABP_DestructionField::ABP_DestructionField()
{
    // Initialize any custom values
    CustomStrainMagnitude = 150.0f;
    CustomTorqueMultiplier = 10.0f;
}

void ABP_DestructionField::TriggerCustomDestruction()
{
    // Apply custom strain or other forces as needed
    ApplyCustomStrain();
    ApplyCustomAngularMomentum();

    // Optionally destroy the actor after the custom operations
    Destroy();
}

void ABP_DestructionField::ApplyCustomStrain()
{
    // Custom logic for applying strain
    if (FieldSystemComponent)
    {
        FieldSystemComponent->ApplyPhysicsField(
            true, EFieldPhysicsType::Field_ExternalClusterStrain, nullptr,
            NewObject<URadialFalloff>(this, URadialFalloff::StaticClass())
            ->SetMagnitude(CustomStrainMagnitude)
            ->SetPosition(GetActorLocation())
        );
    }
}

void ABP_DestructionField::ApplyCustomAngularMomentum()
{
    // Custom logic for applying angular momentum
    if (FieldSystemComponent)
    {
        FieldSystemComponent->ApplyPhysicsField(
            true, EFieldPhysicsType::Field_AngularVelocity, nullptr,
            NewObject<UUniformVector>(this, UUniformVector::StaticClass())
            ->SetVector(FVector::UpVector * CustomTorqueMultiplier)
        );
    }
}
