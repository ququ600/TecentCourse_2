// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gun.h"
#include "SMG.generated.h"

/**
 * 
 */
UCLASS()
class TECENTCOURSE_2_API ASMG : public AGun
{
	GENERATED_BODY()
public:
	ASMG();
		//UFUNCTION(BlueprintCallable, Category = "Fire")
	void Fire() override;
};
