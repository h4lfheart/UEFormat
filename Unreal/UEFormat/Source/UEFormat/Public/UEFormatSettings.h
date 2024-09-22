// Copyright notice here
#pragma once
#include "CoreMinimal.h"
#include "UEFormatSettings.generated.h"

// which file it should be saved to
UCLASS(config = UEFormat)
class UEFORMAT_API UUEFormatSettings : public UObject
{
	GENERATED_BODY()

public:
	UUEFormatSettings(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(EditAnywhere, Config, Category = "UEFormat")
	bool bCreateMaterials;
};