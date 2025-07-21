// Copyright Adam (@portalwars2) 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ForgeEditorObject.generated.h"

/**
 * Import settings for Forge
 */
UCLASS(MinimalAPI)
class UForgeImportEditorObject : public UObject
{
	GENERATED_BODY()
	
public:

	// Path to the forge map data
	UPROPERTY(EditDefaultsOnly, Category = "Import Settings", transient, meta = (FilePathFilter = "Forge Map (*.json)|*.json|Forge Map (*.forge)|*.forge"))
	FFilePath ImportPath;

	// If true this will only import the meshes and no blueprints (i.e. player starts, weapon spawners).
	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "Import Only Static Meshes"), Category = "Import Settings")
	bool ImportStaticMeshesOnly = false;

	TSharedPtr<FJsonObject> ForgeJSON;
};

UENUM(BlueprintType)
enum class EForgeMap : uint8
{
	Forge_Flat_Earth	= 0		UMETA(DisplayName="Flat Earth"),
	Forge_Island		= 1		UMETA(DisplayName="Wet Ocean")
};

UENUM(BlueprintType)
enum class EForgeExportType : uint8
{
	JSON				= 0		UMETA(DisplayName = "JSON"),
	Forge				= 1		UMETA(DisplayName = "Forge Format")
};

/**
 * Export settings for Forge
 */
UCLASS(MinimalAPI)
class UForgeExportEditorObject : public UObject
{
	GENERATED_BODY()

public:

	// Name to display in UI
	UPROPERTY(EditDefaultsOnly, Category="Export Settings", transient)
	FString DisplayName = TEXT("My Forge Map");

	// Map we are building off of (All others work but these are the only 'legal' ones)
	UPROPERTY(EditDefaultsOnly, Category = "Export Settings", transient)
	EForgeMap Map;

	// Ignore
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Export Settings", DisplayName = "Internal Map Name", transient)
	FString BaseMap = TEXT("Forge_Island");

	// Type to export this map to.
	UPROPERTY(EditDefaultsOnly, Category = "Export Settings", DisplayName = "Export Format", transient)
	EForgeExportType ExportType;
};