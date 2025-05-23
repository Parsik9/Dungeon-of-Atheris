

#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "BaseGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Input/AuraInputComponent.h"
#include "Input/AuraInputConfig.h"
#include "Interaction/EnemyInterface.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Character.h"
#include "UI/UserWidget/DamageTextComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
	Spline=CreateDefaultSubobject<USplineComponent>("Spline");
	
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	AutoRun();
}

void AAuraPlayerController::ShowDamageNumber_Implementation(float DamageAmmount, ACharacter* TargetCharacter)
{
	if (IsValid(TargetCharacter)&& DamageTextComponentClass)
	{
		UDamageTextComponent* DamageTextComponent = NewObject<UDamageTextComponent>(TargetCharacter,DamageTextComponentClass);
		DamageTextComponent->RegisterComponent();//createdefaultsubobject gibi ama dinamik olarak oluşturduk ondan
		DamageTextComponent->AttachToComponent(TargetCharacter->GetRootComponent(),FAttachmentTransformRules::KeepRelativeTransform);
		DamageTextComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageTextComponent->SetDamageText(DamageAmmount);
	}
}

void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning) return;

	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline=Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction=Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination=(LocationOnSpline-CachedDestination).Length();
		if (DistanceToDestination<=AutoRunAcceptanceRadius)
		{
			bAutoRunning=false;
		}
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag Tag)
{
	if (Tag.MatchesTagExact(FBaseGameplayTags::Get().InputTag_LMB))
	{
		bTargeting=CurrentActor ? true : false;
		bAutoRunning=false;
	}
	
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag Tag)
{
	if (!Tag.MatchesTagExact(FBaseGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagReleased(Tag);
		}
		return;
	}

	if (GetASC())
	{
		GetASC()->AbilityInputTagReleased(Tag);
	}
	
	if (!bTargeting&&!bShiftKeyDown)
	{
		APawn* ControllerPawn = GetPawn();
		if (UNavigationPath* NavPath=UNavigationSystemV1::FindPathToLocationSynchronously(this,ControllerPawn->GetActorLocation(),CachedDestination))
		{
			Spline->ClearSplinePoints();
			for (const FVector& PointLoc:NavPath->PathPoints)
			{
				Spline->AddSplinePoint(PointLoc,ESplineCoordinateSpace::World);
				DrawDebugSphere(GetWorld(),PointLoc,10,10,FColor::White,false,5.f);
			}
			bAutoRunning=true;
			CachedDestination=NavPath->PathPoints[NavPath->PathPoints.Num()-1];
			
		}
		bTargeting=false;
		FollowTime=0.f;
	}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag Tag)
{
	if (!Tag.MatchesTagExact(FBaseGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(Tag);
		}
		return;
	}
	if (bTargeting||bShiftKeyDown)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(Tag);
		}
	}
	else
	{
		FollowTime+=GetWorld()->GetDeltaSeconds();
		FHitResult HitResult;
		if (GetHitResultUnderCursor(ECC_Visibility,false,HitResult))
		{
			CachedDestination=HitResult.Location;
		}
		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection=(CachedDestination-ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
}

UBaseAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (AbilitySystemComponent==nullptr)
	{
		AbilitySystemComponent = Cast<UBaseAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
	}
	return AbilitySystemComponent;
}


void AAuraPlayerController::CursorTrace()
{

	FHitResult HitResult;
	GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
	LastActor=CurrentActor;
	CurrentActor=HitResult.GetActor();
    /**
	* Line trace from cursor. There are several scenarios:
	* A. LastActor is null && ThisActor is null
	*- Do nothing
	*B. LastActor is null && ThisActor is valid
	*- Highlight ThisActor
	* C. LastActor is valid && ThisActor is null
	*- UnHighlight LastActor
	*D. Both actors are valid, but LastActor != ThisActor
	*- UnHighlight LastActor, and Highlight ThisActor
	*E. Both actors are valid, and are the same actor
	*- Do nothing
	*/
	if (LastActor==nullptr)
	{
		if (CurrentActor!=nullptr)
		{
			CurrentActor->HiglightActor();
		}
		else
		{
			//do nothing
		}
	}
	else
	{
		if (CurrentActor==nullptr)
		{
			LastActor->UnHiglightActor();
		}
		else
		{
			if (CurrentActor!=LastActor)
			{
				LastActor->UnHiglightActor();
				CurrentActor->HiglightActor();
			}
			else
			{
				//do nothing
			}
		}
	}
}



void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);
	
	UEnhancedInputLocalPlayerSubsystem *Subsystem=ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	
	if (Subsystem)
	{Subsystem->AddMappingContext(AuraContext,0);}

	bShowMouseCursor = true;
	DefaultMouseCursor=EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
	
	
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraInputComponent* AuraInputComponent=CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AAuraPlayerController::Move);
	AuraInputComponent->BindAction(ShiftAction,ETriggerEvent::Started,this,&AAuraPlayerController::ShiftKeyPressed);
	AuraInputComponent->BindAction(ShiftAction,ETriggerEvent::Completed,this,&AAuraPlayerController::ShiftKeyReleased);
	AuraInputComponent->BindAbilityActions(InputConfig,this,&ThisClass::AbilityInputTagPressed,&ThisClass::AbilityInputTagReleased,&ThisClass::AbilityInputTagHeld);
    
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector=InputActionValue.Get<FVector2D>();
	const FRotator Rotation=GetControlRotation();
	const FRotator YawRotation(0.f,Rotation.Yaw,0.f);

	const FVector ForwardDirection=FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection=FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn=GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection,InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection,InputAxisVector.X);
	}
	
}

