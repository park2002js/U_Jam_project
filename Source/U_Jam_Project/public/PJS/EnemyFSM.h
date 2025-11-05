// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
    Idle,
    Move,
    Attack,
    Damage,
    Die,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class U_JAM_PROJECT_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

// FSM의 enum 상태들 선언
public:
    // 상태 변수 "mState" 생성 -> 첫 초기화 상태는 Idle 상태
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=FSM)
    EEnemyState mState = EEnemyState::Idle;
    // 대기
    void IdleState();
    // 이동
    void MoveState();
    // 공격
    void AttackState();
    // 피격
    void DamageState();
    // 죽음
    void DieState();

    // 대기 시간
    // idelDelayTime 변수가 에디터에 노출되도록 EditDefaultsOnly 속성을 변수에 추가함
    UPROPERTY(EditDefaultsOnly, Category=FSM)
    float idleDelayTime = 2.0f;
    // 경과 시간
    float currentTime = 0;

    // 타겟 
    // 타겟 Actor를 에디터에서 설정할 수 있도록 Edit 속성 추가
    // 레벨에 배치되어 있지 않은 것을 찾아야 하므로 최상위 Class인 UObject로 설정
    UPROPERTY(EditDefaultsOnly, Category = FSM)
    TSubclassOf<ACharacter> targetClass; // class ATPSPlayer* target;
    class ACharacter* target;
    UPROPERTY(EditAnywhere, Category = FSM)
    float moveSpeed = 0.2;

    // 소유 엑터
    /* Enemy 본인의 위치값은, FSM 컴포넌트에서가 아닌 컴포넌트가 소유하고 있는 AEnemy 액터이므로,
    이를 위해 AEnemyFSM의 소유 액터 AEnemy를 기억할 속성을 추가함 */
    UPROPERTY()
    class AEnemy* me;

    // 공격 범위
    UPROPERTY(EditAnywhere, Category=FSM)
    float attackRange = 150.0f;

    // 공격 대기 시간
    UPROPERTY(EditAnywhere, Category=FSM)
    float attackDelayTime = 2.0f;

    // Player 공격에 피격시 호출되는 이벤트 함수
    UFUNCTION(BlueprintCallable, Category = FSM)
    void OnDamageProcess();

    // Hp 체력, 블루프린트에서 노드로 사용가능하도록 BlueprintReadWrite 속성 부여함
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=FSM)
    int32 hp = 3;

    // 피격시 적이 얼타는 시간, 그러나 보스전에서는 크게 의미 없을 듯?
    UPROPERTY(EditDefaultsOnly, Category=FSM)
    float damageDelayTime = 1.0f;

    // 죽었을 때 아래로 사라지는 속도 -> 일정 좌표 이상 낮아지면 Destroy할 예정
    UPROPERTY(EditAnywhere, Category=FSM)
    float dieSpeed = 50.0f;
};
