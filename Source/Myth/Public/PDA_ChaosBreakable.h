#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "PDA_ChaosBreakable.generated.h"

UCLASS(BlueprintType)
class MYTH_API UPDA_ChaosBreakable : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	/** Please add a function description */
	//UFUNCTION(BlueprintPure)
	//void GetDestructibleData(FS_ChaosDestructibleData& GC);
public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Materials")
	TMap<int32, UMaterialInterface*> OverrideMaterials;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Materials")
	bool SwitchMatOnBreak;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Chaos")
	TObjectPtr<UGeometryCollection> GeometryCollection;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Sounds")
	double BreakSoundFreqMin;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Sounds")
	double BreakSoundFreqMax;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Sounds")
	double CollisionSoundFreqMin;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Sounds")
	double CollisionSoundFreqMax;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Materials")
	TArray<int32> MatIndicestoSwitch;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Materials")
	TArray<UMaterialInterface*> SwitchtoMats;

	///** Please add a variable description */
	//UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "VFX")
	//TObjectPtr<UNiagaraSystem> BreakingVFX;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Sounds")
	TObjectPtr<USoundBase> BreakSound;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Sounds")
	TObjectPtr<USoundBase> BulletImpactSound;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Sounds")
	TObjectPtr<USoundBase> PiecesCollisionSound;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Chaos")
	bool RemovalonSleep;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Chaos")
	bool HasKinematicPieces;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Chaos")
	TArray<double> DamageThreshold;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Chaos")
	bool OverrideDamageThresholds;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Chaos") 
	double DamageRadius; 
	 
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Chaos")
	TObjectPtr<UPhysicalMaterial> PhysicalMaterial;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Chaos")
	TObjectPtr<UStaticMesh> OptionalStaticMesh; 
};

