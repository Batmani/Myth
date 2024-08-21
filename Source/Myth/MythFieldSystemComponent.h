#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Field/FieldSystemActor.h"
#include "Field/FieldSystemComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "MythFieldSystemComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class YOURPROJECT_API UMythFieldSystemComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UMythFieldSystemComponent();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // Apply damage to the GeometryCollectionComponent within the field
    UFUNCTION(BlueprintCallable, Category = "FieldSystem")
    void ApplyDamageToGeometryCollection(float Damage, float Radius, const FVector& Position);

private:
    UPROPERTY(EditAnywhere, Category = "FieldSystem")
    UFieldSystemComponent* FieldSystemComponent;

    UPROPERTY(EditAnywhere, Category = "FieldSystem")
    float FieldRadius = 500.0f;
};