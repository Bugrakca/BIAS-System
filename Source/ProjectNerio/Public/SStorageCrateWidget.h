// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SStorageCrateWidget.generated.h"

class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTNERIO_API USStorageCrateWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TestText;
	
	UPROPERTY(meta = (BindWidget))
	UButton* TestButton;

	void SortItemsWithTag();

	UFUNCTION()
	void OnButtonClicked();
	
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
