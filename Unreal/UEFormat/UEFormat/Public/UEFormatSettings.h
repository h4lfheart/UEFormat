#include "CoreMinimal.h"
#include "UEFormatSettings.generated.h"

UCLASS(config = Editor, defaultconfig)
class UEFORMAT_API UUEFormatSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UUEFormatSettings(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer) {
		bCreateMaterials = true;
	}
	
	UPROPERTY(EditAnywhere, config, Category = "UEFormat")
	bool bCreateMaterials;
};