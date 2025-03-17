// Copyright Adam (@portalwars2) 2025

#pragma once

// Core Includes
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

// Slate includes
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"

#include "ForgeImporter.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogForge, Log, All);

USTRUCT(BlueprintType)
struct FForgeInteractables : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int ID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftClassPtr<AActor> BuildClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMesh* StaticMesh;
};

USTRUCT()
struct FInteractableSave
{
	GENERATED_BODY()

	UPROPERTY()
	int id;

	UPROPERTY()
	FTransform transform;

	FInteractableSave()
	{
		id = 0;
		transform = FTransform();
	}
};

USTRUCT()
struct FForgeMapData
{
	GENERATED_BODY()

	UPROPERTY()
	FString mapID;

	UPROPERTY()
	FString timeStamp;

	UPROPERTY()
	FString sharingCode;

	UPROPERTY()
	FString name;

	UPROPERTY()
	FString displayName;

	UPROPERTY()
	TArray<FInteractableSave> interactablesSaveData;

	FForgeMapData()
	{
		mapID = TEXT("");
		timeStamp = TEXT("");
		sharingCode = TEXT("");
		name = TEXT("");
		displayName = TEXT("");
	}
};

class FForgeImporterModule : public IModuleInterface
{
public:

	/** Custom slate styleset */
	TSharedPtr<FSlateStyleSet> Style;

	static FName GetStyleSetName();

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:

	// Setup custom slate styles
	void InitializeStyle();
};
