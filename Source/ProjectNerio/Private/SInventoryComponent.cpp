#include "SInventoryComponent.h"

#include "SStorageCrateWidget.h"

// Sets default values for this component's properties
USInventoryComponent::USInventoryComponent()
{
	InventoryWidget = nullptr;

	IndexCount = 0;

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

void USInventoryComponent::Add(const FItemData& Item, int32 Quantity)
{
	// Eger array kullarnirsam, belli limitte bir slot sayisi olmasi lazim. Her bir itemin indexini tutmam gerekiyor. Arayuzden indexlerin siralari belli olacak. ( 0 dan baslayip 31 e kadar gidecek)
	// Tikladigim yerin indexini almis olmam lazim. Ve itemi biraktigim yerin indexini de daha sonra itemin verilerine eklemek lazim. Itemi ilk aldigim yer bos kalacagindan oranin icerisini de null yapmam gerekiyor.

	const int32 Index = InventoryArray.IndexOfByPredicate([&Item](const FSlotData& DataElement)
	{
		return DataElement.Item.Name.EqualTo(Item.Name);
	});

	// Check if there is same item
	if (Index != INDEX_NONE && Item.bIsStackable) //&& InventoryArray[Index].Quantity < Item.StackSize
	{
		UE_LOG(LogTemp, Log, TEXT("Item is in the array"));
		InventoryArray[Index].Quantity += Quantity;
		OnSlotUpdate.Broadcast(Index);

		// if (InventoryArray[Index].Quantity >= 999)
		// {
		// 	TODO: For ile ayni itemlari bulmam lazim. Her bir item 999 olana kadar Quantity eklenmesi gerekiyor.
		// 	Eger elimizde bir deger kaldi ve 999dan kucuk, ekleyecek de bir yer kalmadiysa yeni bir item olusturmak gerekiyor.
		// 	int32 NewItemQuantity = InventoryArray[Index].Quantity - Item.StackSize;
		//
		// 	BUG: For bitmeden item eklendigi icin for tekrar calisiyor ve ekstra 1 item daha ekliyor.
		// 	InventoryArray[Index].Quantity = Item.StackSize;
		//
		// 	CreateNewItem(Item, NewItemQuantity);
		// }
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Item is not in the array. Adding now!"));
		const int32 NewIndex = CreateNewItem(Item, Quantity);
		UE_LOG(LogTemp, Warning, TEXT("Sending index: %d"), NewIndex);
		OnSlotUpdate.Broadcast(NewIndex);
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
			// FItemData EmptyItemData{};
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
	const FItemData EmptyItemData{};
	FSlotData EmptyItem{};

	for (int32 Index = 0; Index < Size; ++Index)
	{
		EmptyItem.SlotIndex = Index;
		Array.Add(EmptyItem);
	}
	UE_LOG(LogTemp, Log, TEXT("Init The Inventory Array With Empty Structs"));
}

int32 USInventoryComponent::CreateNewItem(const FItemData& Item, int32 Quantity)
{
	UE_LOG(LogTemp, Log, TEXT("This is the InventoryComp CreateNewItem"));
	const FSlotData NewItem(Item, Quantity, InventoryArray[IndexCount].SlotIndex);

	InventoryArray[IndexCount] = NewItem;
	IndexCount++;
	
	return NewItem.SlotIndex;
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
