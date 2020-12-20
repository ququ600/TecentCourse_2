// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gun.h"
#include "Ak47.generated.h"

/**
 * 
 */
UCLASS()
class TECENTCOURSE_2_API AAk47 : public AGun
{
	GENERATED_BODY()
public:
	AAk47();
	//UFUNCTION(BlueprintCallable, Category = "Fire")
		void Fire() override;
	UFUNCTION(BlueprintCallable, Category = "SunShine")
		void BluePrintCall();
	float LastTime;


};
