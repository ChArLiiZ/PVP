// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PVP : ModuleRules
{
	public PVP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Niagara", "UMG", "GameplayAbilities", "GameplayTasks", "GameplayTags", "OnlineSubsystemEOS", "OnlineSubsystem", "OnlineSubsystemUtils" });
	}
}
