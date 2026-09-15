// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilityAIComponent.h"
#include "UtilityTask.h"
#include "ScheduleDataAsset.h"

// Sets default values for this component's properties
UUtilityAIComponent::UUtilityAIComponent()
{

	PrimaryComponentTick.bCanEverTick = false;
}


void UUtilityAIComponent::BeginPlay()
{
	Super::BeginPlay();

	InitailizeDefaultTask();
	InitailizeScheduleTask();

}

//Initializes the defualt tasks 
void UUtilityAIComponent::InitailizeDefaultTask()
{
	if (DefaultTasks.IsEmpty()) return;

	UUtilityTask* Task;
	TSubclassOf<UUtilityTask> TaskClass;

	for (FUtilityTaskStruct TaskStruct : DefaultTasks)
	{
		TaskClass = TaskStruct.Task;
		Task = NewObject<UUtilityTask>(GetOwner(), TaskClass);
		Task->Initialize();

		Task->bCanBeInterrupted = TaskStruct.bCanBeInterrupted;
		Task->bCanExecute = TaskStruct.bCanExecute;
		Task->Weight = TaskStruct.Weight;
		AvailableTasks.Add(Task);
	}
}

//Initializes the Schedule tasks by setting values
void UUtilityAIComponent::InitailizeScheduleTask()
{
	if (!Schedule) return;

	UUtilityTask* Task;
	TSubclassOf<UUtilityTask> TaskClass;

	for (FScheduleStruct ScheduleStruct : Schedule->Schedules)
	{
		TaskClass = ScheduleStruct.ScheduleTask.Task;
		Task = NewObject<UUtilityTask>(GetOwner(), TaskClass);
		Task->Initialize();

		Task->bCanBeInterrupted = ScheduleStruct.ScheduleTask.bCanBeInterrupted;
		Task->bCanExecute = ScheduleStruct.ScheduleTask.bCanExecute;
		Task->Weight = ScheduleStruct.ScheduleTask.Weight;
		ActiveScheduleTasks.Add(ScheduleStruct.ID, Task);
	}
}

// Called every frame
void UUtilityAIComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

//find the task with the highest score
void UUtilityAIComponent::FindHighestScoreTask(FUtilityContext Context, UUtilityTask*& SelectedTask, float& Score)
{
	if (AvailableTasks.IsEmpty())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("No Task Available"));
		return;
	}

	Score = 0.f;
	SelectedTask = nullptr;

	float MaxScore = 0.f;

	//finds the highest score
	for (UUtilityTask* Task : AvailableTasks)
	{
		if (Task && Task->bCanExecute)
		{
			float TaskScore = Task->CalculateScore(Context);

			if (TaskScore > MaxScore)
			{
				MaxScore = TaskScore;
				SelectedTask = Task;
			}
		}
	}

	Score = MaxScore;
}

//Allows task to be added at runtime
void UUtilityAIComponent::AddDynamicTask(TSubclassOf<UUtilityTask> Task, bool bRemoveWhenFinished)
{

	for (TPair<UUtilityTask*, bool> DynamicTask : ActiveDynamicTasks)
	{
		if (DynamicTask.Key && DynamicTask.Key->GetClass() == Task)
		{
			AvailableTasks.Add(DynamicTask.Key);
			return;
		}
	}

	CurrentDynamicTask = NewObject<UUtilityTask>(GetOwner(), Task);
	CurrentDynamicTask->Initialize();
	ActiveDynamicTasks.Add(CurrentDynamicTask, bRemoveWhenFinished);
	AvailableTasks.Add(CurrentDynamicTask);

}

//removes dynamic task 
bool UUtilityAIComponent::RemoveDynamicTask(TSubclassOf<UUtilityTask> Task)
{
	if (!Task) return false;

	//searches for the task and removes it 
	for (TPair<UUtilityTask*, bool>& DynamicTask : ActiveDynamicTasks)
	{
		if (DynamicTask.Key && DynamicTask.Key->GetClass() == Task)
		{
			AvailableTasks.Remove(DynamicTask.Key);
			ActiveDynamicTasks.FindAndRemoveChecked(DynamicTask.Key);

			//destroy's the task
			DynamicTask.Key->ConditionalBeginDestroy();
			return true;

		}
	}

	return false;
}

//Adds a new scehdule
void UUtilityAIComponent::AddScheduleTask(FName ScheduleID)
{
	UUtilityTask** Task = ActiveScheduleTasks.Find(ScheduleID);
	CurrentScheduleTask = *Task;
	AvailableTasks.Add(CurrentScheduleTask);
}

//removes a scehdule task
bool UUtilityAIComponent::RemoveScheduleTask(FName ScheduleID)
{
	UUtilityTask** Task = ActiveScheduleTasks.Find(ScheduleID);

	if (AvailableTasks.Remove(*Task)) return true;
	return false;
}

//function to call when task is completed
void UUtilityAIComponent::OnTaskCompleted_Implementation(bool bOnSucceded)
{
	//removes any current dynamic task with "bRemoveWhenFinished"  enabled when task is completed
	if (CurrentDynamicTask)
	{
		bool* RemoveWhenFinished = ActiveDynamicTasks.Find(CurrentDynamicTask);

		if (*RemoveWhenFinished)
		{
			AvailableTasks.Remove(CurrentDynamicTask);
			ActiveDynamicTasks.FindAndRemoveChecked(CurrentDynamicTask);
			CurrentDynamicTask->ConditionalBeginDestroy();
		}
	}

}

