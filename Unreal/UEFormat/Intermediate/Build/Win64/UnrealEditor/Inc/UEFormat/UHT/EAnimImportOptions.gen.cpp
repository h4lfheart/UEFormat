// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "../../../../../../../../../../../Users/Marcel/Documents/3D/Unreal Projects/Github Projects/UianaProject/Plugins/UEFormat/Source/UEFormat/Public/Widgets/EAnimImportOptions.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeEAnimImportOptions() {}
// Cross Module References
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
	ENGINE_API UClass* Z_Construct_UClass_USkeleton_NoRegister();
	UEFORMAT_API UClass* Z_Construct_UClass_UEAnimImportOptions();
	UEFORMAT_API UClass* Z_Construct_UClass_UEAnimImportOptions_NoRegister();
	UPackage* Z_Construct_UPackage__Script_UEFormat();
// End Cross Module References
	void UEAnimImportOptions::StaticRegisterNativesUEAnimImportOptions()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UEAnimImportOptions);
	UClass* Z_Construct_UClass_UEAnimImportOptions_NoRegister()
	{
		return UEAnimImportOptions::StaticClass();
	}
	struct Z_Construct_UClass_UEAnimImportOptions_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Skeleton_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPtrPropertyParams NewProp_Skeleton;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UEAnimImportOptions_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UObject,
		(UObject* (*)())Z_Construct_UPackage__Script_UEFormat,
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UEAnimImportOptions_Statics::DependentSingletons) < 16);
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UEAnimImportOptions_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "Widgets/EAnimImportOptions.h" },
		{ "ModuleRelativePath", "Public/Widgets/EAnimImportOptions.h" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UEAnimImportOptions_Statics::NewProp_Skeleton_MetaData[] = {
		{ "Category", "Import Settings" },
		{ "ModuleRelativePath", "Public/Widgets/EAnimImportOptions.h" },
	};
#endif
	const UECodeGen_Private::FObjectPtrPropertyParams Z_Construct_UClass_UEAnimImportOptions_Statics::NewProp_Skeleton = { "Skeleton", nullptr, (EPropertyFlags)0x0014000000000001, UECodeGen_Private::EPropertyGenFlags::Object | UECodeGen_Private::EPropertyGenFlags::ObjectPtr, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UEAnimImportOptions, Skeleton), Z_Construct_UClass_USkeleton_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UEAnimImportOptions_Statics::NewProp_Skeleton_MetaData), Z_Construct_UClass_UEAnimImportOptions_Statics::NewProp_Skeleton_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UEAnimImportOptions_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UEAnimImportOptions_Statics::NewProp_Skeleton,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UEAnimImportOptions_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UEAnimImportOptions>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UEAnimImportOptions_Statics::ClassParams = {
		&UEAnimImportOptions::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UEAnimImportOptions_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UEAnimImportOptions_Statics::PropPointers),
		0,
		0x001000AAu,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UEAnimImportOptions_Statics::Class_MetaDataParams), Z_Construct_UClass_UEAnimImportOptions_Statics::Class_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UEAnimImportOptions_Statics::PropPointers) < 2048);
	UClass* Z_Construct_UClass_UEAnimImportOptions()
	{
		if (!Z_Registration_Info_UClass_UEAnimImportOptions.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UEAnimImportOptions.OuterSingleton, Z_Construct_UClass_UEAnimImportOptions_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UEAnimImportOptions.OuterSingleton;
	}
	template<> UEFORMAT_API UClass* StaticClass<UEAnimImportOptions>()
	{
		return UEAnimImportOptions::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UEAnimImportOptions);
	UEAnimImportOptions::~UEAnimImportOptions() {}
	struct Z_CompiledInDeferFile_FID_Users_Marcel_Documents_3D_Unreal_Projects_Github_Projects_UianaProject_Plugins_UEFormat_Source_UEFormat_Public_Widgets_EAnimImportOptions_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_Marcel_Documents_3D_Unreal_Projects_Github_Projects_UianaProject_Plugins_UEFormat_Source_UEFormat_Public_Widgets_EAnimImportOptions_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UEAnimImportOptions, UEAnimImportOptions::StaticClass, TEXT("UEAnimImportOptions"), &Z_Registration_Info_UClass_UEAnimImportOptions, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UEAnimImportOptions), 2719133466U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_Marcel_Documents_3D_Unreal_Projects_Github_Projects_UianaProject_Plugins_UEFormat_Source_UEFormat_Public_Widgets_EAnimImportOptions_h_2661078188(TEXT("/Script/UEFormat"),
		Z_CompiledInDeferFile_FID_Users_Marcel_Documents_3D_Unreal_Projects_Github_Projects_UianaProject_Plugins_UEFormat_Source_UEFormat_Public_Widgets_EAnimImportOptions_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_Marcel_Documents_3D_Unreal_Projects_Github_Projects_UianaProject_Plugins_UEFormat_Source_UEFormat_Public_Widgets_EAnimImportOptions_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
