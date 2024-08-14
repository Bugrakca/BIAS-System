// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SInputConfigData.generated.h"

class UInputAction;
/**
 * 
 */
UCLASS()
class PROJECTNERIO_API USInputConfigData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_Move;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_Look;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_Interaction;
};
