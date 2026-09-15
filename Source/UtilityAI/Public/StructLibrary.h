#pragma once

#include "CoreMinimal.h"
#include "StructLibrary.generated.h"


USTRUCT(BlueprintType)
struct FUtilityContext
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility|Context")
	class AAIController* AIController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility|Context")
	class AActor* Owner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility|Context")
	class AActor* Target;

};

USTRUCT(BlueprintType)
struct FUtilityConsiderationStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility|Consideration")
	class UUtilityConsideration* Consideration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility|Consideration")
	float Weight = 1.f;

};

USTRUCT(BlueprintType)
struct FUtilityTaskStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility|Task")
	TSubclassOf<class UUtilityTask> Task;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility|Task")
	float Weight = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility|Task")
	bool bCanExecute = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility|Task")
	bool bCanBeInterrupted = true;

};

USTRUCT(BlueprintType)
struct FScheduleStruct
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Schedule")
	FName ID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Schedule")
	float StartTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Schedule")
	float EndTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Schedule")
	FUtilityTaskStruct ScheduleTask;

};