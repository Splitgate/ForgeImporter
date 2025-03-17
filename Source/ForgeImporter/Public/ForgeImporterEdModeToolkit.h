// Copyright Adam (@portalwars2) 2025

#pragma once

#include "CoreMinimal.h"
#include "ForgeImporterEdMode.h"
#include "Toolkits/BaseToolkit.h"

class FForgeImporterEdModeToolkit : public FModeToolkit
{
public:

	FForgeImporterEdModeToolkit();
	
	/** FModeToolkit interface */
	virtual void Init(const TSharedPtr<IToolkitHost>& InitToolkitHost) override;

	/** IToolkit interface */
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual class FEdMode* GetEditorMode() const override;
	virtual TSharedPtr<class SWidget> GetInlineContent() const override { return ToolkitWidget; }

	virtual FForgeImporterEdMode* GetForgeEditorMode() const;

	void StartTool();
	TSharedPtr<IDetailsView> GetDetailsView() const { return DetailsView; };

private:

	TSharedPtr<SWidget> ToolkitWidget;
	TSharedPtr<IDetailsView> DetailsView;
};
