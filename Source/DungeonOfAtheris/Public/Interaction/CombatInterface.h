
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"


class UNiagaraSystem;
class UAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnASCRegistered, UAbilitySystemComponent*)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathSignature, AActor*, DeadActor);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDamageSignature, float );

USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UAnimMontage* Montage;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag MontageTag;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag SocketTag;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	USoundBase* ImpactSound;
	
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI,Blueprintable)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DUNGEONOFATHERIS_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    int32 GetLevel();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetSocketLocation(const FGameplayTag& MontageTag);

	virtual void Die(const FVector& DeathImpulse)=0;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateFacingTarget(const FVector& Target);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    UAnimMontage* GetHitReactMontage();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDead() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetAvatar();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<FTaggedMontage> GetAttackMontages();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UNiagaraSystem* GetBloodEffect();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FTaggedMontage GetTaggedMontageByTag(const FGameplayTag& MontageTag);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetMinionCount();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetMinionCount(int32 Amount);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ECharacterClass GetCharacterClass();

	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() = 0;
	virtual FOnDeathSignature& GetOnDeathDelegate() = 0;
	virtual FOnDamageSignature& GetOnDamageSignature() = 0; 

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetInShockLoop(bool bInLoop);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	USkeletalMeshComponent* GetWeapon();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsBeingShocked() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetIsBeingShocked(bool bInShock);

	

};
