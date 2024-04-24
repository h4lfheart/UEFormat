// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UEFormat/Public/Factories/UEAnimFactory.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeUEAnimFactory() {}

// Begin Cross Module References
UEFORMAT_API UClass* Z_Construct_UClass_UAnimImportOptions_NoRegister();
UEFORMAT_API UClass* Z_Construct_UClass_UEAnimFactory();
UEFORMAT_API UClass* Z_Construct_UClass_UEAnimFactory_NoRegister();
UNREALED_API UClass* Z_Construct_UClass_UFactory();
UPackage* Z_Construct_UPackage__Script_UEFormat();
// End Cross Module References

// Begin Class UEAnimFactory
void UEAnimFactory::StaticRegisterNativesUEAnimFactory()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UEAnimFactory);
UClass* Z_Construct_UClass_UEAnimFactory_NoRegister()
{
	return UEAnimFactory::StaticClass();
}
struct Z_Construct_UClass_UEAnimFactory_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "HideCategories", "Object" },
		{ "IncludePath", "Factories/UEAnimFactory.h" },
		{ "ModuleRelativePath", "Public/Factories/UEAnimFactory.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SettingsImporter_MetaData[] = {
		{ "ModuleRelativePath", "Public/Factories/UEAnimFactory.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_SettingsImporter;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UEAnimFactory>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UEAnimFactory_Statics::NewProp_SettingsImporter = { "SettingsImporter", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UEAnimFactory, SettingsImporter), Z_Construct_UClass_UAnimImportOptions_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SettingsImporter_MetaData), NewProp_SettingsImporter_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UEAnimFactory_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UEAnimFactory_Statics::NewProp_SettingsImporter,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UEAnimFactory_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UEAnimFactory_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UFactory,
	(UObject* (*)())Z_Construct_UPackage__Script_UEFormat,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UEAnimFactory_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UEAnimFactory_Statics::ClassParams = {
	&UEAnimFactory::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UEAnimFactory_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UEAnimFactory_Statics::PropPointers),
	0,
	0x001000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UEAnimFactory_Statics::Class_MetaDataParams), Z_Construct_UClass_UEAnimFactory_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UEAnimFactory()
{
	if (!Z_Registration_Info_UClass_UEAnimFactory.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UEAnimFactory.OuterSingleton, Z_Construct_UClass_UEAnimFactory_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UEAnimFactory.OuterSingleton;
}
template<> UEFORMAT_API UClass* StaticClass<UEAnimFactory>()
{
	return UEAnimFactory::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UEAnimFactory);
UEAnimFactory::~UEAnimFactory() {}
// End Class UEAnimFactory

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_Marcel_Documents_3D_Unreal_Projects_Main_UUTesting_Plugins_UEFormat_Source_UEFormat_Public_Factories_UEAnimFactory_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UEAnimFactory, UEAnimFactory::StaticClass, TEXT("UEAnimFactory"), &Z_Registration_Info_UClass_UEAnimFactory, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UEAnimFactory), 1355197157U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_Marcel_Documents_3D_Unreal_Projects_Main_UUTesting_Plugins_UEFormat_Source_UEFormat_Public_Factories_UEAnimFactory_h_1523209106(TEXT("/Script/UEFormat"),
	Z_CompiledInDeferFile_FID_Users_Marcel_Documents_3D_Unreal_Projects_Main_UUTesting_Plugins_UEFormat_Source_UEFormat_Public_Factories_UEAnimFactory_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_Marcel_Documents_3D_Unreal_Projects_Main_UUTesting_Plugins_UEFormat_Source_UEFormat_Public_Factories_UEAnimFactory_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
