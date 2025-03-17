// Copyright Adam (@portalwars2) 2025

#include "ForgeImporterEdMode.h"
#include "ForgeImporterEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"

#include "Engine/StaticMeshActor.h"

#include "Misc/ScopedSlowTask.h"
#include "ForgeImporter.h"

#include "JsonObjectConverter.h"
#include "Misc/FileHelper.h"

const FEditorModeID FForgeImporterEdMode::EM_ForgeImporterEdModeId = TEXT("EM_ForgeImporterEdMode");

#define LOCTEXT_NAMESPACE "FForgeImporterEdMode"

FForgeImporterEdMode::FForgeImporterEdMode()
	: FEdMode()
{
	// create the editor objects, easier way of having settings

	UIImportSettings = NewObject<UForgeImportEditorObject>(GetTransientPackage(), TEXT("UIImportSettings"), RF_Transactional);
	UIExportSettings = NewObject<UForgeExportEditorObject>(GetTransientPackage(), TEXT("UIExportSettings"), RF_Transactional);
}

FForgeImporterEdMode::~FForgeImporterEdMode()
{

}

void FForgeImporterEdMode::Enter()
{
	FEdMode::Enter();

	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FForgeImporterEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());

		FForgeImporterEdModeToolkit* ForgeToolkit = (FForgeImporterEdModeToolkit*)Toolkit.Get();
		TSharedPtr<IDetailsView> DetailsView = ForgeToolkit->GetDetailsView();

		if (DetailsView)
		{
			DetailsView->OnFinishedChangingProperties().AddSP(this, &FForgeImporterEdMode::OnPropertyChanged);
		}
	}
}

void FForgeImporterEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	FEdMode::Exit();
}

bool FForgeImporterEdMode::UsesToolkits() const
{
	return true;
}

void FForgeImporterEdMode::AddReferencedObjects(FReferenceCollector& Collector)
{
	FEdMode::AddReferencedObjects(Collector);

	// add these as references so we dont have crashes betweem switching levels
	Collector.AddReferencedObject(UIImportSettings);
	Collector.AddReferencedObject(UIExportSettings);
}

void FForgeImporterEdMode::OnPropertyChanged(const FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.MemberProperty)
	{
		if (PropertyChangedEvent.MemberProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UForgeExportEditorObject, Map))
		{
			// Update locked actual map name the game uses (display name is formality)

			FString Enum, EnumName;
			UEnum::GetValueAsString(UIExportSettings->Map).Split("::", &Enum, &EnumName);
			UIExportSettings->BaseMap = EnumName;
		}

		if (PropertyChangedEvent.MemberProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UForgeImportEditorObject, ImportPath))
		{
			// update chosen forge map

			if (!FPaths::FileExists(UIImportSettings->ImportPath.FilePath)) return;

			FString LoadedFile = TEXT("");
			FFileHelper::LoadFileToString(LoadedFile, *UIImportSettings->ImportPath.FilePath);

			if (LoadedFile.IsEmpty()) return;

			TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
			TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(LoadedFile);
			if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
			{
				// JSON hopefully loaded properly
				UIImportSettings->ForgeJSON = JsonObject;
			}
		}
	}
}

bool FForgeImporterEdMode::IsValidMap(TSharedPtr<FJsonObject> GiftedJSON) const
{
	// massive checked, if all of these exists, hooray, we have a valid forge map

	return (GiftedJSON.IsValid()
		&& GiftedJSON->HasField("mapID")
		&& GiftedJSON->HasField("timeStamp")
		&& GiftedJSON->HasField("sharingCode")
		&& GiftedJSON->HasField("name")
		&& GiftedJSON->HasField("displayName")
		&& GiftedJSON->HasField("interactablesSaveData"));
}

bool FForgeImporterEdMode::CanLoadMap() const
{
	if (UIImportSettings)
		return IsValidMap(UIImportSettings->ForgeJSON); // enable load button

	return false;
}

FReply FForgeImporterEdMode::AttemptLoadMap()
{
	if (!UIImportSettings && !UIImportSettings->ForgeJSON.IsValid())
	{
		UE_LOG(LogForge, Error, TEXT("No valid map to load!"));
		return FReply::Handled();
	}

	FForgeMapData MapData;
	if (FJsonObjectConverter::JsonObjectToUStruct<FForgeMapData>(UIImportSettings->ForgeJSON.ToSharedRef(), &MapData) && IsValidMap(UIImportSettings->ForgeJSON))
	{
		UE_LOG(LogForge, Display, TEXT("Processing \"%s\" with %d objects"), *MapData.displayName, MapData.interactablesSaveData.Num());

		// not a fan of hardcoding this but whatever it shouldnt move
		UDataTable* ForgeInteractables = LoadObject<UDataTable>(GEngine, TEXT("DataTable'/ForgeImporter/ForgeInteractables.ForgeInteractables'"));
		if (!ForgeInteractables)
		{
			UE_LOG(LogForge, Error, TEXT("Failed to find ForgeInteractables."));

			return FReply::Handled();
		}

		FFormatNamedArguments Args;
		Args.Add(TEXT("MapName"), FText::FromString(MapData.displayName));

		// start editor delay
		FScopedSlowTask ProcessForgeDataTask(MapData.interactablesSaveData.Num(),
			FText::Format(LOCTEXT("ForgeProcessing", "Processing {MapName}"), Args));

		// Create a scoped transition to allow for ctrl z undo/redo
		const FScopedTransaction Transaction(FText::Format(LOCTEXT("ForgeUndo", "Created {MapName}"), Args));

		ProcessForgeDataTask.MakeDialog();

		auto GetStaticMesh = [&](int ID) -> UStaticMesh* {
			auto ForgeInfo = ForgeInteractables->FindRow<FForgeInteractables>(FName(FString::FromInt(ID)), TEXT(""));

			return ForgeInfo->StaticMesh;
		};

		auto GetForgeInteractableClass = [&](int ID) -> TSoftClassPtr<AActor> {
			auto ForgeInfo = ForgeInteractables->FindRow<FForgeInteractables>(FName(FString::FromInt(ID)), TEXT(""));

			return ForgeInfo->BuildClass;
		};

		for (auto& Interactable : MapData.interactablesSaveData)
		{
			TSoftClassPtr<AActor> ForgeClass = GetForgeInteractableClass(Interactable.id);
			UStaticMesh* Mesh = GetStaticMesh(Interactable.id);

			UClass* LoadedClass = ForgeClass.LoadSynchronous();

			// if the user only wants the meshes ignore any actual classes
			if (UIImportSettings->ImportStaticMeshesOnly && Mesh)
			{
				AStaticMeshActor* SMActor = GWorld->SpawnActor<AStaticMeshActor>(
					AStaticMeshActor::StaticClass(), Interactable.transform);

				if (!SMActor) continue;

				SMActor->GetStaticMeshComponent()->SetStaticMesh(Mesh);

				if (SMActor)
					SMActor->SetFolderPath(FName(MapData.displayName)); // bit of categorization
			}

			// normal object spawn
			if (!UIImportSettings->ImportStaticMeshesOnly && LoadedClass)
			{
				AActor* SpawnedActor = GWorld->SpawnActor<AActor>(LoadedClass, Interactable.transform);
				if (!SpawnedActor) continue;

				SpawnedActor->SetFolderPath(FName(MapData.displayName));
			}

			ProcessForgeDataTask.EnterProgressFrame(); // bit of categorization
		}
	}
	else
	{
		UE_LOG(LogForge, Error, TEXT("Supplied JSON is not a valid Forge map."));
	}

	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE