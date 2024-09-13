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
bool USInventoryComponent::DropItem(int32 SlotIndex, int32 Quantity)
{
	// TODO: Drag and drop system. Select how many items you want to drop.
	
	if (SlotIndex >= 0 && SlotIndex < InventoryArray.Num() && !IsIndexEmpty(SlotIndex))
	{
		FSlotData& SlotData = InventoryArray[SlotIndex];
		if (SlotData.Quantity >= Quantity)
		{
			SpawnDroppedItem(SlotData, Quantity);
			SlotData.Quantity -= Quantity;
            
			if (SlotData.Quantity <= 0)
			{
				// Clear the slot if all items are dropped
				InventoryArray[SlotIndex] = FSlotData();
				InventoryArray[SlotIndex].SlotIndex = SlotIndex;
			}
			return true;
		}
	}
	return false;
}

void USInventoryComponent::StackItems(int32 DraggedIndex, int32 DroppedIndex)
{
	// Stack items with drag and drop
	FSlotData& DraggedItem = InventoryArray[DraggedIndex];
	FSlotData& DroppedItem = InventoryArray[DroppedIndex];

	int32 RemainingQuantity = DraggedItem.Quantity;

	if (DraggedItem.Item.Name.EqualTo(DroppedItem.Item.Name) && DraggedItem.Item.bIsStackable && DroppedItem.Item.bIsStackable)
	{
		int32 SumQuantity = DraggedItem.Quantity + DroppedItem.Quantity;
		int32 AvailableSpaceInSlot = MaxStackSize - SumQuantity;
		
		if (AvailableSpaceInSlot > 0)
		{
			int32 QuantityToAddToSlot = FMath::Min(RemainingQuantity, AvailableSpaceInSlot);
			DroppedItem.Quantity += DraggedItem.Quantity;
			RemainingQuantity -= QuantityToAddToSlot;

			OnSlotUpdate.Broadcast(DroppedIndex);

			if (RemainingQuantity == 0)
			{
				UE_LOG(LogTemp, Log, TEXT("All items have been added."));
				DraggedItem = FSlotData{};
				OnSlotUpdate.Broadcast(DraggedIndex);
				return; // All items have been added
			}
		}
	}

	// while (RemainingQuantity > 0)
	// {
	// 	int32 QuantityForNewSlot = FMath::Min(RemainingQuantity, MaxStackSize);
	// 	int32 NewSlotIndex = CreateNewItem(Item, QuantityForNewSlot);
	//
	// 	if (NewSlotIndex != INDEX_NONE)
	// 	{
	// 		RemainingQuantity -= QuantityForNewSlot;
	// 		OnSlotUpdate.Broadcast(NewSlotIndex);
	// 	}
	// 	else
	// 	{
	// 		// Inventory is full
	// 		UE_LOG(LogTemp, Warning, TEXT("Inventory is full. Couldn't add %d %s"),
	// 			   RemainingQuantity, *Item.Name.ToString());
	// 		break;
	// 	}
	// }
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
			
			UE_LOG(LogTemp, Log, TEXT("Inventory Is Visible"));
			InventoryWidget->SetVisibility(ESlateVisibility::Visible);
			
		}
		else
		{
			PlayerController->bShowMouseCursor = false;
			PlayerController->SetInputMode(FInputModeGameOnly());
			
			UE_LOG(LogTemp, Log, TEXT("Inventory Is Invisible"));
			InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
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
	FSlotData EmptyItem{};

	for (int32 Index = 0; Index < Size; ++Index)
	{
		EmptyItem.SlotIndex = Index;
		Array.Add(EmptyItem);
	}
	UE_LOG(LogTemp, Log, TEXT("Init The Inventory Array With Empty Struct"));
}

void USInventoryComponent::SpawnDroppedItem(const FSlotData& SlotData, int32 Quantity)
{
	if (GetWorld() && SlotData.Item.ItemClass)
	{
		UE_LOG(LogTemp, Log, TEXT("Spawned Item Quantity: %d"), Quantity);
		FVector SpawnLocation = GetOwner()->GetActorLocation() + FVector(100, 0, 0); // Offset from the player
		FRotator SpawnRotation = FRotator::ZeroRotator;
        
		//ADroppedItem* DroppedItem = GetWorld()->SpawnActor<ADroppedItem>(SlotData.Item.ItemClass, SpawnLocation, SpawnRotation);
		// if (DroppedItem)
		// {
		// 	DroppedItem->SetQuantity(Quantity);
		// 	DroppedItem->SetItemData(SlotData.Item);
		// }
	}
}

int32 USInventoryComponent::CreateNewItem(const FItemData& Item, const int32 Quantity)
{
	UE_LOG(LogTemp, Log, TEXT("This is the InventoryComp CreateNewItem"));

	if (const int32 AvailableSlot = FindFirstAvailableSlot(); AvailableSlot != INDEX_NONE)
	{
		InventoryArray[AvailableSlot] = FSlotData(Item, Quantity, AvailableSlot);

		return AvailableSlot;
	}

	UE_LOG(LogTemp, Warning, TEXT("No empty slot found in inventory"));
	return INDEX_NONE;
}

void USInventoryComponent::SwapItems(int32 DraggedIndex, int32 DroppedIndex)
{
	if (DraggedIndex >= 0 && DraggedIndex < InventoryArray.Num() &&
		DroppedIndex >= 0 && DroppedIndex < InventoryArray.Num())
	{
		// Swap the items
		const FSlotData TempItem = InventoryArray[DraggedIndex];
		
		InventoryArray[DraggedIndex] = InventoryArray[DroppedIndex];
		InventoryArray[DroppedIndex] = TempItem;
        
		InventoryArray[DroppedIndex].SlotIndex = DroppedIndex;
		InventoryArray[DraggedIndex].SlotIndex = DraggedIndex;
		
		OnSlotUpdate.Broadcast(DroppedIndex);
		OnSlotUpdate.Broadcast(DraggedIndex);
		
		// If the draggedItem is empty and the destination is not, do nothing
		// This prevents creating items out of thin air		
        
		UE_LOG(LogTemp, Log, TEXT("SwapItems - Dragged Index: %d, Dropped Index: %d"), DraggedIndex, DroppedIndex);
		UE_LOG(LogTemp, Log, TEXT("Dragged Empty: %s, Dropped Empty: %s"), 
			IsIndexEmpty(DraggedIndex) ? TEXT("True") : TEXT("False"), 
			IsIndexEmpty(DroppedIndex) ? TEXT("True") : TEXT("False"));
	}
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
