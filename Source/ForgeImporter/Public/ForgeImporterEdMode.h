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
	UForgeImportEditorObject* UIImportSettings;
	UForgeExportEditorObject* UIExportSettings;

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

	// Bindings
	bool CanLoadMap() const;
	FReply AttemptLoadMap();
};
