using System;
using UnrealBuildTool;

public class STATTests : ModuleRules
{
    public STATTests(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        if (!Target.bBuildEditor)
        {
            throw new BuildException("STATTests module requires editor build.");
        }

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core", "CoreUObject", "Engine", "STAT"
        });

        PrivateDependencyModuleNames.AddRange(new[]
        {
            "UnrealEd", "Slate", "SlateCore", "Projects", "DeveloperSettings", "FunctionalTesting", "AutomationController"
        });
    }
}
