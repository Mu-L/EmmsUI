#include "MMEditorUtilityTab.h"
#include "Widgets/Docking/SDockTab.h"
#include "Components/VerticalBox.h"
#include "Framework/Docking/TabManager.h"
#include "MMWidget.h"
#include "EmmsStatics.h"
#include "EmmsSlotHelpers.h"
#include "Editor.h"

UWorld* UMMEditorUtilityTab::GetWorld() const
{
	if (World != nullptr)
		return World;
	else
		return GEditor->GetEditorWorldContext().World();
}

bool UMMEditorUtilityTab::IsTabFocused() const
{
	if (!SlateTab.IsValid())
		return false;
	return SlateTab->IsActive();
}

bool UMMEditorUtilityTab::IsTabVisible() const
{
	if (!SlateTab.IsValid())
		return false;
	return SlateTab->IsForeground();
}

void UMMEditorUtilityTab::Spawn()
{
	StrongSelf = TStrongObjectPtr<UMMEditorUtilityTab>(this);

	SlateTab->SetOnTabClosed(
		SDockTab::FOnTabClosedCallback::CreateLambda(
		[this](TSharedRef<SDockTab> ClosedTab)
		{
			if (bOpen)
			{
				bOpen = false;
				FEditorScriptExecutionGuard ScopeAllowScript;
				OnTabClosed();
			}

			StrongSelf.Reset();
			SlateTab.Reset();

			if (ReinstanceHandle.IsValid())
			{
				FCoreUObjectDelegates::OnObjectsReinstanced.Remove(ReinstanceHandle);
				ReinstanceHandle.Reset();
			}
		})
	);

	MMWidget = NewObject<UMMWidget>(this, UMMWidget::StaticClass(), NAME_None, RF_Transient);
	MMWidget->ExternalDrawFunction = [this](UMMWidget* Widget, float DeltaTime)
	{
		FEditorScriptExecutionGuard ScopeAllowScript;
		DrawTab(DeltaTime);
	};

	SlateTab->SetContent(MMWidget->TakeWidget());

	bOpen = true;

	if (!ReinstanceHandle.IsValid())
	{
		ReinstanceHandle = FCoreUObjectDelegates::OnObjectsReinstanced.AddLambda(
		[this](const TMap<UObject*, UObject*>& ReinstancedObjects)
		{
			HandleObjectReinstancing(ReinstancedObjects);
		});
	}

	{
		FEditorScriptExecutionGuard ScopeAllowScript;
		OnTabOpened();
	}
}

void UMMEditorUtilityTab::CloseTab()
{
	if (SlateTab.IsValid())
		SlateTab->RequestCloseTab();

	if (ReinstanceHandle.IsValid())
	{
		FCoreUObjectDelegates::OnObjectsReinstanced.Remove(ReinstanceHandle);
		ReinstanceHandle.Reset();
	}
}

UMMEditorUtilityTab* UMMEditorUtilityTab::SpawnOrFocusTab(TSubclassOf<UMMEditorUtilityTab> TabType)
{
	if (TabType != nullptr)
	{
		TSharedPtr<SDockTab> DockTab = FGlobalTabmanager::Get()->TryInvokeTab(TabType.Get()->GetFName());
		if (!DockTab.IsValid())
			return nullptr;
		
		// This should be the safest way to find the UObject associated with the dock tab, since there's no dynamic casting for slate widgets.
		// There aren't going to be very many of them, so should work fine.
		UMMEditorUtilityTab* MatchingTabObject = nullptr;
		ForEachObjectOfClass(UMMEditorUtilityTab::StaticClass(), [&](UObject* Object)
		{
			UMMEditorUtilityTab* TabObject = CastChecked<UMMEditorUtilityTab>(Object);
			if (TabObject->SlateTab == DockTab)
			{
				MatchingTabObject = TabObject;
			}
		});

		return MatchingTabObject;
	}

	return nullptr;
}

void UMMEditorUtilityTab::BeginDestroy()
{
	Super::BeginDestroy();

	if (ReinstanceHandle.IsValid())
	{
		FCoreUObjectDelegates::OnObjectsReinstanced.Remove(ReinstanceHandle);
		ReinstanceHandle.Reset();
	}
}

void UMMEditorUtilityTab::HandleObjectReinstancing(const TMap<UObject*, UObject*>& ReinstancedObjects)
{
	UObject* const* ReplacementObjectPtr = ReinstancedObjects.Find(this);
	if (ReplacementObjectPtr != nullptr && SlateTab.IsValid())
	{
		UMMEditorUtilityTab* NewTab = Cast<UMMEditorUtilityTab>(*ReplacementObjectPtr);
		if (NewTab != nullptr)
		{
			OnTabClosed();

			NewTab->SlateTab = SlateTab;
			NewTab->Spawn();

			SlateTab.Reset();
			StrongSelf.Reset();
			MMWidget = nullptr;

			if (ReinstanceHandle.IsValid())
			{
				FCoreUObjectDelegates::OnObjectsReinstanced.Remove(ReinstanceHandle);
				ReinstanceHandle.Reset();
			}
		}
	}
}
