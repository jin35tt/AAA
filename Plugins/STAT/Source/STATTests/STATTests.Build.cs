using UnrealBuildTool;

public class STATTests : ModuleRules
{
    public STATTests(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PrivateDependencyModuleNames.AddRange(new[] { "Core", "CoreUObject", "Engine", "FunctionalTesting", "STAT" });
    }
}
