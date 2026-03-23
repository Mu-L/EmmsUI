#include "MMObjectPropertyEntryBox.h"
#include "ThumbnailRendering/ThumbnailManager.h"

void UMMObjectPropertyEntryBox::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	EntryBox.Reset();
}

TSharedRef<SWidget> UMMObjectPropertyEntryBox::RebuildWidget()
{
	const UClass* DisplayClass = AllowedClass;
	if (DisplayClass == nullptr)
		DisplayClass = UObject::StaticClass();

	EntryBox = SNew(SObjectPropertyEntryBox)
		.AllowedClass(DisplayClass)
		.AllowClear(bAllowClear)
		.AllowCreate(bAllowCreate)
		.DisplayUseSelected(bDisplayUseSelected)
		.DisplayBrowse(bDisplayBrowse)
		.EnableContentPicker(bEnableContentPicker)
		.DisplayCompactSize(bDisplayCompactSize)
		.DisplayThumbnail(bDisplayThumbnail)
		.ThumbnailPool(UThumbnailManager::Get().GetSharedThumbnailPool())
		.ThumbnailSizeOverride(ThumbnailSize)
		.ObjectPath_Lambda([this]()
		{
			if (Object.IsValid())
				return Object->GetPathName();
			else
				return FString();
		})
		.OnObjectChanged_Lambda([this](const FAssetData& AssetData)
		{
			Object = AssetData.GetAsset();

			FEditorScriptExecutionGuard ScopeAllowScript;
			OnObjectChanged.Broadcast(Object.Get());
		})
		.OnShouldFilterAsset_Lambda([this](const FAssetData& AssetData)
		{
			FEditorScriptExecutionGuard ScopeAllowScript;
			if (ShouldShowAsset.IsBound())
				return !ShouldShowAsset.Execute(AssetData);
			else
				return false;
		})
		.OnShouldFilterActor_Lambda([this](const AActor* Actor)
		{
			// NB: ShouldFilterActor works the exact opposite of ShouldFilterAsset
			// That is, for ShouldFilterAsset Filter=true means HIDE
			// For ShouldFilterActor Filter=true means SHOW
			FEditorScriptExecutionGuard ScopeAllowScript;
			if (ShouldShowActor.IsBound())
				return ShouldShowActor.Execute(Actor);
			else
				return true;
		})
	;

	return EntryBox.ToSharedRef();
}
