//Copyright Geodesic 2020

using UnrealBuildTool;

public class GeodesicChat : ModuleRules
{
    public GeodesicChat(ReadOnlyTargetRules target) : base(target)
    {
        PrivateIncludePaths.AddRange(new string[] { "GeodesicChat/Private" });

        PublicDependencyModuleNames.AddRange(new string[] { "Engine", "Core", "CoreUObject", "Slate", "SlateCore", "UnrealEd", "EditorStyle", "Projects" });
        PrivateDependencyModuleNames.AddRange(new string[] { "InputCore", "SlateCore" });

        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
    }
}
