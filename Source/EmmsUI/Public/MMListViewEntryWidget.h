#pragma once
#include "MMWidget.h"
#include "Components/VerticalBox.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "MMListViewEntryWidget.generated.h"

UCLASS(NotBlueprintable, Hidden)
class EMMSUI_API UMMListViewEntryWidget : public UMMWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:

	UMMListViewEntryWidget(const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
	{
		bDrawOnConstruct = false;
		bAllowDraw = false;
		DefaultRootPanel = UVerticalBox::StaticClass();
	}

	UPROPERTY()
	UObject* ListItem;

	UFUNCTION(ScriptCallable, meta = (DeterminesOutputType = "ItemClass"))
	UObject* GetListItem(const TSubclassOf<UObject>& ItemClass) const;

	UFUNCTION(ScriptCallable)
	int GetItemIndex() const;

	UFUNCTION(ScriptCallable, meta = (ScriptName = "IsListItemSelected"))
	bool AS_IsListItemSelected() const;

	UFUNCTION(ScriptCallable, meta = (ScriptName = "IsListItemExpanded"))
	bool AS_IsListItemExpanded() const;

	UFUNCTION(ScriptCallable, meta = (ScriptName = "SetListItemExpanded"))
	void AS_SetListItemExpanded(bool bExpanded);

	UFUNCTION(ScriptCallable, meta = (ScriptName = "GetOwningListView"))
	UListViewBase* AS_GetOwningListView() const;

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void OnRootWidgetChanged() override;
};

UCLASS(NotBlueprintable, NotBlueprintType)
class EMMSUI_API UMMListViewEmptyItemObject : public UObject
{
	GENERATED_BODY()

public:

};