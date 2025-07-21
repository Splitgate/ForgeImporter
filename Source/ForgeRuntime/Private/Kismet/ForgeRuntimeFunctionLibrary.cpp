// Copyright Adam (@portalwars2) 2025

#include "Kismet/ForgeRuntimeFunctionLibrary.h"
#include "GameFramework/PlayerStart.h"

UArrowComponent* UForgeRuntimeFunctionLibrary::GetPlayerStartArrow(APlayerStart* PlayerStart)
{
#if WITH_EDITORONLY_DATA
    if (PlayerStart)
        return PlayerStart->GetArrowComponent();
#endif

    return nullptr;
}

UCapsuleComponent* UForgeRuntimeFunctionLibrary::GetPlayerStartCapsule(APlayerStart* PlayerStart)
{
    if (PlayerStart)
        return PlayerStart->GetCapsuleComponent();

    return nullptr;
}
