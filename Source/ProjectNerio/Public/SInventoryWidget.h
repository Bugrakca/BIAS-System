// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "SInventoryWidget.generated.h"

class USInventoryComponent;
class USInventorySlotWidget;
class UTextBlock;
class UUniformGridPanel;
/**
 * 
 */
UCLASS()
class PROJECTNERIO_API USInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeOnInitialized() override;
	
	virtual void NativeConstruct() override;
	
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USInventoryComponent> InventoryComp;

	UPROPERTY(EditAnywhere, Category = "Inventory Panel")
	int32 SlotsPerRow;
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory Panel")
	TSubclassOf<UUserWidget> SlotWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory Panel")
	TArray<UUserWidget*> SlotWidgets;
	
	UPROPERTY(BlueprintReadOnly, Category = "Inventory Panel", meta = (BindWidget))
	TObjectPtr<UTextBlock> InventoryLabel;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory Panel", meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> SlotGrid;

	UFUNCTION()
	void OnSlotUpdated(int32 UpdatedIndex);
	
	UFUNCTION(BlueprintCallable)
	void GenerateSlotWidgets();

	int32 FindColumn(int32 ArrayIndex);

	int32 FindRow(int32 ArrayIndex);
};
