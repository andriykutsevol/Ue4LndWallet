// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "LNDWalletPlugin.h"

#define LOCTEXT_NAMESPACE "FLNDWalletPluginModule"

void FLNDWalletPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	UE_LOG(LogTemp, Warning, TEXT("FLNDWalletPluginModule::StartupModule()"));

}

void FLNDWalletPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	//auto ulndnode = ULNDNode::StaticClass();
	//auto asdf = Cast<ULNDNode>(ulndnode);
	//UE_LOG(LogTemp, Warning, TEXT("asdfasdf: %b"), asdf->is_node_proc_running());

	//auto ulndnode = FindObject<UObject>(ANY_PACKAGE, TEXT("ULNDNode"), true);
	//auto asdf = Cast<ULNDNode>(ulndnode);
	//UE_LOG(LogTemp, Warning, TEXT("asdfasdf: %b"), asdf->is_node_proc_running());


}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FLNDWalletPluginModule, LNDWalletPlugin)