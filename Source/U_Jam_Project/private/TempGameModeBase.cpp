// Fill out your copyright notice in the Description page of Project Settings.


#include "TempGameModeBase.h"
#include "U_Jam_Project.h"

ATempGameModeBase::ATempGameModeBase()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *CALLINFO);
}
