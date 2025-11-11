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

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
    bool bAttackPlay = false;

    UFUNCTION(BlueprintCallable, Category=FSMEvent)
    void OnEndAttackAnimation();
};
