// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/GameplayAbility/AuraProjectileSpell.h"

#include "Actor/AuraProjectile.h"
#include "Interface/CombatInterface.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Projectiles are replicated actors; only spawn on the server.
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	if (!ProjectileClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: ProjectileClass is not set! Assign it in the ability Blueprint."), *GetName());
		return;
	}

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		// TODO (Section 11): set rotation toward the mouse target using Target Data.

		if (AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
		{
			// TODO (Section 11): give the projectile a GameplayEffectSpecHandle for damage.

			Projectile->FinishSpawning(SpawnTransform);
		}
	}
}
