// Copyright Adam (@portalwars2) 2025

#include "ForgeImporter.h"
#include "ForgeImporterEdMode.h"
#include "Interfaces/IPluginManager.h"

DEFINE_LOG_CATEGORY(LogForge);

#define LOCTEXT_NAMESPACE "FForgeImporterModule"

// Quick macro helper for getting slate images
#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )

FName FForgeImporterModule::GetStyleSetName()
{
	static const FName ForgeStyleName(TEXT("ForgeStyle")); // static style name
	return ForgeStyleName;
}

void FForgeImporterModule::StartupModule()
{
	InitializeStyle(); // so we can use our slate brushes

	if (Style.IsValid())
	{
		FEditorModeRegistry::Get().RegisterMode<FForgeImporterEdMode>(
			FForgeImporterEdMode::EM_ForgeImporterEdModeId,
			LOCTEXT("ForgeImporterEdModeName", "Forge Importer"),
			FSlateIcon(GetStyleSetName(), "ForgeImport.ModeIcon", "ForgeImport.ModeIcon.Small"),
			true);
	}
}

void FForgeImporterModule::ShutdownModule()
{
	// proper shut down so its graceful and doesnt end with a crash

	if (Style.IsValid())
		FSlateStyleRegistry::UnRegisterSlateStyle(Style->GetStyleSetName());

	FEditorModeRegistry::Get().UnregisterMode(FForgeImporterEdMode::EM_ForgeImporterEdModeId);
}

// icon sizes
const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);
const FVector2D Icon40x40(40.0f, 40.0f);
const FVector2D Icon64x64(64.0f, 64.0f);

void FForgeImporterModule::InitializeStyle()
{
	Style = MakeShareable(new FSlateStyleSet(GetStyleSetName()));

	// unhardcoded way of finding resources
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("ForgeImporter")->GetBaseDir() / TEXT("Resources"));

	// error but compile
	Style->Set("ForgeImport.ModeIcon", new IMAGE_BRUSH(TEXT("Icon_ModeIcon40"), Icon40x40));
	Style->Set("ForgeImport.ModeIcon.Small", new IMAGE_BRUSH(TEXT("Icon_ModeIcon40"), Icon20x20));

	// setup our style
	FSlateStyleRegistry::RegisterSlateStyle(*Style.Get());
}

#undef IMAGE_BRUSH
#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FForgeImporterModule, ForgeImporter)