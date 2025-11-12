// Fill out your copyright notice in the Description page of Project Settings.

#include <Kismet/GameplayStatics.h>
#include "PJS/EnemyFSM.h"
#include "PJS/Enemy.h"
#include "PJS/EnemyAnim.h"
#include <Components/CapsuleComponent.h>

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	// 월드에서 Player 가져오기
    

    auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), targetClass); // GetActorOfClass(GetWorld(), TPSPlayer::StaticClass());
    target = Cast<ACharacter>(actor);
    me = Cast<AEnemy>(GetOwner());     // EnemyFSM 코드를 가지고 있는 인스턴스(Enemy)를 가져와 me에 할당
    
    UE_LOG(LogTemp, Display, TEXT("애니메이션 블루프린트 할당 준비 중"))
    anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance()); // AnimIntance 자식인 UEnemyAnim을 가져옴
    if(!anim) { UE_LOG(LogTemp, Display, TEXT("애니메이션 블루프린트 로드 실패")); }
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // 실행창에 상태 메세지 출력하기
    FString logMsg = UEnum::GetValueAsString(mState);
    GEngine->AddOnScreenDebugMessage(0, 1, FColor::Cyan, logMsg);

	// mState에 따라 다른 State()가 매 틱마다 실행되도록 Switch 문 정의
    switch (mState)
    {
    case EEnemyState::Idle:
        IdleState();
        break;
    case EEnemyState::Move:
        MoveState();
        break;
    case EEnemyState::Attack:
        AttackState();
        break;
    case EEnemyState::Damage:
        DamageState();
        break;
    case EEnemyState::Die:
        DieState();
        break;
    }
}

// 상태 처리 함수들의 뼈대 구현
void UEnemyFSM::IdleState()  // 대기 상태
{
    // 시간은 매 상태 호출때마다 갱신, 상태호출은 매 tick마다 이뤄짐, frame 영향 없게 DeltaTime 사용
    currentTime += GetWorld()->DeltaTimeSeconds;

    // 경과 시간이 설정한 DelayTime보다 길어질 경우
    if (currentTime > idleDelayTime)
    {
        // 현재 상태를 이동상태로 전환 -> 다음 Tick부터 Switch 문에서 Move 상태 함수 호출됨
        mState = EEnemyState::Move;
        
        // 애니메이션 동기화
        anim->animState = mState;

        // 경과 시간 초기화
        currentTime = 0;
    }
}

void UEnemyFSM::MoveState() {  // 이동

    // 1. 타겟 목적지(vector 형태)를 저장
    FVector destination = target->GetActorLocation();
    // 2. 현 위치를 기반으로 타겟 방향 계산 후 저장
    FVector dir = destination - me->GetActorLocation();
    // 3. 해당방향으로 "AddMovementInput"을 이용해 이동하도록 설정
    me->AddMovementInput(dir.GetSafeNormal(), moveSpeed);

    // 타겟이 공격 범위 안에 들어오면 상태를 Attack으로 전환
    if (dir.Size() < attackRange) // dir.size = distance
    {
        mState = EEnemyState::Attack;

        // 애니메이션 동기화
        anim->animState = mState;
        // 공격 애니메이션 재생 활성화
        anim->bAttackPlay = true;
        // 공격 상태 전환 시 대기 시간이 바로 끝나도록 처리 = 외부에서 공격 상태 되면 바로 공격
        currentTime = attackDelayTime;
    }
}

void UEnemyFSM::AttackState() {  // 공격 (to Player)
    // 시간은 매 상태 호출때마다 갱신, 상태호출은 매 tick마다 이뤄짐, frame 영향 없게 DeltaTime 사용
    currentTime += GetWorld()->DeltaTimeSeconds;
    
    // 일정 시간 경과시 공격
    if (currentTime > attackDelayTime)
    {
        // 공격 구현 (로그로 대체)
        UE_LOG(LogTemp, Display, TEXT("attack"));

        currentTime = 0;
        anim->bAttackPlay = true;
    }
    
    // 상대 위치에서 현 위치 빼서 거리 구하기
    float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
    // 타깃과의 거리가 일정 거리 이상 멀어지면 상태를 Move로 변경
    if (distance > attackRange)
    {
        mState = EEnemyState::Move;

        // 애니메이션 동기화
        anim->animState = mState;
    }
}
// 어떤 상태에서든 이 함수가 호출되면 피격 혹은 죽음 상태로 변경됨
void UEnemyFSM::OnDamageProcess()
{
    // 함수 호출시 체력 감소
    hp--;

    // 체력이 남아있으면 피격 상태로 전환
    if (hp)
    {
        mState = EEnemyState::Damage;

        
    }
    else // 체력 없으면 죽음 상태로 전환
    {
        mState = EEnemyState::Die;
        // 죽었으므로 Destroy되기 전까지 물리작용 없도록 Collision 비활성화 해야 함
        me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    // 애니메이션 동기화
    anim->animState = mState;
}

void UEnemyFSM::DamageState() // 피격
{
    // 시간은 매 상태 호출때마다 갱신, 상태호출은 매 tick마다 이뤄짐, frame 영향 없게 DeltaTime 사용
    currentTime += GetWorld()->DeltaTimeSeconds;
    // 일정 시간 경과시 Move 상태로 변경
    if (currentTime > damageDelayTime)
    {
        mState = EEnemyState::Move;
        // 애니메이션 동기화
        anim->animState = mState;
        currentTime = 0;
    }
}
void UEnemyFSM::DieState()   // 죽음
{
    // Die 상태가 되면 바닥 아래로 내려감
    FVector p0 = me->GetActorLocation();
    FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
    FVector p = p0 + vt;
    me->SetActorLocation(p);

    // 일정 거리 이하로 내려가면 제거
    if (p.Z < -200.0f)
    {
        me->Destroy();
    }
}

bool UEnemyFSM::CanApplyDamage()
{
    return anim->bAttackDamageEnable;
}