#include "SInventoryComponent.h"

#include "SStorageCrateWidget.h"

// Sets default values for this component's properties
USInventoryComponent::USInventoryComponent()
{
	InventoryWidget = nullptr;
	
	MaxStackSize = 999;

	Weapon = FText::FromString("Weapon");

	Armor = FText::FromString("Armor");
}

void USInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	InitList(InventoryArray, 40);

	if (WidgetClass)
	{
		InventoryWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
		UE_LOG(LogTemp, Log, TEXT("Create widget"));
		InventoryWidget->AddToViewport();
		InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USInventoryComponent::Add(const FItemData& Item, const int32 Quantity)
{
	int32 RemainingQuantity = Quantity;

	// First, try to stack with existing items
	for (int32 SlotIndex = 0; SlotIndex < InventoryArray.Num(); ++SlotIndex)
	{
		FSlotData& CurrentSlot = InventoryArray[SlotIndex];
        
		if (CurrentSlot.Item.Name.EqualTo(Item.Name) && Item.bIsStackable)
		{
			int32 AvailableSpaceInSlot = MaxStackSize - CurrentSlot.Quantity;
			if (AvailableSpaceInSlot > 0)
			{
				int32 QuantityToAddToSlot = FMath::Min(RemainingQuantity, AvailableSpaceInSlot);
				CurrentSlot.Quantity += QuantityToAddToSlot;
				RemainingQuantity -= QuantityToAddToSlot;
                
				OnSlotUpdate.Broadcast(SlotIndex);

				if (RemainingQuantity == 0)
				{
					UE_LOG(LogTemp, Log, TEXT("All items have been added."));
					return; // All items have been added
				}
			}
		}
	}

	// If there are still items remaining, create new stacks
	while (RemainingQuantity > 0)
	{
		int32 QuantityForNewSlot = FMath::Min(RemainingQuantity, MaxStackSize);
		int32 NewSlotIndex = CreateNewItem(Item, QuantityForNewSlot);
        
		if (NewSlotIndex != INDEX_NONE)
		{
			RemainingQuantity -= QuantityForNewSlot;
			OnSlotUpdate.Broadcast(NewSlotIndex);
		}
		else
		{
			// Inventory is full
			UE_LOG(LogTemp, Warning, TEXT("Inventory is full. Couldn't add %d %s"), 
				   RemainingQuantity, *Item.Name.ToString());
			break;
		}
	}
}

// Get the index when mouse is clicked to the icon(button) - drag and drop or right click for all the items
void USInventoryComponent::DropItem(const FItemData& Item, int32 Quantity)
{
	const int32 Index = InventoryArray.IndexOfByPredicate([&Item](const FSlotData& DataElement)
	{
		return DataElement.Item.Name.EqualTo(Item.Name);
	});

	if (InventoryArray.IsValidIndex(Index)) // Check this, even if it is valid
	{
		FSlotData& ItemData = InventoryArray[Index];

		if (Index != INDEX_NONE)
		{
			UE_LOG(LogTemp, Log, TEXT("ItemData is not null"));
			ItemData.Quantity -= Quantity;

			OnSlotUpdate.Broadcast(Index);
			// TODO: If all of the item quantity dropped. Clear the slot (Replace it with an empty slot).
			// FSlotData EmptyItem{};
			//
			// if (ItemData.Quantity <= 0)
			// {
			// 	ItemData = EmptyItem;
			// }
		}
	}
}

void USInventoryComponent::ShowInventory()
{
	if (InventoryWidget)
	{
		UE_LOG(LogTemp, Log, TEXT("Inventory Is Visible"));
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void USInventoryComponent::HideInventory()
{
	if (InventoryWidget)
	{
		UE_LOG(LogTemp, Log, TEXT("Inventory Is Invisible"));
		InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

bool USInventoryComponent::IsIndexEmpty(const int32 Index)
{
	UE_LOG(LogTemp, Log, TEXT("This is the InventoryComp IsIndexEmpty"));
	return !IsValid(InventoryArray[Index].Item.ItemClass);
}

FSlotData& USInventoryComponent::GetItemAtIndex(const int32 Index)
{
	UE_LOG(LogTemp, Log, TEXT("This is the InventoryComp GetItemAtIndex"));
	return InventoryArray[Index];
}

void USInventoryComponent::InitList(TArray<FSlotData>& Array, const int32 Size)
{
	// const FItemData EmptyItemData{};
	FSlotData EmptyItem{};

	for (int32 Index = 0; Index < Size; ++Index)
	{
		EmptyItem.SlotIndex = Index;
		Array.Add(EmptyItem);
	}
	UE_LOG(LogTemp, Log, TEXT("Init The Inventory Array With Empty Struct"));
}

int32 USInventoryComponent::CreateNewItem(const FItemData& Item, const int32 Quantity)
{
	UE_LOG(LogTemp, Log, TEXT("This is the InventoryComp CreateNewItem"));

	if (const int32 AvailableSlot = FindFirstAvailableSlot(); AvailableSlot != INDEX_NONE)
	{
		const FSlotData NewItem(Item, Quantity, AvailableSlot);

		InventoryArray[AvailableSlot] = NewItem;
	
		return AvailableSlot;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("No empty slot found in inventory"));
	return INDEX_NONE;
}

int32 USInventoryComponent::FindFirstAvailableSlot() const
{
	for (int32 EmptySlotIndex = 0; EmptySlotIndex < InventoryArray.Num(); ++EmptySlotIndex)
	{
		if (InventoryArray[EmptySlotIndex].Item.Name.IsEmpty() || InventoryArray[EmptySlotIndex].Quantity == 0)
		{
			return EmptySlotIndex;
		}
	}

	return INDEX_NONE;
}

int32 USInventoryComponent::FindNextStackableItem(const FItemData& StackableItem)
{
	for (FSlotData& FoundItem : InventoryArray)
	{
		if (FoundItem.Item.Name.EqualTo(StackableItem.Name) && FoundItem.Item.bIsStackable)
		{
			const int32 AvailableSpaceInSlot = MaxStackSize - FoundItem.Quantity;
			
			if (AvailableSpaceInSlot > 0)
			{
				return FoundItem.SlotIndex;
			}
		}
	}
	// All slots are full.
	return INDEX_NONE;
}

TArray<FSlotData> USInventoryComponent::FindSimilarItems(TArray<FSlotData>& ItemArray, const FItemData& Item)
{
	TArray<FSlotData> FoundItems;

	for (FSlotData& FoundItem : ItemArray)
	{
		if (FoundItem.Item.Name.EqualTo(Item.Name))
		{
			FoundItems.Add(FoundItem);
		}
	}

	return FoundItems;
}
