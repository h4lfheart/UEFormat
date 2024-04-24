// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UEFormat/Public/Factories/UEModelFactory.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeUEModelFactory() {}

// Begin Cross Module References
UEFORMAT_API UClass* Z_Construct_UClass_UEModelFactory();
UEFORMAT_API UClass* Z_Construct_UClass_UEModelFactory_NoRegister();
UEFORMAT_API UClass* Z_Construct_UClass_USkelMeshImportOptions_NoRegister();
UNREALED_API UClass* Z_Construct_UClass_UFactory();
UPackage* Z_Construct_UPackage__Script_UEFormat();
// End Cross Module References

// Begin Class UEModelFactory
void UEModelFactory::StaticRegisterNativesUEModelFactory()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UEModelFactory);
UClass* Z_Construct_UClass_UEModelFactory_NoRegister()
{
	return UEModelFactory::StaticClass();
}
struct Z_Construct_UClass_UEModelFactory_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "HideCategories", "Object" },
		{ "IncludePath", "Factories/UEModelFactory.h" },
		{ "ModuleRelativePath", "Public/Factories/UEModelFactory.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SettingsImporter_MetaData[] = {
		{ "ModuleRelativePath", "Public/Factories/UEModelFactory.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_SettingsImporter;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UEModelFactory>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UEModelFactory_Statics::NewProp_SettingsImporter = { "SettingsImporter", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UEModelFactory, SettingsImporter), Z_Construct_UClass_USkelMeshImportOptions_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SettingsImporter_MetaData), NewProp_SettingsImporter_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UEModelFactory_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UEModelFactory_Statics::NewProp_SettingsImporter,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UEModelFactory_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UEModelFactory_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UFactory,
	(UObject* (*)())Z_Construct_UPackage__Script_UEFormat,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UEModelFactory_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UEModelFactory_Statics::ClassParams = {
	&UEModelFactory::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UEModelFactory_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UEModelFactory_Statics::PropPointers),
	0,
	0x001000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UEModelFactory_Statics::Class_MetaDataParams), Z_Construct_UClass_UEModelFactory_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UEModelFactory()
{
	if (!Z_Registration_Info_UClass_UEModelFactory.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UEModelFactory.OuterSingleton, Z_Construct_UClass_UEModelFactory_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UEModelFactory.OuterSingleton;
}
template<> UEFORMAT_API UClass* StaticClass<UEModelFactory>()
{
	return UEModelFactory::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UEModelFactory);
UEModelFactory::~UEModelFactory() {}
// End Class UEModelFactory

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_Marcel_Documents_3D_Unreal_Projects_Main_UUTesting_Plugins_UEFormat_Source_UEFormat_Public_Factories_UEModelFactory_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UEModelFactory, UEModelFactory::StaticClass, TEXT("UEModelFactory"), &Z_Registration_Info_UClass_UEModelFactory, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UEModelFactory), 1039053464U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_Marcel_Documents_3D_Unreal_Projects_Main_UUTesting_Plugins_UEFormat_Source_UEFormat_Public_Factories_UEModelFactory_h_2476173727(TEXT("/Script/UEFormat"),
	Z_CompiledInDeferFile_FID_Users_Marcel_Documents_3D_Unreal_Projects_Main_UUTesting_Plugins_UEFormat_Source_UEFormat_Public_Factories_UEModelFactory_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_Marcel_Documents_3D_Unreal_Projects_Main_UUTesting_Plugins_UEFormat_Source_UEFormat_Public_Factories_UEModelFactory_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
