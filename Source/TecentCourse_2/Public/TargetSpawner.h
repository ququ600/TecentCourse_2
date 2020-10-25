// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetSpawner.generated.h"

UCLASS()
class TECENTCOURSE_2_API ATargetSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATargetSpawner();
	FTimerHandle timerHandle;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnAreaLength")
		float length = 100.f;
	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	TSubclassOf<class ATarget> SpawnClass;


	void Spawn();
};
