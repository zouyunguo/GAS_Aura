// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"
class UDamageTextComponent;
class USplineComponent;
class UAuraInputConfig;
struct FGameplayTag;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
class IIEnemyInterface;
class UAuraAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class MYPROJECT2_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AAuraPlayerController();
	virtual void PlayerTick(float DeltaTime) override;
	
	/**
 * 飘字。用 Client RPC 而非 Multicast —— 伤害数字只该给打出这一下的
 * 玩家看，Multicast 会让所有人屏幕上飘满别人的数字。
 */
	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter);

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction;
	bool bshiftPressed = false;
	void shiftPressed(){bshiftPressed = true;}
	void shiftReleased(){bshiftPressed = false;}
	
	void move(const FInputActionValue& Value);

	void cursurTrace();

	TObjectPtr<IIEnemyInterface> lastActor ;

	TObjectPtr<IIEnemyInterface> currentActor;
	
	FHitResult CursorHit;
	
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AbilitySystemComponent;
	
	UAuraAbilitySystemComponent* GetASC();
	
	
	/* Click to Move */
	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	bool bTargeting = false;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	void AutoRun();
};
