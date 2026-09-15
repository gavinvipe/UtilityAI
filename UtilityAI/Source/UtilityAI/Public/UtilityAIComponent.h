// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StructLibrary.h"
#include "UtilityAIComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = (UtilityAI), meta = (BlueprintSpawnableComponent))
class UTILITYAI_API UUtilityAIComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties 
	UUtilityAIComponent();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Utility|Task")
	class UScheduleDataAsset* Schedule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility|Task")
	TArray<FUtilityTaskStruct> DefaultTasks;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	class UUtilityTask* CurrentDynamicTask;

	UPROPERTY()
	class UUtilityTask* CurrentScheduleTask;

	UPROPERTY()
	TArray<class UUtilityTask*> AvailableTasks;

	UPROPERTY()
	TMap<FName, class UUtilityTask*> ActiveScheduleTasks;

	UPROPERTY()
	TMap<class UUtilityTask*, bool> ActiveDynamicTasks;

	UFUNCTION()
	void InitailizeDefaultTask();

	UFUNCTION()
	void InitailizeScheduleTask();

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Utility AI Component")
	void FindHighestScoreTask(FUtilityContext Context, UUtilityTask*& SelectedTask, float& Score);

	UFUNCTION(BlueprintCallable, Category = "Utility AI Component")
	void AddDynamicTask(TSubclassOf<UUtilityTask> Task, bool bRemoveWhenFinished = true);

	UFUNCTION(BlueprintCallable, Category = "Utility AI Component")
	bool RemoveDynamicTask(TSubclassOf<UUtilityTask>  Task);

	UFUNCTION(BlueprintCallable, Category = "Utility AI Component")
	void AddScheduleTask(FName ScheduleID);

	UFUNCTION(BlueprintCallable, Category = "Utility AI Component")
	bool RemoveScheduleTask(FName ScheduleID);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Utility AI Component")
	void OnTaskCompleted(bool bOnSucceded);
};
