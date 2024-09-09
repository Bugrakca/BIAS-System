#include "SInventoryComponent.h"
#include "SStorageCrateWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Framework/Application/NavigationConfig.h"
#include "Kismet/GameplayStatics.h"

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

	PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = false;
		PlayerController->SetInputMode(FInputModeGameOnly());
	}

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
	// TODO: Refactor for Drag and drop system.
	// TODO: When dropping items into another slot, swap the slots if its not empty.
	int32 RemainingQuantity = Quantity;

	// Try to stack with existing items
	for (int32 SlotIndex = 0; SlotIndex < InventoryArray.Num(); ++SlotIndex)
	{
		FSlotData& CurrentSlot = InventoryArray[SlotIndex];

		if (Item.bIsStackable && CurrentSlot.Item.Name.EqualTo(Item.Name))
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
	// TODO: Drag and drop system. Select how many items you want to drop.
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

void USInventoryComponent::ToggleInventory()
{
	if (PlayerController)
	{
		bIsMouseVisible = !bIsMouseVisible;

		UE_LOG(LogTemp, Log, TEXT("ToggleInventory called. bIsMouseVisible is now: %s"),
		       bIsMouseVisible ? TEXT("true") : TEXT("false"));

		if (bIsMouseVisible)
		{
			PlayerController->bShowMouseCursor = true;
			PlayerController->SetInputMode(FInputModeGameAndUI());
			FSlateApplication::Get().GetNavigationConfig()->bTabNavigation = false;

			if (InventoryWidget)
			{
				UE_LOG(LogTemp, Log, TEXT("Inventory Is Visible"));
				InventoryWidget->SetVisibility(ESlateVisibility::Visible);
			}
		}
		else
		{
			PlayerController->bShowMouseCursor = false;
			PlayerController->SetInputMode(FInputModeGameOnly());

			if (InventoryWidget)
			{
				UE_LOG(LogTemp, Log, TEXT("Inventory Is Invisible"));
				InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
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
		// const FSlotData NewItem(Item, Quantity, AvailableSlot);

		InventoryArray[AvailableSlot] = FSlotData(Item, Quantity, AvailableSlot);

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
