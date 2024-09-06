// Fill out your copyright notice in the Description page of Project Settings.


#include "SStorageCrate.h"

#include "SStorageComponent.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ASStorageCrate::ASStorageCrate()
{
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = BoxCollision;
	
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(BoxCollision);

	StorageComp = CreateDefaultSubobject<USStorageComponent>(TEXT("StorageComp"));
	
	StorageSize = 32;
}

void ASStorageCrate::Interact_Implementation(APawn* InstigatorPawn)
{
	// TODO: Bir arayuz goster.
	// TODO: StorageComponent ile iletisim kurup, arayuzdeki sekmelere tiklayip gecis yapabilelim.
	// TODO: StorageComp sadece verileri bulunduran ve algoritmayi barindan bir component. Arayuzle ilgili interaction kismini storage create kendi icerisinde halledecek.

	
}

void ASStorageCrate::AddToStorage()
{
	
}

void ASStorageCrate::RemoveFromStorage()
{
}



