// Fill out your copyright notice in the Description page of Project Settings.


#include "R4DHelper.h"

/// Bone이 속한 부위를 대표하는 Bone의 이름을 반환하는 함수
/// @param SkeletalMeshComp Bone이 포함된 Skeletal Mesh Component
/// @param BoneName 
/// @return Bone이 속한 부위를 대표하는 Bone의 이름
FName UR4DHelper::GetParentBone(const USkeletalMeshComponent* SkeletalMeshComp, const FName& BoneName)
{
	if (BoneName.IsNone())
	{
		return BoneName;
	}
	
	if (IsChildBone(SkeletalMeshComp, BoneName, TEXT("neck_01")))
	{
		return TEXT("neck_01");
	}
	if (IsChildBone(SkeletalMeshComp, BoneName, TEXT("lowerarm_l")))
	{
		return TEXT("lowerarm_l");	
	}
	if (IsChildBone(SkeletalMeshComp, BoneName, TEXT("lowerarm_r")))
	{
		return TEXT("lowerarm_r");
	}
	if (IsChildBone(SkeletalMeshComp, BoneName, TEXT("thigh_l")))
	{
		return TEXT("thigh_l");	
	}
	if (IsChildBone(SkeletalMeshComp, BoneName, TEXT("thigh_r")))
	{
		return TEXT("thigh_r");
	}
	if (IsChildBone(SkeletalMeshComp, BoneName, TEXT("spine_02")))
	{
		return TEXT("spine_02");
	}
	return FName(NAME_None);
}

/// Bone이 특정 Bone의 자식인지 여부를 반환하는 함수
/// @param SkeletalMeshComp Bone이 포함된 Skeletal Mesh Component
/// @param BoneName 특정 Bone의 자식인지 여부를 알고싶은 Bone의 이름
/// @param TargetBoneName 부모인지 확인하고 싶은 Bone의 이름
/// @return A는 B의 자식 Bone인가?
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
