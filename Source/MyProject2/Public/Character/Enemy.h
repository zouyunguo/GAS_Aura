// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "Interface/IEnemyInterface.h"
#include "Enemy.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT2_API AEnemy : public ACharacterBase , public IIEnemyInterface
{
	GENERATED_BODY()

public:
	AEnemy();
	
	virtual void Highlight() override;
	
	virtual void UnHighlight() override;
	
	/** combat interface **/
	virtual int32 GetPlayerLevel() override;
	
	virtual void BeginPlay() override;
protected:
virtual void InitAbilityActorInfo() override;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Character Class Defaults")
	int32 Level=1;
};
