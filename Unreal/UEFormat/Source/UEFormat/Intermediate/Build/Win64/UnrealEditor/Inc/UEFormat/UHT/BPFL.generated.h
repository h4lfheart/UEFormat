// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "BPFL.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class AActor;
class UMaterialInterface;
class UObject;
class USceneComponent;
class UStaticMeshComponent;
#ifdef UEFORMAT_BPFL_generated_h
#error "BPFL.generated.h already included, missing '#pragma once' in BPFL.h"
#endif
#define UEFORMAT_BPFL_generated_h

#define FID_Users_Marcel_Documents_GitRepositories_UniversalUmap_UniversalUmap_Unreal_UEFormat_Source_UEFormat_Public_BPFL_h_10_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execOverrideVertexColors); \
	DECLARE_FUNCTION(execAddComponentToActor); \
	DECLARE_FUNCTION(execSpawnGenericActorWithoutRoot); \
	DECLARE_FUNCTION(execSetMaterial);


#define FID_Users_Marcel_Documents_GitRepositories_UniversalUmap_UniversalUmap_Unreal_UEFormat_Source_UEFormat_Public_BPFL_h_10_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUBPFL(); \
	friend struct Z_Construct_UClass_UBPFL_Statics; \
public: \
	DECLARE_CLASS(UBPFL, UBlueprintFunctionLibrary, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/UEFormat"), NO_API) \
	DECLARE_SERIALIZER(UBPFL)


#define FID_Users_Marcel_Documents_GitRepositories_UniversalUmap_UniversalUmap_Unreal_UEFormat_Source_UEFormat_Public_BPFL_h_10_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UBPFL(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UBPFL(UBPFL&&); \
	UBPFL(const UBPFL&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UBPFL); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UBPFL); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UBPFL) \
	NO_API virtual ~UBPFL();


#define FID_Users_Marcel_Documents_GitRepositories_UniversalUmap_UniversalUmap_Unreal_UEFormat_Source_UEFormat_Public_BPFL_h_7_PROLOG
#define FID_Users_Marcel_Documents_GitRepositories_UniversalUmap_UniversalUmap_Unreal_UEFormat_Source_UEFormat_Public_BPFL_h_10_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Users_Marcel_Documents_GitRepositories_UniversalUmap_UniversalUmap_Unreal_UEFormat_Source_UEFormat_Public_BPFL_h_10_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Users_Marcel_Documents_GitRepositories_UniversalUmap_UniversalUmap_Unreal_UEFormat_Source_UEFormat_Public_BPFL_h_10_INCLASS_NO_PURE_DECLS \
	FID_Users_Marcel_Documents_GitRepositories_UniversalUmap_UniversalUmap_Unreal_UEFormat_Source_UEFormat_Public_BPFL_h_10_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> UEFORMAT_API UClass* StaticClass<class UBPFL>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Users_Marcel_Documents_GitRepositories_UniversalUmap_UniversalUmap_Unreal_UEFormat_Source_UEFormat_Public_BPFL_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
