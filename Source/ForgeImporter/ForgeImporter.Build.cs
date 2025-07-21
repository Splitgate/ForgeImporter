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
				#if UE_5_0_OR_LATER
					"EditorFramework",
				#endif
				"LevelEditor",
				"ContentBrowser",
				"AssetRegistry",
				"Projects",
				"EditorStyle",
				"PropertyEditor",
				"DesktopPlatform",
				"DeveloperSettings",
				"Json",
				"JsonUtilities",
				"ForgeRuntime",
			});

	}
}
