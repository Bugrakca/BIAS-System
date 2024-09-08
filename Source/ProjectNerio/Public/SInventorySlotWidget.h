// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SInventorySlotWidget.generated.h"

class UTextBlock;
class UImage;
class UButton;
class USInventoryComponent;
class USInventoryWidget;
/**
 * 
 */
UCLASS()
class PROJECTNERIO_API USInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> SlotButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> SlotIcon;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> AmountText;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USInventoryComponent> InventoryComp;

	UPROPERTY()
	int32 UISlotIndex;

	// Functions
	virtual void NativeConstruct() override;

	virtual void NativeOnInitialized() override;
	
	void GetTextAmountVisibility(int32 Index) const;

public:
	UFUNCTION()
	void UpdateSlot() const;

	UFUNCTION()
	void SetSlotIndex(int32 Index);

};
