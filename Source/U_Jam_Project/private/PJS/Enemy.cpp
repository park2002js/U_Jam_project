// Fill out your copyright notice in the Description page of Project Settings.


#include "PJS/Enemy.h"
#include "PJS/EnemyFSM.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 1. 스켈레탈 메시 데이터 로드
    ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny_Simple.SKM_Manny_Simple'"));
    // 1-1. 데이터가 로드 된다면
    if (tempMesh.Succeeded())
    {
        // 1-2. 스켈레탈 메시를 할당함
        GetMesh()->SetSkeletalMesh(tempMesh.Object);
        // 1-3. 할당된 메시를 위치 설정 & 회전 설정
        GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));
    }

    // 2. EnemyFSM 컴포넌트를 Enemy 인스턴스에 추가
    // fsm은 헤더에 선언됨 -> 사실상 헤더를 include하는 모든 cpp 사이에서 쓰이는 전역 변수
    fsm = CreateDefaultSubobject<UEnemyFSM>(TEXT("FSM"));
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

