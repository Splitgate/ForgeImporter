// Copyright Adam (@portalwars2) 2025

#pragma once

#include "CoreMinimal.h"
#include "EdMode.h"

#include "Toolkits/BaseToolkit.h"
#include "InputState.h"
#include "InteractiveToolManager.h"

#include "ForgeEditorObject.h"

class FForgeImporterEdMode : public FEdMode
{
public:
	const static FEditorModeID EM_ForgeImporterEdModeId;
public:

#if (ENGINE_MAJOR_VERSION < 5)
	UForgeImportEditorObject* UIImportSettings;
	UForgeExportEditorObject* UIExportSettings;
#else
	TObjectPtr<UForgeImportEditorObject> UIImportSettings;
	TObjectPtr<UForgeExportEditorObject> UIExportSettings;
#endif

	FForgeImporterEdMode();
	virtual ~FForgeImporterEdMode();

	// FEdMode interface
	virtual void Enter() override;
	virtual void Exit() override;
	//virtual void Tick(FEditorViewportClient* ViewportClient, float DeltaTime) override;
	//virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI) override;
	//virtual void ActorSelectionChangeNotify() override;
	bool UsesToolkits() const override;
	// End of FEdMode interface

	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;

	// Delegates
	void OnPropertyChanged(const FPropertyChangedEvent& PropertyChangedEvent);

	// Functions
	bool IsValidMap(TSharedPtr<FJsonObject> GiftedJSON) const;

	// Return the datatable of valid interactables
	UDataTable* GetInteractables() const;

	// -1 if not found else valid
	int FindForgeInteractable(UObject* ClassOrMesh) const;

	// -1 if not found else found
	int GetInteractableId(UObject* ClassOrMesh) const;

	// Bindings
	bool CanLoadMap() const;
	bool CanExportMap() const;

	FReply AttemptLoadMap();
	FReply AttemptExportMap();
};
