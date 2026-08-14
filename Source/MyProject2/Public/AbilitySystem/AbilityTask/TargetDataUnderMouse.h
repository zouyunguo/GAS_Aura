// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FGameplayAbilityTargetDataHandle&, DataHandle);

/**
 * Custom Ability Task: grabs the cursor hit result on the locally controlled
 * client, replicates it to the server as Target Data, and broadcasts it on
 * both ends.
 *
 * Why this exists: the server has no mouse. Under client prediction the client
 * runs the ability immediately, so it must ship its aim point up to the server
 * for the authoritative version of the ability to use.
 */
UCLASS()
class MYPROJECT2_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks",
		meta = (DisplayName = "TargetDataUnderMouse",
			HidePin = "OwningAbility", DefaultToSelf = "OwningAbility",
			BlueprintInternalUseOnly = "true"))
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);

	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature ValidData;

private:
	virtual void Activate() override;

	/** Local client path: read the cursor and send the data up. */
	void SendMouseCursorData();

	/** Server path: fires when the client's target data arrives. */
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);
};

