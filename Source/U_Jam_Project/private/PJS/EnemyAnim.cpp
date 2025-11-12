// Fill out your copyright notice in the Description page of Project Settings.


#include "PJS/EnemyAnim.h"

void UEnemyAnim::OnEndAttackAnimation()
{
    bAttackPlay = false;
}

void UEnemyAnim::OnAttackDamageStart()
{
    bAttackDamageEnable = true;
}

void UEnemyAnim::OnAttackDamageEnd()
{
    bAttackDamageEnable = false;
}