using UnrealBuildTool;
 
public class ForgeRuntime : ModuleRules
{
	public ForgeRuntime(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine"});
 
		PublicIncludePaths.AddRange(new string[] {"ForgeRuntime/Public"});
		PrivateIncludePaths.AddRange(new string[] {"ForgeRuntime/Private"});
	}
}