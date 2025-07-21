// Copyright Adam (@portalwars2) 2025

#include "World/ForgeStaticMeshInteractable.h"
#include "Components/StaticMeshComponent.h"

AForgeStaticMeshInteractable::AForgeStaticMeshInteractable()
{
#if WITH_EDITOR
	FCoreUObjectDelegates::OnObjectPropertyChanged.AddUObject(this, &AForgeStaticMeshInteractable::OnObjectPropertyChanged);
#endif

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	SetRootComponent(RootComponent);
}

void AForgeStaticMeshInteractable::UpdateForgeMesh(UStaticMesh* NewMesh)
{
	ForgeMesh = NewMesh;

	if (MeshComponent)
		MeshComponent->SetStaticMesh(ForgeMesh);
}

void AForgeStaticMeshInteractable::OnConstruction(const FTransform& Transform)
{
	UpdateForgeMesh(ForgeMesh);
}

#if WITH_EDITOR
void AForgeStaticMeshInteractable::OnObjectPropertyChanged(UObject* ObjectBeingModified, FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.MemberProperty)
	{
		if (PropertyChangedEvent.MemberProperty->GetFName() == GET_MEMBER_NAME_CHECKED(AForgeStaticMeshInteractable, ForgeMesh))
		{
			UpdateForgeMesh(ForgeMesh);
		}
	}
}
#endif