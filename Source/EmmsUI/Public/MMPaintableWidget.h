#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Styling/SlateBrush.h"
#include "MMWidget.h"
#include "MMPaintableWidget.generated.h"

/**
 * Helper widget that can be used to paint arbitrary lines and brushes in immediate mode.
 */
UCLASS(NotBlueprintable, Hidden)
class EMMSUI_API UMMPaintableWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	enum class EPaintableDrawType : uint8
	{
		Box,
		Spline,
	};

	UPROPERTY()
	UMMWidget* ContainingWidget = nullptr;

	uint64 LastMMWidgetDrawCount = 0;
	TArray<TFunction<void(FPaintContext&)>> Draws;

	void ConditionalStartDraws();

	void DrawBox(FVector2D Position, FVector2D Size, const FSlateBrush* Brush, FLinearColor Tint);
	void DrawLines(const TArray<FVector2D>& Points, FLinearColor Tint, bool bAntiAlias, float Thickness);
	void DrawText(const FString& String, FVector2D Position, const FSlateFontInfo& Font, FLinearColor Tint, FVector2D Alignment);
	void DrawRing(FVector2D Position, float Radius, FLinearColor Tint, float Thickness);
	void DrawCircle(FVector2D Position, float Radius, FLinearColor Tint);

	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled ) const;

};
