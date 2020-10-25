// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetSpawner.h"
#include "Target.h"

// Sets default values
ATargetSpawner::ATargetSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void ATargetSpawner::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(timerHandle, this, &ATargetSpawner::Spawn, 1.0f, true);
}

// Called every frame
void ATargetSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATargetSpawner::Spawn()
{
	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = GetActorRotation();
	SpawnLocation.Z += (-1.0 + (2 * (rand() / (float)RAND_MAX)))*length;
	UWorld* world = GetWorld();
	ATarget* target = world->SpawnActor<ATarget>(SpawnClass, SpawnLocation, SpawnRotation);
}

