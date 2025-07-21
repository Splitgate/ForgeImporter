// Copyright Adam (@portalwars2) 2025

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "ForgeDeveloperSettings.generated.h"

/**
 * 
 */
UCLASS(Config=ForgeSettings)
class FORGEIMPORTER_API UForgeDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UForgeDeveloperSettings(const FObjectInitializer& ObjectInitializer);

#if WITH_EDITOR
	virtual FText GetSectionText() const override;
	virtual FText GetSectionDescription() const override;
#endif

	// Table to use to find each interactable the forge maps contain.
	UPROPERTY(EditAnywhere, Config, Category = "Interactables", meta = (RequiredAssetDataTags = "RowStructure=ForgeInteractables"))
	TSoftObjectPtr<UDataTable> InteractablesTable;
	// NOTE: UE5 Requires full structure path to the row struct but since the asset is from 4.26 it does not require it.

	// Table to use to find each interactable the forge maps contain.
	UPROPERTY(EditAnywhere, Config, Category = "Export")
	FDirectoryPath ExportPath;
};
