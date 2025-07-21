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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interactable")
	int ID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interactable")
	TSoftClassPtr<AActor> BuildClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interactable")
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

	friend FArchive& operator<<(FArchive& Ar, FInteractableSave& ObjectToSerialize)
	{
		Ar << ObjectToSerialize.id;
		Ar << ObjectToSerialize.transform;

		return Ar;
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

	friend FArchive& operator<<(FArchive& Ar, FForgeMapData& ObjectToSerialize) 
	{
		Ar << ObjectToSerialize.mapID;
		Ar << ObjectToSerialize.timeStamp;
		Ar << ObjectToSerialize.sharingCode;
		Ar << ObjectToSerialize.name;
		Ar << ObjectToSerialize.displayName;
		Ar << ObjectToSerialize.interactablesSaveData;

		return Ar;
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
