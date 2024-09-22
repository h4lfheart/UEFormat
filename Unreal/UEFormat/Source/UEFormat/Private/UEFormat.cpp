// Copyright Epic Games, Inc. All Rights Reserved.
#include "../Public/UEFormat.h"
#include "UEFormatSettings.h"
#include "ISettingsModule.h"

#define LOCTEXT_NAMESPACE "FUEFormat"

void FUEFormatModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// Register settings
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "Plugins", "UEFormat",
			LOCTEXT("RuntimeSettingsName", "UEFormat"),
			LOCTEXT("RuntimeSettingsDescription", "Configure the UEFormat plugin"),
			GetMutableDefault<UUEFormatSettings>()
		);
	}
}

void FUEFormatModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	// Unregister settings
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "UEFormat");
	}
}

IMPLEMENT_MODULE(FUEFormatModule, UEFormat)