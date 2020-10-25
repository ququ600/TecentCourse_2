// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TecentCourse_2 : ModuleRules
{
	public TecentCourse_2(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
