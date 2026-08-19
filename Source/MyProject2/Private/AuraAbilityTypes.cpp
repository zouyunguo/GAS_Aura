#include "AuraAbilityTypes.h"

/**
 * 位打包序列化。
 *
 * 思路：先用一个 RepBits 位掩码记录"哪些字段有值"，只序列化有值的那些。
 * 一次攻击可能每秒发生几十次，context 里大部分字段（Actors 数组、
 * WorldOrigin 等）通常是空的，逐个无条件序列化会浪费大量带宽。
 *
 */
bool FAuraGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	//repbits是一个记录true/false的掩码，每一个bit的01都代表是否需要序列化/反序列化某一个东西
	//服务端：根据每个bool的情况写入repbits, serializebits函数将repbits放入网络包，然后继续序列化需要序列化的数据
	//客户端，跳过第一个if，从serializebits函数读取repbits，然后根据repbits获取需要反序列化的数据
	uint32 RepBits = 0;
	if (Ar.IsSaving())
	{
		if (bReplicateInstigator && Instigator.IsValid())
		{
			RepBits |= 1 << 0;
		}
		if (bReplicateEffectCauser && EffectCauser.IsValid())
		{
			RepBits |= 1 << 1;
		}
		if (AbilityCDO.IsValid())
		{
			RepBits |= 1 << 2;
		}
		if (bReplicateSourceObject && SourceObject.IsValid())
		{
			RepBits |= 1 << 3;
		}
		if (Actors.Num() > 0)
		{
			RepBits |= 1 << 4;
		}
		if (HitResult.IsValid())
		{
			RepBits |= 1 << 5;
		}
		if (bHasWorldOrigin)
		{
			RepBits |= 1 << 6;
		}
		if (bIsBlockedHit)
		{
			RepBits |= 1 << 7;
		}
		if (bIsCriticalHit)
		{
			RepBits |= 1 << 8;
		}
	}

	Ar.SerializeBits(&RepBits, 9);   // ← 父类是 7，新增两个 bool 后必须是 9

	if (RepBits & (1 << 0))
	{
		Ar << Instigator;
	}
	if (RepBits & (1 << 1))
	{
		Ar << EffectCauser;
	}
	if (RepBits & (1 << 2))
	{
		Ar << AbilityCDO;
	}
	if (RepBits & (1 << 3))
	{
		Ar << SourceObject;
	}
	if (RepBits & (1 << 4))
	{
		SafeNetSerializeTArray_Default<31>(Ar, Actors);
	}
	if (RepBits & (1 << 5))
	{
		if (Ar.IsLoading())
		{
			if (!HitResult.IsValid())
			{
				HitResult = TSharedPtr<FHitResult>(new FHitResult());
			}
		}
		HitResult->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 6))
	{
		Ar << WorldOrigin;
		bHasWorldOrigin = true;
	}
	else
	{
		bHasWorldOrigin = false;
	}

	// 这两个 bool 不需要 Ar << ——位掩码本身就承载了它们的值，
	// 存在即为 true。读取时直接从 RepBits 还原。
	if (Ar.IsLoading())
	{
		bIsBlockedHit = (RepBits & (1 << 7)) != 0;
		bIsCriticalHit = (RepBits & (1 << 8)) != 0;
	}

	bOutSuccess = true;
	return true;
}
