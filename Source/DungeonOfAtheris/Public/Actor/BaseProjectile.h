// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class DUNGEONOFATHERIS_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(visibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
	ABaseProjectile();

protected:

	virtual void BeginPlay() override;
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere);
	TObjectPtr<USphereComponent> Sphere;

	
};