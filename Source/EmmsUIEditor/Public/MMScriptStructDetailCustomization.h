#pragma once
#include "CoreMinimal.h"
#include "Layout/Visibility.h"
#include "IPropertyTypeCustomization.h"
#include "ClassGenerator/ASStruct.h"
#include "MMWidget.h"
#include "StructUtils/InstancedStruct.h"
#include "AngelscriptAnyStructParameter.h"
#include "MMScriptStructDetailCustomization.generated.h"

class UMMScriptStructDetailCustomization;

class FMMScriptStructDetailCustomizationWrapper : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance(TSubclassOf<UMMScriptStructDetailCustomization> CustomizationClass);

	virtual ~FMMScriptStructDetailCustomizationWrapper();

	/** IPropertyTypeCustomization instance */
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, class IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils ) override;

	/** Cached property handle */
	TSharedPtr<IPropertyHandle> StructPropertyHandle;
	TStrongObjectPtr<UMMScriptStructDetailCustomization> Customization;

	FDetailWidgetRow* ActiveRow = nullptr;
	IPropertyTypeCustomizationUtils* ActiveUtils = nullptr;
	IDetailChildrenBuilder* ActiveChildBuilder = nullptr;

};

UCLASS(BlueprintType)
class UMMScriptStructDetailCustomization : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:

	UMMScriptStructDetailCustomization(const FObjectInitializer& Initializer)
		: Super(Initializer)
		, FTickableGameObject(IsTemplate() ? ETickableTickType::Never : ETickableTickType::NewObject)
	{}

	UPROPERTY(EditDefaultsOnly, Category = "Detail Customization")
	UScriptStruct* DetailStruct = nullptr;

	UPROPERTY()
	TArray<UMMWidget*> UsedImmediateWidgets;

	TArray<TWeakObjectPtr<UObject>> OuterObjects;
	FMMScriptStructDetailCustomizationWrapper* Wrapper = nullptr;

	/** Get all outer objects that are containing the struct that is being edited. */
	UFUNCTION(ScriptCallable)
	const TArray<TWeakObjectPtr<UObject>>& GetOuterObjects() const
	{
		return OuterObjects;
	}

	/** Get the first outer object that contains the struct being edited. */
	UFUNCTION(ScriptCallable)
	UObject* GetOuterObject() const
	{
		if (OuterObjects.Num() != 0)
			return OuterObjects[0].Get();
		return nullptr;
	}

	/**
	 * Check whether the property we're displaying that contains this struct has a specific metadata set on it.
	 */
	UFUNCTION(ScriptCallable)
	bool ContainingPropertyHasMetaData(FName MetaData);

	/**
	 * Get metadata for the property we're displaying that contains this struct.
	 */
	UFUNCTION(ScriptCallable)
	FString GetContainingPropertyMetaData(FName MetaData);

	// Set the content of the name column to the default content (name of the property)
	UFUNCTION(ScriptCallable)
	void DefaultNameContent();
	// Set the content of the name column to a custom immediate drawer widget
	UFUNCTION(ScriptCallable)
	UMMWidget* ImmediateNameContent(EHorizontalAlignment HAlign = EHorizontalAlignment::HAlign_Left, EVerticalAlignment VAlign = EVerticalAlignment::VAlign_Center);

	// Set the content of the value column to the default content (value of the property)
	UFUNCTION(ScriptCallable)
	void DefaultValueContent();
	// Set the content of the value column to a custom immediate drawer widget
	UFUNCTION(ScriptCallable)
	UMMWidget* ImmediateValueContent(EHorizontalAlignment HAlign = EHorizontalAlignment::HAlign_Left, EVerticalAlignment VAlign = EVerticalAlignment::VAlign_Center);

	// Set the content of the entire row to the default content (name and value columns)
	UFUNCTION(ScriptCallable)
	void DefaultWholeRowContent();
	// Set the content of the entire row to a custom immediate drawer widget
	UFUNCTION(ScriptCallable)
	UMMWidget* ImmediateWholeRowContent();

	// Add all properties to be displayed as child rows
	UFUNCTION(ScriptCallable)
	void AddAllPropertyChildRows();
	// Add a specific property to be displayed as a child row
	UFUNCTION(ScriptCallable)
	void AddPropertyChildRow(FName PropertyName);
	// Add a custom immediate drawer as a named property
	UFUNCTION(ScriptCallable)
	UMMWidget* AddImmediateChildProperty(FString PropertyName, EHorizontalAlignment HAlign = EHorizontalAlignment::HAlign_Left, EVerticalAlignment VAlign = EVerticalAlignment::VAlign_Center);
	// Add a custom immediate drawer as a whole child row
	UFUNCTION(ScriptCallable)
	UMMWidget* AddImmediateChildRow(EHorizontalAlignment HAlign = EHorizontalAlignment::HAlign_Fill, EVerticalAlignment VAlign = EVerticalAlignment::VAlign_Center);

	// Check whether a valid struct value is available
	UFUNCTION(ScriptCallable, meta=(DeterminesOutputType = "StructType"))
	bool HasStructValue();

	// Get the current value being displayed in the details panel
	UFUNCTION(ScriptCallable, meta=(DeterminesOutputType = "StructType"))
	const FScriptStructWildcard& GetStructValue(UScriptStruct* StructType);

	// Change the value of the property being displayed in the details panel
	UFUNCTION(ScriptCallable)
	void SetStructValue(FAngelscriptAnyStructParameter NewValue);

	UFUNCTION(BlueprintImplementableEvent)
	void CustomizeHeader();

	UFUNCTION(BlueprintImplementableEvent)
	void CustomizeChildren();

	UFUNCTION(BlueprintImplementableEvent)
	void OnRemoved();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_Tick(float DeltaTime);

	UWorld* GetWorld() const;

	void DeleteCustomization();
	void FinishCustomizingHeader();
	void FinishCustomizingChildren();

	virtual bool IsTickableInEditor() const override { return true; }
	virtual bool IsTickableWhenPaused() const override { return true; }
	virtual UWorld* GetTickableGameObjectWorld() const override { return GetWorld(); }
	virtual bool IsTickable() const override { return !HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject) && Wrapper != nullptr; }

	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;

};
