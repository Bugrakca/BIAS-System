// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeapon.h"

#include "SInventoryComponent.h"
#include "Components/SphereComponent.h"


// Sets default values
ASWeapon::ASWeapon()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = SphereComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(SphereComp);
}

void ASWeapon::Interact_Implementation(APawn* InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	USInventoryComponent* InventoryComp = Cast<USInventoryComponent>(InstigatorPawn->GetComponentByClass(USInventoryComponent::StaticClass()));
	//USInventoryComponent* InventoryComponent = GetComponentByClass<USInventoryComponent>();
	InventoryComp->Add(ItemData, 999); // This quantity can be changed
	Destroy();
}


