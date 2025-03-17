// Copyright Adam (@portalwars2) 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ForgeInterface.generated.h"

UINTERFACE(MinimalAPI)
class UForgeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Empty interface purely for iterating actors to find each forge actor
 */
class FORGERUNTIME_API IForgeInterface
{
	GENERATED_BODY()

public:
};
