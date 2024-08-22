#pragma once

#include "CoreMinimal.h"
#include "Field/FieldSystemActor.h"
#include "ABP_DestructionField.generated.h"

UCLASS(Blueprintable, BlueprintType)
class ABP_DestructionField : public AFieldSystemActor
{
    GENERATED_BODY()

public:
    ABP_DestructionField();

    UFUNCTION(BlueprintCallable, Category = "Custom")
    void TriggerCustomDestruction();

protected:
    // Custom variables and methods for extended behavior
    void ApplyCustomStrain();
    void ApplyCustomAngularMomentum();

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom")
    float CustomStrainMagnitude;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom")
    float CustomTorqueMultiplier;
};