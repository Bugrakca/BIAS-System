// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SItemDataStructs.h"
#include "SInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotUpdate, int32, Index);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTNERIO_API USInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> WidgetClass;

	UPROPERTY(BlueprintReadOnly)
	int32 MaxStackSize;

	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY()
	bool bIsMouseVisible;
	
	// Sets default values for this component's properties
	USInventoryComponent();

	virtual void BeginPlay() override;
	
	void InitList(TArray<FSlotData>& Array, const int32 Size);

	void SpawnDroppedItem(const FSlotData& SlotData, int32 Quantity);
	
	int32 FindFirstAvailableSlot() const;

	int32 FindNextStackableItem(const FItemData& StackableItem);

	UFUNCTION(BlueprintCallable)
	void ToggleInventory();

public:
	UPROPERTY(BlueprintReadOnly)
	TArray<FSlotData> InventoryArray;

	UPROPERTY()
	TArray<FSlotData> WeaponData;

	UPROPERTY()
	TArray<FSlotData> ArmorData;

	UPROPERTY()
	FText Weapon;

	UPROPERTY()
	FText Armor;
	
	UPROPERTY(BlueprintAssignable)
	FOnSlotUpdate OnSlotUpdate;

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* InventoryWidget;

	UFUNCTION(BlueprintCallable)
	bool IsIndexEmpty(const int32 Index);

	UFUNCTION(BlueprintCallable)
	FSlotData& GetItemAtIndex(const int32 Index);

	void Add(const FItemData& Item, const int32 Quantity);

	int32 CreateNewItem(const FItemData& Item, int32 Quantity);

	void SwapItems(int32 SourceIndex, int32 DroppedIndex);
	
	bool DropItem(int32 SlotIndex, int32 Quantity);

	void StackItems(int32 DraggedIndex, int32 DroppedIndex);
};
