// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Right4Dead : ModuleRules
{
	public Right4Dead(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "NavigationSystem", "AIModule", "GameplayTasks", "UMG" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		PublicIncludePaths.AddRange(new string[] {
			"Right4Dead/Public/Survivor",
			"Right4Dead/Public/Zombies",
			"Right4Dead/Public/Common",
			"Right4Dead/Public/Common/UI"
		});
		
		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
