#include "SInventoryComponent.h"

#include "SStorageCrateWidget.h"

// Sets default values for this component's properties
USInventoryComponent::USInventoryComponent()
{
}

void USInventoryComponent::Add(const FItemData& Item, int32 Quantity)
{
	//Check if the item quantity is over 999 then create new stack.
	if (FSlotData* ItemData = InventoryData.Find(Item.Name))
	{
		UE_LOG(LogTemp, Log, TEXT("Item is in the map"));
		ItemData->Quantity += Quantity;
		InventoryData.Add(Item.Name, *ItemData);
		// Array kullanirsam, arayuzde slotlari ona gore ayarlamak lazim. Ilk slot 0 ile baslamali, sonra 1 vs.
		// Tikladigim slotun indexini alabiliyor olmam lazim. Yoksa array ile yapmak biraz performanstan odun vermeyi gerektirecek.
	}
	else
	{
		// ItemData is null. Create new item and Add to the list.
		UE_LOG(LogTemp, Log, TEXT("Item is not in the map. Adding now!"));
		const FSlotData NewItem{Item, Quantity};
		InventoryData.Add(Item.Name, NewItem);
		if (Item.ItemCategory == "Weapon")
		{
			// Eger array kullarnirsam, belli limitte bir slot sayisi olmasi lazim. Her bir itemin indexini tutmam gerekiyor. Arayuzden indexlerin siralari belli olacak. ( 0 dan baslayip 31 e kadar gidecek)
			// Tikladigim yerin indexini almis olmam lazim. Ve itemi biraktigim yerin indexini de daha sonra itemin verilerine eklemek lazim. Itemi ilk aldigim yer bos kalacagindan oranin icerisini de null yapmam gerekiyor.
			WeaponData.Add(NewItem);
		}
	}
}

void USInventoryComponent::DropItem(const FItemData& Item, int32 Quantity)
{
	FSlotData* ItemData = InventoryData.Find(Item.Name);

	if (ItemData != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("ItemData is not null"));
		ItemData->Quantity -= Quantity;
		if (ItemData->Quantity <= 0)
		{
			InventoryData.Remove(Item.Name);
		}
		else
		{
			InventoryData.Add(Item.Name, *ItemData);
		}
	}
}

void USInventoryComponent::ShowInventory()
{
	AActor* MyOwner = GetOwner();
	APawn* MyPawn = Cast<APawn>(MyOwner);

	AController* MyController = MyPawn->GetController();
	if (MyController->IsLocalController())
	{
		InventoryWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
		
		if (InventoryWidget)
		{
			InventoryWidget->AddToViewport();
		}
	}
	
}

void USInventoryComponent::HideInventory()
{
	if (IsValid(InventoryWidget))
	{
		InventoryWidget->RemoveFromParent();
	}
}
