// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "SItemDrag.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNERIO_API USItemDrag : public UDragDropOperation
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UUserWidget> DraggedWidgetReference;
	
	UPROPERTY(BlueprintReadOnly, Category = "Drag and Drop")
	int32 SlotIndex;

	UPROPERTY(BlueprintReadOnly, Category = "Drag and Drop")
	UTexture2D* ItemImage;

	UPROPERTY()
	FVector2D DragOffset;
};
