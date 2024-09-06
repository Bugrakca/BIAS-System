// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SItemDataStructs.generated.h"

USTRUCT(BlueprintType)
struct FItemData //: public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText ItemCategory;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsStackable;
};

USTRUCT(BlueprintType)
struct FSlotData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FItemData Item;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 SlotIndex;
};
