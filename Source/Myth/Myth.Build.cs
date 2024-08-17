// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Myth : ModuleRules
{
	public Myth(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Chaos", "ChaosSolverEngine", "GeometryCollectionEngine" });
	}
}
