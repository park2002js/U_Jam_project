// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyFSM.h"
#include "EnemyAnim.generated.h"


/**
 * 
 */
UCLASS()
class U_JAM_PROJECT_API UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=FSM)
    EEnemyState animState;

    // AttackDelay에서 Attack 상태로 변환되는 Transition의 조건으로 사용
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
    bool bAttackPlay = false;

    // Enemy 블루프린트에서 TakeDamage를 보내기 여부를 결정하는 조건으로 사용
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
    bool bAttackDamageEnable = false;  

    UFUNCTION(BlueprintCallable, Category=FSMEvent)
    void OnEndAttackAnimation();

    UFUNCTION(BlueprintCallable, Category = FSMEvent)
    void OnAttackDamageStart();

    UFUNCTION(BlueprintCallable, Category = FSMEvent)
    void OnAttackDamageEnd();

};
