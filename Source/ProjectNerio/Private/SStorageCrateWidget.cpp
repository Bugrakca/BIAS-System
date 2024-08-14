// Fill out your copyright notice in the Description page of Project Settings.

//Item uzerine geldiginde geri bildirim verilmesi lazim. Itemlar butonlardan olusabilir? Ve iconlari olur?

#include "SStorageCrateWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

// Begin Play
void USStorageCrateWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// Event tabanli bir sistem daha mantikli olabilir. Bir item uzerine sag klik yapildiginda, itemlarin nereye koyulmasi gerektigini hesaplayacak. Event broadcast edilir. Evente sub olanlar bunu gorur ve itemlari ona gore siralanir ve
	// yerlestirilir. Eger custom bir tab aciksa ve focus onda ise otomatik yerlestirme sistemi deaktive olur.

	TestButton->OnClicked.AddUniqueDynamic(this, &USStorageCrateWidget::OnButtonClicked);
}

void USStorageCrateWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void USStorageCrateWidget::SortItemsWithTag()
{
	TestText->SetText(FText::FromString("OnButtonClicked!!!!"));
}

void USStorageCrateWidget::OnButtonClicked()
{
	SortItemsWithTag();
}
