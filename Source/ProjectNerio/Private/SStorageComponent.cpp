// Fill out your copyright notice in the Description page of Project Settings.


#include "SStorageComponent.h"

#include "SItemDataStructs.h"

// Bu componentin amaci, sandik sistemini(Algoritmasini) bulunduruyor olmasi. Interaction ile bir alakasi yok.
// TODO: Sandik icerisinde kategoriler olacak. Diablo 4 deki gibi sekmeler. (Weapons, Craft Items, etc.)
// TODO: Her bir itemda "tag" olacak. Sandiga koyulan itemlar bu taglere gore sekmelere otomatik yerlestirilecek.
// TODO: Her yerden erisilebilir olacak. Tek bir sandik olacak. (Evdeki sandiga disaridan erisebilme olanagi saglanmali)
// TODO: Custom bir kategori olusturulabilir. Bu kategorilere gore itemlar yerlestirilebilir. ( Eger custom bir kategori focus ise, itemlari otomatik sekmelere yerlestirme kapanacak.)
// TODO: StorageCrate icersinde her bir stack icin bir limit olacak. Bu limit max 999 olacak. Bu limit asildiktan sonra yeni bir stack olusturulacak ve kalan itemlar burada stacklanmeye devam edecek.
// TODO: Item tamamen sandiktan alindiginda bulundugu slot bosalacak. Bos kalan slotu siralama algoritmasi kullanarak doldurabiliriz fakat bunu kullanici istediginde yaparsa daha iyi olabilir.

// Sets default values for this component's properties
USStorageComponent::USStorageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USStorageComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USStorageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

