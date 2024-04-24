// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UEFormat/Public/Widgets/SkelMesh/USkelMeshImportOptions.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeUSkelMeshImportOptions() {}

// Begin Cross Module References
COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
ENGINE_API UClass* Z_Construct_UClass_USkeleton_NoRegister();
UEFORMAT_API UClass* Z_Construct_UClass_USkelMeshImportOptions();
UEFORMAT_API UClass* Z_Construct_UClass_USkelMeshImportOptions_NoRegister();
UPackage* Z_Construct_UPackage__Script_UEFormat();
// End Cross Module References

// Begin Class USkelMeshImportOptions
void USkelMeshImportOptions::StaticRegisterNativesUSkelMeshImportOptions()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(USkelMeshImportOptions);
UClass* Z_Construct_UClass_USkelMeshImportOptions_NoRegister()
{
	return USkelMeshImportOptions::StaticClass();
}
struct Z_Construct_UClass_USkelMeshImportOptions_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Widgets/SkelMesh/USkelMeshImportOptions.h" },
		{ "ModuleRelativePath", "Public/Widgets/SkelMesh/USkelMeshImportOptions.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Skeleton_MetaData[] = {
		{ "Category", "Import Settings" },
		{ "ModuleRelativePath", "Public/Widgets/SkelMesh/USkelMeshImportOptions.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Skeleton;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<USkelMeshImportOptions>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_USkelMeshImportOptions_Statics::NewProp_Skeleton = { "Skeleton", nullptr, (EPropertyFlags)0x0114000000000001, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(USkelMeshImportOptions, Skeleton), Z_Construct_UClass_USkeleton_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Skeleton_MetaData), NewProp_Skeleton_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_USkelMeshImportOptions_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_USkelMeshImportOptions_Statics::NewProp_Skeleton,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_USkelMeshImportOptions_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_USkelMeshImportOptions_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UObject,
	(UObject* (*)())Z_Construct_UPackage__Script_UEFormat,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_USkelMeshImportOptions_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_USkelMeshImportOptions_Statics::ClassParams = {
	&USkelMeshImportOptions::StaticClass,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_USkelMeshImportOptions_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_USkelMeshImportOptions_Statics::PropPointers),
	0,
	0x001000AAu,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_USkelMeshImportOptions_Statics::Class_MetaDataParams), Z_Construct_UClass_USkelMeshImportOptions_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_USkelMeshImportOptions()
{
	if (!Z_Registration_Info_UClass_USkelMeshImportOptions.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_USkelMeshImportOptions.OuterSingleton, Z_Construct_UClass_USkelMeshImportOptions_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_USkelMeshImportOptions.OuterSingleton;
}
template<> UEFORMAT_API UClass* StaticClass<USkelMeshImportOptions>()
{
	return USkelMeshImportOptions::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(USkelMeshImportOptions);
USkelMeshImportOptions::~USkelMeshImportOptions() {}
// End Class USkelMeshImportOptions

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_Marcel_Documents_3D_Unreal_Projects_Main_UUTesting_Plugins_UEFormat_Source_UEFormat_Public_Widgets_SkelMesh_USkelMeshImportOptions_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_USkelMeshImportOptions, USkelMeshImportOptions::StaticClass, TEXT("USkelMeshImportOptions"), &Z_Registration_Info_UClass_USkelMeshImportOptions, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(USkelMeshImportOptions), 1077605066U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_Marcel_Documents_3D_Unreal_Projects_Main_UUTesting_Plugins_UEFormat_Source_UEFormat_Public_Widgets_SkelMesh_USkelMeshImportOptions_h_2504674023(TEXT("/Script/UEFormat"),
	Z_CompiledInDeferFile_FID_Users_Marcel_Documents_3D_Unreal_Projects_Main_UUTesting_Plugins_UEFormat_Source_UEFormat_Public_Widgets_SkelMesh_USkelMeshImportOptions_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_Marcel_Documents_3D_Unreal_Projects_Main_UUTesting_Plugins_UEFormat_Source_UEFormat_Public_Widgets_SkelMesh_USkelMeshImportOptions_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
