#pragma once
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "MMRootVerticalBox.generated.h"

UCLASS(Hidden)
class UMMRootVerticalBox : public UVerticalBox
{
	GENERATED_BODY()

public:

	virtual void OnSlotAdded(UPanelSlot* InSlot) override
	{
		if (InSlot != nullptr)
		{
			CastChecked<UVerticalBoxSlot>(InSlot)->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Left);
			CastChecked<UVerticalBoxSlot>(InSlot)->SetPadding(FMargin(4, 2));
		}

		Super::OnSlotAdded(InSlot);
	}
};