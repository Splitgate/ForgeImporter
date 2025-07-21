// Copyright Adam (@portalwars2) 2025

#include "Settings/ForgeDeveloperSettings.h"

#define LOCTEXT_NAMESPACE "ForgeImporter"

UForgeDeveloperSettings::UForgeDeveloperSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CategoryName = TEXT("Plugins");
	SectionName = TEXT("Forge Importer");

	InteractablesTable = LoadObject<UDataTable>(GEngine, TEXT("DataTable'/ForgeImporter/ForgeInteractables.ForgeInteractables'"));
	ExportPath = FDirectoryPath();
	ExportPath.Path = (FPaths::ProjectDir() + TEXT("ForgeMaps"));
}

#if WITH_EDITOR

FText UForgeDeveloperSettings::GetSectionText() const
{
	return LOCTEXT("SettingsText", "Forge Importer");
}

FText UForgeDeveloperSettings::GetSectionDescription() const
{
	return LOCTEXT("SettingsDescription", "Settings for the Forge Map Importer, most settings are per import rather than project wide.");
}

#endif // WITH_EDITOR

#undef LOCTEXT_NAMESPACE