#pragma once
#include "PropertyCustomizationHelpers.h"
#include "Components/Widget.h"
#include "MMObjectPropertyEntryBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMMObjectPropertyEntryChanged, UObject*, NewObject);
DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(bool, FOnMMObjectPropertyShowAsset, const FAssetData&, AssetData);
DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(bool, FOnMMObjectPropertyShowActor, const AActor*, Actor);

UCLASS(Hidden)
class UMMObjectPropertyEntryBox : public UWidget
{
	GENERATED_BODY()

public:

	TSharedPtr<SObjectPropertyEntryBox> EntryBox;

	/** Class that is allowed in the asset picker */
	UPROPERTY(EditAnywhere, Category = "Object Property Entry Box")
	const UClass* AllowedClass;

	/** Whether the asset can be 'None' */
	UPROPERTY(EditAnywhere, Category = "Object Property Entry Box")
	bool bAllowClear = true;

	/** Whether the asset can be created from the asset picker directly */
	UPROPERTY(EditAnywhere, Category = "Object Property Entry Box")
	bool bAllowCreate = false;

	/** Whether to show the 'Use Selected' button */
	UPROPERTY(EditAnywhere, Category = "Object Property Entry Box")
	bool bDisplayUseSelected = true;

	/** Whether to show the 'Browse' button */
	UPROPERTY(EditAnywhere, Category = "Object Property Entry Box")
	bool bDisplayBrowse = true;

	/** Whether to enable the content Picker */
	UPROPERTY(EditAnywhere, Category = "Object Property Entry Box")
	bool bEnableContentPicker = true;

	/** Whether or not to display a smaller, compact size for the asset thumbnail */ 
	UPROPERTY(EditAnywhere, Category = "Object Property Entry Box")
	bool bDisplayCompactSize = false;

	/** Whether or not to display the asset thumbnail */ 
	UPROPERTY(EditAnywhere, Category = "Object Property Entry Box")
	bool bDisplayThumbnail = true;

	/** Whether or not to display the asset thumbnail */ 
	UPROPERTY(EditAnywhere, Category = "Object Property Entry Box")
	FIntPoint ThumbnailSize = FIntPoint(48, 48);

	UPROPERTY(BlueprintReadWrite, Category = "Object Property Entry Box")
	TWeakObjectPtr<UObject> Object;

	UPROPERTY(BlueprintAssignable, Category="Object Property Entry Box")
	FOnMMObjectPropertyEntryChanged OnObjectChanged;

	UPROPERTY(BlueprintReadWrite, Category="Object Property Entry Box")
	FOnMMObjectPropertyShowAsset ShouldShowAsset;

	UPROPERTY(BlueprintReadWrite, Category="Object Property Entry Box")
	FOnMMObjectPropertyShowActor ShouldShowActor;
	
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	virtual TSharedRef<SWidget> RebuildWidget() override;

};