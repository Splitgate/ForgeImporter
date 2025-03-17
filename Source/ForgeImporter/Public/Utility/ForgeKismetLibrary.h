// Copyright Adam (@portalwars2) 2025

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ForgeKismetLibrary.generated.h"

/**
 * Editor utilities
 */
UCLASS()
class FORGEIMPORTER_API UForgeKismetLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Forge")
	static void Ed_CreateForgeActor(const FString& Name, const FString& Path, UStaticMesh* NewMesh);

	UFUNCTION(BlueprintPure, Category = "Forge")
	static UBlueprint* GetBlueprintAsset(UObject* Object);

	UFUNCTION(BlueprintPure, Category = "Forge")
	static TArray<UActorComponent*> GetBlueprintTemplates(UBlueprint* Blueprint);

	UFUNCTION(BlueprintPure, Category = "Forge")
	static UActorComponent* FindComponent(UBlueprint* Blueprint, FName TemplateName);

	UFUNCTION(BlueprintPure, Category = "Forge")
	static UClass* GetGeneratedClass(UBlueprint* BlueprintObject);

	UFUNCTION(BlueprintPure, Category = "Forge")
	static UObject* GetDefaultObject(UClass* Class);
};
