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
#if (ENGINE_MAJOR_VERSION < 5)
		DetailsViewArgs.bShowActorLabel = false;
#else
		DetailsViewArgs.bShowObjectLabel = false;
#endif
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
#if (ENGINE_MAJOR_VERSION < 5)
		.BorderImage(FEditorStyle::GetBrush("NoBorder"))
#else
		.BorderImage(FAppStyle::GetBrush("NoBorder"))
#endif
		.Padding(1)
		[
			SNew( SVerticalBox )
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign( HAlign_Fill )
			.VAlign( VAlign_Fill )
			.Padding(1)
			[
				SNew(SExpandableArea)
				.HeaderPadding(FMargin(2.0f))
				.Padding(FMargin(10.f))
				.BorderBackgroundColor(FLinearColor(.6, .6, .6, 1.0f))
				.BodyBorderBackgroundColor(FLinearColor::Transparent)
#if (ENGINE_MAJOR_VERSION < 5)
				.BorderImage(FEditorStyle::Get().GetBrush("DetailsView.CategoryTop"))
				.AreaTitleFont(FEditorStyle::Get().GetFontStyle("EditorModesPanel.CategoryFontStyle"))
#else
				.BorderImage(FAppStyle::Get().GetBrush("DetailsView.CategoryTop"))
				.AreaTitleFont(FAppStyle::Get().GetFontStyle("EditorModesPanel.CategoryFontStyle"))
#endif
				.BodyContent()
				[
					SNew(STextBlock)
					.Text(LOCTEXT("ForgeToolMessage", "This tool allows you to import Splitgate 1 Forge maps via JSON save from https://github.com/Splitgate/SplitgateForgeBackup."))
#if (ENGINE_MAJOR_VERSION < 5)
					.Font(FEditorStyle::Get().GetFontStyle("EditorModesPanel.ToolDescriptionFont"))
#else
					.Font(FAppStyle::Get().GetFontStyle("EditorModesPanel.ToolDescriptionFont"))
#endif
					.AutoWrapText(true)
				]
				.HeaderContent()
				[
					SNew(STextBlock)
					.Text(LOCTEXT("ForgeDisplay", "Forge Importer"))
					.Justification(ETextJustify::Center)
#if (ENGINE_MAJOR_VERSION < 5)
					.Font(FEditorStyle::Get().GetFontStyle("EditorModesPanel.CategoryFontStyle"))
#else
					.Font(FAppStyle::Get().GetFontStyle("EditorModesPanel.CategoryFontStyle"))
#endif
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
#if (ENGINE_MAJOR_VERSION < 5)
						.TextStyle(FEditorStyle::Get(), "LargeText")
						.ButtonStyle(FEditorStyle::Get(), "FlatButton.Success")
#else
						.TextStyle(FAppStyle::Get(), "LargeText")
						.ButtonStyle(FAppStyle::Get(), "FlatButton.Success")
#endif
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
						.Text(LOCTEXT("ForgeToolCreate", "Import"))
#if (ENGINE_MAJOR_VERSION < 5)
						.TextStyle(FEditorStyle::Get(), "LargeText")
						.ButtonStyle(FEditorStyle::Get(), "FlatButton.Success")
#else
						.TextStyle(FAppStyle::Get(), "LargeText")
						.ButtonStyle(FAppStyle::Get(), "FlatButton.Success")
#endif
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