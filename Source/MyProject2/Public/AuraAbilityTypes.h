#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

/**
 * 扩展的 GameplayEffectContext。
 *
 * 为什么需要它：ExecCalc 算出"这一下格挡了 / 暴击了"之后，这个信息必须
 * 跟着 GE 一路传到客户端的 UI（飘字要显示不同样式）。EffectContext 是
 * GE 生命周期里唯一能携带自定义数据、且会被网络同步的地方。
 *
 * 要让它真正工作，需要三件事配套：
 *   1. 重写 GetScriptStruct() —— 否则序列化时会当成父类，多出来的字段丢失
 *   2. 重写 Duplicate()       —— GAS 内部会复制 context，不重写会切片
 *   3. 自定义 NetSerialize + TStructOpsTypeTraits —— 告诉引擎用我们的序列化
 * 再加上 UAuraAbilitySystemGlobals 覆盖 AllocGameplayEffectContext，
 * 让 MakeEffectContext() 分配的是这个类型而不是基类。
 */
USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	bool IsBlockedHit() const { return bIsBlockedHit; }
	bool IsCriticalHit() const { return bIsCriticalHit; }

	void SetIsBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }
	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }

	/** 序列化时用哪个 UScriptStruct —— 子类必须重写，否则自定义字段会丢。 */
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FAuraGameplayEffectContext::StaticStruct();
	}

	/** GAS 内部会复制 context（比如 Spec 拷贝时），不重写会发生对象切片。 */
	virtual FAuraGameplayEffectContext* Duplicate() const override
	{
		FAuraGameplayEffectContext* NewContext = new FAuraGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// 深拷贝 HitResult，否则两个 context 共享同一份
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

protected:
	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;
};

/**
 * 告诉引擎这个结构体有自定义的 NetSerialize 和拷贝语义。
 * 没有这段，上面的 NetSerialize 永远不会被调用 —— 这是最容易漏的一步，
 * 而且不报错，只是数据同步不过去。
 */
template<>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};
