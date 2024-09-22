// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class UEFormat : ModuleRules
{
	public UEFormat(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        DynamicallyLoadedModuleNames.AddRange(
            new string[] {
                "AssetTools",
                "MainFrame",
            });

        string PluginsPath = Path.GetFullPath(Target.RelativeEnginePath) + "Plugins/";
        PublicIncludePaths.AddRange(
            new string[]
            {
                PluginsPath + "Experimental/Animation/SkeletalMeshModelingTools/Source/SkeletalMeshModelingTools/Private"
                // ... add public include paths required here ...
            }
        );
        PrivateIncludePaths.AddRange(
            new string[]
            {
                // ... add other private include paths required here ...
            }
        );
        PublicIncludePathModuleNames.AddRange(
            new string[]
            {
            }
        );
        PrivateIncludePathModuleNames.AddRange(
            new string[]
            {
            }
        );
        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "MeshDescription",
                "StaticMeshDescription",
                "SkeletalMeshModelingTools",

                //BPFL
                "Json",
                "JsonUtilities",
              //  "DeveloperSettings"
                // ... add other public dependencies that you statically link with here ...
            }
        );
        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "SkeletalMeshModelingTools",
                "ContentBrowser",
                "Core",
                "CoreUObject",
                "DesktopWidgets",
                "EditorStyle",
                "AnimationModifiers",
                "Engine",
                "EditorScriptingUtilities",
                "InputCore",
                "Projects",
                "UnrealEd",
                "Slate",
                "MeshBuilder",
                "SlateCore",
                "UnrealEd",
                "ApplicationCore",
                "MeshDescription",
                "StaticMeshDescription",
                "ToolWidgets",
                "RenderCore"
                // ... add private dependencies that you statically link with here ...	
            }
        );
    }
}