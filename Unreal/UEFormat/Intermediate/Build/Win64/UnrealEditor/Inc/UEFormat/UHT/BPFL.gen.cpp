// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UEFormat/Public/BPFL.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBPFL() {}

// Begin Cross Module References
ENGINE_API UClass* Z_Construct_UClass_UBlueprintFunctionLibrary();
ENGINE_API UClass* Z_Construct_UClass_UStaticMeshComponent_NoRegister();
UEFORMAT_API UClass* Z_Construct_UClass_UBPFL();
UEFORMAT_API UClass* Z_Construct_UClass_UBPFL_NoRegister();
UPackage* Z_Construct_UPackage__Script_UEFormat();
// End Cross Module References

// Begin Class UBPFL Function OverrideVertexColors
struct Z_Construct_UFunction_UBPFL_OverrideVertexColors_Statics
{
	struct BPFL_eventOverrideVertexColors_Parms
	{
		UStaticMeshComponent* Component;
		TArray<FString> Data;
		FString FilePath;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "VertexPainting" },
		{ "ModuleRelativePath", "Public/BPFL.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Component_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Component;
	static const UECodeGen_Private::FStrPropertyParams NewProp_Data_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_Data;
	static const UECodeGen_Private::FStrPropertyParams NewProp_FilePath;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UBPFL_OverrideVertexColors_Statics::NewProp_Component = { "Component", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BPFL_eventOverrideVertexColors_Parms, Component), Z_Construct_UClass_UStaticMeshComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Component_MetaData), NewProp_Component_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UBPFL_OverrideVertexColors_Statics::NewProp_Data_Inner = { "Data", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UBPFL_OverrideVertexColors_Statics::NewProp_Data = { "Data", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BPFL_eventOverrideVertexColors_Parms, Data), EArrayPropertyFlags::None, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UBPFL_OverrideVertexColors_Statics::NewProp_FilePath = { "FilePath", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(BPFL_eventOverrideVertexColors_Parms, FilePath), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UBPFL_OverrideVertexColors_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBPFL_OverrideVertexColors_Statics::NewProp_Component,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBPFL_OverrideVertexColors_Statics::NewProp_Data_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBPFL_OverrideVertexColors_Statics::NewProp_Data,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UBPFL_OverrideVertexColors_Statics::NewProp_FilePath,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UBPFL_OverrideVertexColors_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UBPFL_OverrideVertexColors_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UBPFL, nullptr, "OverrideVertexColors", nullptr, nullptr, Z_Construct_UFunction_UBPFL_OverrideVertexColors_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UBPFL_OverrideVertexColors_Statics::PropPointers), sizeof(Z_Construct_UFunction_UBPFL_OverrideVertexColors_Statics::BPFL_eventOverrideVertexColors_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UBPFL_OverrideVertexColors_Statics::Function_MetaDataParams), Z_Construct_UFunction_UBPFL_OverrideVertexColors_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UBPFL_OverrideVertexColors_Statics::BPFL_eventOverrideVertexColors_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UBPFL_OverrideVertexColors()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UBPFL_OverrideVertexColors_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UBPFL::execOverrideVertexColors)
{
	P_GET_OBJECT(UStaticMeshComponent,Z_Param_Component);
	P_GET_TARRAY(FString,Z_Param_Data);
	P_GET_PROPERTY(FStrProperty,Z_Param_FilePath);
	P_FINISH;
	P_NATIVE_BEGIN;
	UBPFL::OverrideVertexColors(Z_Param_Component,Z_Param_Data,Z_Param_FilePath);
	P_NATIVE_END;
}
// End Class UBPFL Function OverrideVertexColors

// Begin Class UBPFL
void UBPFL::StaticRegisterNativesUBPFL()
{
	UClass* Class = UBPFL::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "OverrideVertexColors", &UBPFL::execOverrideVertexColors },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UBPFL);
UClass* Z_Construct_UClass_UBPFL_NoRegister()
{
	return UBPFL::StaticClass();
}
struct Z_Construct_UClass_UBPFL_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "BPFL.h" },
		{ "ModuleRelativePath", "Public/BPFL.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UBPFL_OverrideVertexColors, "OverrideVertexColors" }, // 1350733183
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UBPFL>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UBPFL_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UBlueprintFunctionLibrary,
	(UObject* (*)())Z_Construct_UPackage__Script_UEFormat,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UBPFL_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UBPFL_Statics::ClassParams = {
	&UBPFL::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	0,
	0,
	0x001000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UBPFL_Statics::Class_MetaDataParams), Z_Construct_UClass_UBPFL_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UBPFL()
{
	if (!Z_Registration_Info_UClass_UBPFL.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UBPFL.OuterSingleton, Z_Construct_UClass_UBPFL_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UBPFL.OuterSingleton;
}
template<> UEFORMAT_API UClass* StaticClass<UBPFL>()
{
	return UBPFL::StaticClass();
}
UBPFL::UBPFL(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UBPFL);
UBPFL::~UBPFL() {}
// End Class UBPFL

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_Marcel_Documents_3D_Unreal_Projects_Main_UUTesting_Plugins_UEFormat_Source_UEFormat_Public_BPFL_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UBPFL, UBPFL::StaticClass, TEXT("UBPFL"), &Z_Registration_Info_UClass_UBPFL, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UBPFL), 2875682410U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_Marcel_Documents_3D_Unreal_Projects_Main_UUTesting_Plugins_UEFormat_Source_UEFormat_Public_BPFL_h_350977530(TEXT("/Script/UEFormat"),
	Z_CompiledInDeferFile_FID_Users_Marcel_Documents_3D_Unreal_Projects_Main_UUTesting_Plugins_UEFormat_Source_UEFormat_Public_BPFL_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_Marcel_Documents_3D_Unreal_Projects_Main_UUTesting_Plugins_UEFormat_Source_UEFormat_Public_BPFL_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
