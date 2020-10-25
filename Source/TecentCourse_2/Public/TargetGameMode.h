// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TargetGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TECENTCOURSE_2_API ATargetGameMode : public AGameModeBase
{
	GENERATED_BODY()
		ATargetGameMode();
public:
	UPROPERTY(BlueprintReadOnly)
	int score;
	
	void IncreaseScore();
	
};
