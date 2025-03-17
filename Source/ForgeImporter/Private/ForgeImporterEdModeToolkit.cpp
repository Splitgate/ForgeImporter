// Copyright Adam (@portalwars2) 2025

#include "ForgeImporterEdModeToolkit.h"
#include "ForgeImporterEdMode.h"

#include "ForgeImporter.h"

#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SExpandableArea.h"
#include "GameFramework/WorldSettings.h"

#include "JsonObjectConverter.h"
#include "Misc/ScopedSlowTask.h"

#include "EditorModeManager.h"
#include "Misc/FileHelper.h"
#include "DesktopPlatformModule.h"
#include "Engine/StaticMeshActor.h"

#define LOCTEXT_NAMESPACE "FForgeImporterEdModeToolkit"

FForgeImporterEdModeToolkit::FForgeImporterEdModeToolkit()
{
}

void FForgeImporterEdModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost)
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FDetailsViewArgs DetailsViewArgs;
	{
		DetailsViewArgs.bAllowSearch = false;
		DetailsViewArgs.bHideSelectionTip = true;
		DetailsViewArgs.bLockable = false;
		DetailsViewArgs.bSearchInitialKeyFocus = true;
		DetailsViewArgs.bUpdatesFromSelection = false;
		DetailsViewArgs.bShowOptions = false;
		DetailsViewArgs.bShowModifiedPropertiesOption = false;
		DetailsViewArgs.bShowActorLabel = false;
		DetailsViewArgs.bCustomNameAreaLocation = true;
		DetailsViewArgs.bCustomFilterAreaLocation = true;
		DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
		DetailsViewArgs.bAllowMultipleTopLevelObjects = true;
		DetailsViewArgs.bShowScrollBar = false;
	}

	DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	StartTool(); // adds the settings objects we use

	FForgeImporterEdMode* EditorMode = GetForgeEditorMode();

	SAssignNew(ToolkitWidget, SBorder)
		.HAlign(HAlign_Fill)
		.BorderImage(FEditorStyle::GetBrush("NoBorder"))
		.Padding(1)
		[
			SNew( SVerticalBox )
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign( HAlign_Fill )
			.Padding(1)
			[
				SNew(SExpandableArea)
				.HeaderPadding(FMargin(2.0f))
				.Padding(FMargin(10.f))
				.BorderImage(FEditorStyle::Get().GetBrush("DetailsView.CategoryTop"))
				.BorderBackgroundColor(FLinearColor(.6, .6, .6, 1.0f))
				.BodyBorderBackgroundColor(FLinearColor::Transparent)
				.AreaTitleFont(FEditorStyle::Get().GetFontStyle("EditorModesPanel.CategoryFontStyle"))
				.BodyContent()
				[
					SNew(STextBlock)
					.Text(LOCTEXT("ForgeToolMessage", "This tool allows you to import Splitgate 1 Forge maps via JSON save from https://github.com/Splitgate/SplitgateForgeBackup."))
					.Font(FEditorStyle::Get().GetFontStyle("EditorModesPanel.ToolDescriptionFont"))
					.AutoWrapText(true)
				]
				.HeaderContent()
				[
					SNew(STextBlock)
					.Text(LOCTEXT("ForgeDisplay", "Forge Importer"))
					.Justification(ETextJustify::Center)
					.Font(FEditorStyle::Get().GetFontStyle("EditorModesPanel.CategoryFontStyle"))
				]
			]

			+ SVerticalBox::Slot()
			.FillHeight( 1.0f )
			.Padding( 1 )
			[
				SNew( SVerticalBox )
				+SVerticalBox::Slot()
				.AutoHeight()
				[
					DetailsView->AsShared()
				]
			]

			+ SVerticalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Bottom)
			.Padding(5, 0, 5, 20)
			[
				SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
				.Padding(0, 0, 5, 0)
				[
					SNew(SButton)
						.Text(LOCTEXT("ForgeToolExport", "Export"))
						.TextStyle(FEditorStyle::Get(), "LargeText")
						.ButtonStyle(FEditorStyle::Get(), "FlatButton.Success")
						.ForegroundColor(FLinearColor::White)
						.ContentPadding(FMargin(6, 6))
						.IsEnabled(false)
						.ToolTip(SNew(SToolTip)
							.Text(LOCTEXT("ForgeToolExport_Tip", "Exporting Not Added")))
						.HAlign(HAlign_Center)
						.VAlign(VAlign_Center)
						//.OnClicked_Static(&Locals::OnLoadButtonClick);
				]

				+ SHorizontalBox::Slot()
				.Padding(5, 0, 0, 0)
				[
					SNew(SButton)
						.Text(LOCTEXT("ForgeToolCreate", "Load"))
						.TextStyle(FEditorStyle::Get(), "LargeText")
						.ButtonStyle(FEditorStyle::Get(), "FlatButton.Success")
						.ForegroundColor(FLinearColor::White)
						.ContentPadding(FMargin(6, 6))
						.IsEnabled_Raw(EditorMode, &FForgeImporterEdMode::CanLoadMap)
						.HAlign(HAlign_Center)
						.VAlign(VAlign_Center)
						.OnClicked_Raw(EditorMode, &FForgeImporterEdMode::AttemptLoadMap)
				]
			]
		];
		
	FModeToolkit::Init(InitToolkitHost);
}

FName FForgeImporterEdModeToolkit::GetToolkitFName() const
{
	return FName("ForgeImporterEdMode");
}

FText FForgeImporterEdModeToolkit::GetBaseToolkitName() const
{
	return NSLOCTEXT("ForgeImporterEdModeToolkit", "DisplayName", "ForgeImporterEdMode Tool");
}

FEdMode* FForgeImporterEdModeToolkit::GetEditorMode() const
{
	return GLevelEditorModeTools().GetActiveMode(FForgeImporterEdMode::EM_ForgeImporterEdModeId);
}

FForgeImporterEdMode* FForgeImporterEdModeToolkit::GetForgeEditorMode() const
{
	return (FForgeImporterEdMode*)GetEditorMode();
}

void FForgeImporterEdModeToolkit::StartTool()
{
	FForgeImporterEdMode* ForgeMode = GetForgeEditorMode();
	if (!ForgeMode) return; // this shouldnt occur

	TArray<UObject*> ObjectSettings = TArray<UObject*>();

	ObjectSettings.Add(ForgeMode->UIImportSettings);
	ObjectSettings.Add(ForgeMode->UIExportSettings);

	DetailsView->SetObjects(ObjectSettings, true, true);
}

#undef LOCTEXT_NAMESPACE