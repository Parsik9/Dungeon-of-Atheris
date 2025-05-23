// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BaseGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONOFATHERIS_API UBaseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,Category="Input")
	FGameplayTag StartupInputTag;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="Damage")
	FScalableFloat Damage;
};
