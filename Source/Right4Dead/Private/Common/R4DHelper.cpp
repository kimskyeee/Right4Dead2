// Fill out your copyright notice in the Description page of Project Settings.


#include "R4DHelper.h"

bool UR4DHelper::IsChildBone(const USkeletalMeshComponent* SkeletalMeshComp, const FName& BoneName,
	const FName& TargetBoneName)
{
	if (BoneName == TargetBoneName)
	{
		return true; // 현재 Bone이 목표 Bone과 동일한 경우
	}

	// 현재 Bone의 부모 Bone을 가져옴
	const FName ParentBoneName = SkeletalMeshComp->GetParentBone(BoneName);

	// 부모 Bone이 없으면 최상위 Bone이므로 false 반환
	if (ParentBoneName == NAME_None)
	{
		return false;
	}

	// 재귀적으로 부모 Bone을 탐색
	return IsChildBone(SkeletalMeshComp, ParentBoneName, TargetBoneName);
}
