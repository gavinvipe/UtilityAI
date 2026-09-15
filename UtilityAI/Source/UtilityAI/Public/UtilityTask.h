// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UtilityTask.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class UTILITYAI_API UUtilityTask : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility|Task")
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility|Task")
	float Weight = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility|Task")
	TArray<TObjectPtr<class UUtilityConsideration>> Considerations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility|Task")
	bool bCanExecute = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility|Task")
	bool bCanBeInterrupted = true;


	//functions
	UFUNCTION(BlueprintCallable, Category = "Utility|Task")
	float CalculateScore(FUtilityContext Context);

	UFUNCTION()
	void Initialize();

	UFUNCTION(BlueprintCallable, Category = "Utility|Task")
	void ExecuteTask();
	
};
