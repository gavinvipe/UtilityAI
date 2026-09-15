// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ScheduleDataAsset.generated.h"

/**
 * 
 */
struct FScheduleStruct;

UCLASS(Blueprintable, BlueprintType)
class UTILITYAI_API UScheduleDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Schedule")
	TArray<FScheduleStruct> Schedules;
	
};
