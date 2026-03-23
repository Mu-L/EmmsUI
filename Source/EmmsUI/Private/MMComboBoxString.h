#pragma once
#include "Components/ComboBoxString.h"
#include "MMComboBoxString.generated.h"

UCLASS(Hidden)
class UMMComboBoxString : public UComboBoxString
{
	GENERATED_BODY()

public:
	
	TArray<FString> ActiveOptions;
	FString DefaultSelectedItem;
};