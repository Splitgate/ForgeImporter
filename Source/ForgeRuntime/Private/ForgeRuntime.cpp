#include "ForgeRuntime.h"

DEFINE_LOG_CATEGORY(ForgeRuntime);

#define LOCTEXT_NAMESPACE "FForgeRuntime"

void FForgeRuntime::StartupModule()
{
	UE_LOG(ForgeRuntime, Warning, TEXT("ForgeRuntime module has been loaded"));
}

void FForgeRuntime::ShutdownModule()
{
	UE_LOG(ForgeRuntime, Warning, TEXT("ForgeRuntime module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FForgeRuntime, ForgeRuntime)