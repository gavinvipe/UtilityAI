// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilityTask.h"
#include "UtilityConsideration.h"
#include "StructLibrary.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"


//calculates score based on score from considerations
float UUtilityTask::CalculateScore(FUtilityContext Context)
{
	if (Considerations.IsEmpty())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("No Consideration Available"));
		return 0.0f;
	}

	float Score = 1.f;

	for (UUtilityConsideration* Consideration : Considerations)
	{
		if (Consideration)
		{
			Score *= Consideration->Evaluate(Context);
		}
	}

	return Score * Weight;
}

void UUtilityTask::Initialize()
{
	APawn* OwnerPawn = GetTypedOuter<APawn>();

	if (OwnerPawn)
	{
		AAIController* AIController = Cast<AAIController>(OwnerPawn->GetController());
	}

}

//Executes the task
void UUtilityTask::ExecuteTask()
{
	

}

