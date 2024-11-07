// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeUEFormat_init() {}
	static FPackageRegistrationInfo Z_Registration_Info_UPackage__Script_UEFormat;
	FORCENOINLINE UPackage* Z_Construct_UPackage__Script_UEFormat()
	{
		if (!Z_Registration_Info_UPackage__Script_UEFormat.OuterSingleton)
		{
			static const UECodeGen_Private::FPackageParams PackageParams = {
				"/Script/UEFormat",
				nullptr,
				0,
				PKG_CompiledIn | 0x00000040,
				0x8622DD56,
				0x2F810873,
				METADATA_PARAMS(0, nullptr)
			};
			UECodeGen_Private::ConstructUPackage(Z_Registration_Info_UPackage__Script_UEFormat.OuterSingleton, PackageParams);
		}
		return Z_Registration_Info_UPackage__Script_UEFormat.OuterSingleton;
	}
	static FRegisterCompiledInInfo Z_CompiledInDeferPackage_UPackage__Script_UEFormat(Z_Construct_UPackage__Script_UEFormat, TEXT("/Script/UEFormat"), Z_Registration_Info_UPackage__Script_UEFormat, CONSTRUCT_RELOAD_VERSION_INFO(FPackageReloadVersionInfo, 0x8622DD56, 0x2F810873));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
