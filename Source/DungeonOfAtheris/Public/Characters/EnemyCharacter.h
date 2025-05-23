// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Characters/BaseCharacter.h"
#include "Interaction/EnemyInterface.h"
#include  "UI/WidgetController/OverlayWidgetController.h"
#include "EnemyCharacter.generated.h"

class UWidgetComponent;
/**
 * 
 */
UCLASS()
class DUNGEONOFATHERIS_API AEnemyCharacter : public ABaseCharacter,public IEnemyInterface
{
	GENERATED_BODY()

public:

	AEnemyCharacter();

	// begin Enemy Interface
	virtual void HiglightActor() override;
	virtual void UnHiglightActor() override;
	virtual void Die() override;
	// end Enemy Interface

	virtual int32 GetLevel() override;

	void HitReactTagChanged(const FGameplayTag CallbackTag,int32 NewCount);
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangeSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangeSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintReadOnly,Category="Combat")
	bool bHitReacting=false;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Combat")
	float BaseWalkSpeed=250.f;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Combat")
	float LifeSpan=5.f;
	
protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;

	virtual void InitDefaultAttributes() const override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Character Class Defaults")
	int32 Level=1;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Character Class Defaults")
	ECharacterClass CharacterClass=ECharacterClass::Warrior;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
	
};
