// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Project_DT : ModuleRules
{
	public Project_DT(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Niagara", "NavigationSystem" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

	}
}
