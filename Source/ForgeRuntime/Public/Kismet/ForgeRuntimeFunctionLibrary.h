// Copyright Adam (@portalwars2) 2025

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ForgeRuntimeFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class FORGERUNTIME_API UForgeRuntimeFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	// Helper for UE5 deprecation of the playerstart arrow component variable
	UFUNCTION(BlueprintPure, Category = "UE Compatibility")
	static class UArrowComponent* GetPlayerStartArrow(class APlayerStart* PlayerStart);

	// Helper for UE5 deprecation of the playerstart capsule component variable
	UFUNCTION(BlueprintPure, Category = "UE Compatibility")
	static class UCapsuleComponent* GetPlayerStartCapsule(class APlayerStart* PlayerStart);
};
