// Copyright Adam (@portalwars2) 2025



#include "Utility/ForgeKismetLibrary.h"
#include "ForgeImporter.h"

#include "World/ForgeStaticMeshInteractable.h"
#include "IContentBrowserSingleton.h"
#include "ContentBrowserModule.h"
#include "AssetRegistry/AssetRegistryModule.h"

#include "Factories/BlueprintFactory.h"
#include "AssetToolsModule.h"

void UForgeKismetLibrary::Ed_CreateForgeActor(const FString& Name, const FString& Path, UStaticMesh* NewMesh)
{
	UBlueprintFactory* BlueprintFac = NewObject<UBlueprintFactory>(GetTransientPackage());
	BlueprintFac->ParentClass = AForgeStaticMeshInteractable::StaticClass();

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));

	FAssetToolsModule& AssetToolsModule = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools");
	IAssetTools& AssetTools = AssetToolsModule.Get();

	FContentBrowserModule& ContentBrowserModule = FModuleManager::Get().LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	IContentBrowserSingleton& ContentBrowser = ContentBrowserModule.Get();

	FString IncomingPath = FString::Printf(TEXT("%s/%s"), *Path, *Name);

	if (!AssetRegistryModule.Get().IsLoadingAssets())
	{
		FAssetData AssetData = AssetRegistryModule.Get().GetAssetByObjectPath(*IncomingPath);
		if (AssetData.IsValid())
		{
			// for this i dont want it making another as it may confuse me
			return;
		}
	}

	FString AssetName, PackageName;
	AssetToolsModule.Get().CreateUniqueAssetName(IncomingPath, TEXT(""), PackageName, AssetName);

	UPackage* NewPackage = CreatePackage(*PackageName);
	if (NewPackage)
	{
		UBlueprint* NewAsset = (UBlueprint*)AssetTools.CreateAsset(AssetName, Path, BlueprintFac->GetSupportedClass(), BlueprintFac);
		if (!NewAsset) return;

		AForgeStaticMeshInteractable* CDO = Cast<AForgeStaticMeshInteractable>(NewAsset->GeneratedClass->ClassDefaultObject);
		CDO->UpdateForgeMesh(NewMesh);

		UPackage::Save(NewPackage, NewAsset, RF_Public | RF_Standalone, *FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension()));

		//TArray<UObject*> Objects;
		//Objects.Add(NewAsset);
		//ContentBrowserModule.Get().SyncBrowserToAssets(Objects);

		NewAsset->OnCompiled();
	}
}

UBlueprint* UForgeKismetLibrary::GetBlueprintAsset(UObject* Object)
{
	return Cast<UBlueprint>(Object);
}

TArray<UActorComponent*> UForgeKismetLibrary::GetBlueprintTemplates(UBlueprint* Blueprint)
{
	if (Blueprint)
		return Blueprint->ComponentTemplates;

	return TArray<UActorComponent*>();
}

UActorComponent* UForgeKismetLibrary::FindComponent(UBlueprint* Blueprint, FName TemplateName)
{
	if (Blueprint && TemplateName != NAME_None)
		Blueprint->FindTemplateByName(TemplateName);

	return nullptr;
}

UClass* UForgeKismetLibrary::GetGeneratedClass(UBlueprint* BlueprintObject)
{
	if (BlueprintObject)
	{
		if (BlueprintObject->GeneratedClass == nullptr)
		{
			UE_LOG(LogForge, Warning, TEXT("Blueprint %s does not have a generated class - consider compiling it"), *BlueprintObject->GetPathName());
			return nullptr;
		}
		return BlueprintObject->GeneratedClass->GetAuthoritativeClass();
	}
	return nullptr;
}

UObject* UForgeKismetLibrary::GetDefaultObject(UClass* Class)
{
	if (Class && Class->ClassDefaultObject)
		Class->ClassDefaultObject;

	return nullptr;
}
