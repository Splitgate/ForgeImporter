// Copyright Adam (@portalwars2) 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ForgeInterface.h"

#include "ForgeStaticMeshInteractable.generated.h"

UCLASS()
class FORGERUNTIME_API AForgeStaticMeshInteractable : public AActor, public IForgeInterface
{
	GENERATED_BODY()
	
public:	
	AForgeStaticMeshInteractable();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Static Mesh")
	UStaticMesh* ForgeMesh;

	void UpdateForgeMesh(UStaticMesh* NewMesh);

protected:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Static Mesh")
	UStaticMeshComponent* MeshComponent;

	virtual void OnConstruction(const FTransform& Transform);

#if WITH_EDITOR
	void OnObjectPropertyChanged(UObject* ObjectBeingModified, FPropertyChangedEvent& PropertyChangedEvent);
#endif
};
