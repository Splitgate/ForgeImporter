// Copyright Adam (@portalwars2) 2025

#include "ForgeImporterEdMode.h"
#include "ForgeImporterEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"

#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"

#include "Engine/StaticMeshActor.h"
#include "EngineUtils.h"

#include "Misc/ScopedSlowTask.h"
#include "ForgeImporter.h"

#include "JsonObjectConverter.h"
#include "Misc/FileHelper.h"
#include "Serialization/BufferArchive.h"

#include "Settings/ForgeDeveloperSettings.h"
#include "World/ForgeInterface.h"

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

	// add these as references so we dont have crashes between switching levels
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
		&& GiftedJSON->HasField(TEXT("mapID"))
		&& GiftedJSON->HasField(TEXT("timeStamp"))
		&& GiftedJSON->HasField(TEXT("sharingCode"))
		&& GiftedJSON->HasField(TEXT("name"))
		&& GiftedJSON->HasField(TEXT("displayName"))
		&& GiftedJSON->HasField(TEXT("interactablesSaveData")));
}

UDataTable* FForgeImporterEdMode::GetInteractables() const
{
	const UForgeDeveloperSettings* DefaultSettings = GetDefault<UForgeDeveloperSettings>();
	if (DefaultSettings)
	{
		UDataTable* InteractablesTable = DefaultSettings->InteractablesTable.LoadSynchronous(); // This will get it if its already loaded
		if (InteractablesTable)
			return InteractablesTable;
	}

	// Fall back to loading hardcoded path
	return LoadObject<UDataTable>(GEngine, TEXT("DataTable'/ForgeImporter/ForgeInteractables.ForgeInteractables'"));
}

int FForgeImporterEdMode::FindForgeInteractable(UObject* ClassOrMesh) const
{
	UDataTable* Interactables = GetInteractables();
	if (!Interactables || !ClassOrMesh) return -1;

	for (auto& Interactable : Interactables->GetRowMap())
	{
		FForgeInteractables* InteractableData = (FForgeInteractables*)Interactable.Value;

		if (ClassOrMesh->IsA<AStaticMeshActor>())
		{
			AStaticMeshActor* SMActor = Cast<AStaticMeshActor>(ClassOrMesh);
			if (!SMActor->GetStaticMeshComponent()) return -1;

			if (SMActor->GetStaticMeshComponent()->GetStaticMesh() == InteractableData->StaticMesh)
				return InteractableData->ID;
		}

		if (ClassOrMesh->IsA<UStaticMesh>())
		{
			UStaticMesh* Mesh = Cast<UStaticMesh>(ClassOrMesh);

			if (Mesh == InteractableData->StaticMesh)
				return InteractableData->ID;
		}

		if (ClassOrMesh->GetClass() == InteractableData->BuildClass)
			return InteractableData->ID;
	}

	return -1;
}

bool FForgeImporterEdMode::CanLoadMap() const
{
	if (UIImportSettings)
		return IsValidMap(UIImportSettings->ForgeJSON); // enable load button

	return false;
}

bool FForgeImporterEdMode::CanExportMap() const
{
	// Not a fan of this, but honestly doesnt matter too much
	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (ActorItr->Implements<UForgeInterface>())
			return true; // We have atleast 1 forge actor, allow this to be exported
	}

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
		UDataTable* ForgeInteractables = GetInteractables();
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

			ProcessForgeDataTask.EnterProgressFrame();
		}

		FNotificationInfo ImportNoti = FNotificationInfo(
			FText::Format(LOCTEXT("ForgeImportDone", "Imported '{MapName}'!"), Args));

		FSlateNotificationManager::Get().AddNotification(ImportNoti);
	}
	else
	{
		UE_LOG(LogForge, Error, TEXT("Supplied JSON is not a valid Forge map."));

		FNotificationInfo ImportFailedNoti = FNotificationInfo(
			LOCTEXT("ForgeImportFailed", "Failed to import Forge map, invalid map"));

		FSlateNotificationManager::Get().AddNotification(ImportFailedNoti);
	}

	return FReply::Handled();
}

FReply FForgeImporterEdMode::AttemptExportMap()
{
	const UForgeDeveloperSettings* EditorSettings = GetDefault<UForgeDeveloperSettings>();

	FForgeMapData ForgeMap = FForgeMapData();
	ForgeMap.displayName = UIExportSettings->DisplayName;
	ForgeMap.name = UIExportSettings->BaseMap;
	ForgeMap.timeStamp = FDateTime::Now().ToString();

	FString MapFile = EditorSettings->ExportPath.Path;
	MapFile.Append(TEXT("/") + ForgeMap.displayName);

	FFormatNamedArguments Args;
	Args.Add(TEXT("MapName"), FText::FromString(ForgeMap.displayName));

	// start editor delay
	FScopedSlowTask ExportForgeDataTask(2,
		FText::Format(LOCTEXT("ForgeExporting", "Exporting {MapName}"), Args));

	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Only consider forge implemented actors
		if (!ActorItr->Implements<UForgeInterface>() && !ActorItr->IsA<AStaticMeshActor>())
			continue;

		int FoundId = FindForgeInteractable(*ActorItr);
		if (FoundId == -1) continue;

		FInteractableSave Interactable = FInteractableSave();
		Interactable.transform = ActorItr->GetTransform();
		Interactable.id = FoundId;

		ForgeMap.interactablesSaveData.Add(Interactable);
	}
	ExportForgeDataTask.EnterProgressFrame();

	IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();

	if (UIExportSettings->ExportType == EForgeExportType::Forge)
	{
		// Serialize format
		MapFile.Append(TEXT(".forge"));

		FBufferArchive ForgeFileData;
		ForgeFileData << ForgeMap;

		FFileHelper::SaveArrayToFile(ForgeFileData, *MapFile);

		ForgeFileData.FlushCache();
		ForgeFileData.Empty();
	}
	else
	{
		// Default to JSON format
		MapFile.Append(TEXT(".json"));

		FString ForgeDataString;
		FJsonObjectConverter::UStructToJsonObjectString<FForgeMapData>(ForgeMap, ForgeDataString);

		FFileHelper::SaveStringToFile(ForgeDataString, *MapFile);
	}	

	ExportForgeDataTask.EnterProgressFrame();

	FNotificationInfo ExportNoti = FNotificationInfo(
		FText::Format(LOCTEXT("ForgeExportDone", "Exported {MapName}"), Args));
	ExportNoti.HyperlinkText = FText::FromString(L"None");

	FSlateNotificationManager::Get().AddNotification(ExportNoti);

	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE