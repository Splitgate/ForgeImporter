// Copyright Adam (@portalwars2) 2025

using UnrealBuildTool;

public class ForgeImporter : ModuleRules
{
	public ForgeImporter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			});
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"UnrealEd",
				"LevelEditor",
				"ContentBrowser",
				"AssetRegistry",
				"Projects",
				"EditorStyle",
				"PropertyEditor",
				"DesktopPlatform",
				"Json",
				"JsonUtilities",
				"ForgeRuntime",
			});
	}
}
