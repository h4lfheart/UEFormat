// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UEFormat/Source/UEFormat/Public/Factories/UEFModelFactory.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeUEFModelFactory() {}

// Begin Cross Module References
UEFORMAT_API UClass* Z_Construct_UClass_UEFModelFactory();
UEFORMAT_API UClass* Z_Construct_UClass_UEFModelFactory_NoRegister();
UNREALED_API UClass* Z_Construct_UClass_UFactory();
UPackage* Z_Construct_UPackage__Script_UEFormat();
// End Cross Module References

// Begin Class UEFModelFactory
void UEFModelFactory::StaticRegisterNativesUEFModelFactory()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UEFModelFactory);
UClass* Z_Construct_UClass_UEFModelFactory_NoRegister()
{
	return UEFModelFactory::StaticClass();
}
struct Z_Construct_UClass_UEFModelFactory_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "HideCategories", "Object" },
		{ "IncludePath", "Factories/UEFModelFactory.h" },
		{ "ModuleRelativePath", "Public/Factories/UEFModelFactory.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UEFModelFactory>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UEFModelFactory_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UFactory,
	(UObject* (*)())Z_Construct_UPackage__Script_UEFormat,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UEFModelFactory_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UEFModelFactory_Statics::ClassParams = {
	&UEFModelFactory::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x001000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UEFModelFactory_Statics::Class_MetaDataParams), Z_Construct_UClass_UEFModelFactory_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UEFModelFactory()
{
	if (!Z_Registration_Info_UClass_UEFModelFactory.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UEFModelFactory.OuterSingleton, Z_Construct_UClass_UEFModelFactory_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UEFModelFactory.OuterSingleton;
}
template<> UEFORMAT_API UClass* StaticClass<UEFModelFactory>()
{
	return UEFModelFactory::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UEFModelFactory);
UEFModelFactory::~UEFModelFactory() {}
// End Class UEFModelFactory

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_Marcel_Documents_3D_Unreal_Projects_Main_UniversalUmapTesting_Plugins_UEFormat_Source_UEFormat_Source_UEFormat_Public_Factories_UEFModelFactory_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UEFModelFactory, UEFModelFactory::StaticClass, TEXT("UEFModelFactory"), &Z_Registration_Info_UClass_UEFModelFactory, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UEFModelFactory), 3863231043U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_Marcel_Documents_3D_Unreal_Projects_Main_UniversalUmapTesting_Plugins_UEFormat_Source_UEFormat_Source_UEFormat_Public_Factories_UEFModelFactory_h_2601193592(TEXT("/Script/UEFormat"),
	Z_CompiledInDeferFile_FID_Users_Marcel_Documents_3D_Unreal_Projects_Main_UniversalUmapTesting_Plugins_UEFormat_Source_UEFormat_Source_UEFormat_Public_Factories_UEFModelFactory_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_Marcel_Documents_3D_Unreal_Projects_Main_UniversalUmapTesting_Plugins_UEFormat_Source_UEFormat_Source_UEFormat_Public_Factories_UEFModelFactory_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
