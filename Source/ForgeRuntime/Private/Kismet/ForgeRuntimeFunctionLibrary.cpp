// Copyright Adam (@portalwars2) 2025

#include "Kismet/ForgeRuntimeFunctionLibrary.h"
#include "GameFramework/PlayerStart.h"

UArrowComponent* UForgeRuntimeFunctionLibrary::GetPlayerStartArrow(APlayerStart* PlayerStart)
{
    if (PlayerStart)
        return PlayerStart->GetArrowComponent();

    return nullptr;
}

UCapsuleComponent* UForgeRuntimeFunctionLibrary::GetPlayerStartCapsule(APlayerStart* PlayerStart)
{
    if (PlayerStart)
        return PlayerStart->GetCapsuleComponent();

    return nullptr;
}
