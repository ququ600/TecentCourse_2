// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Gun.generated.h"

UCLASS()
class TECENTCOURSE_2_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();
	// 枪械骨骼组件
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Weapon")
	USkeletalMeshComponent* MeshComponent;
	// 碰撞组件
	UPROPERTY(VisibleAnywhere, Category = "Target")
		USphereComponent* CollisionComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DamageType")
		TSubclassOf<UDamageType> DamageType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DamageType")
		float ShootSpeed;
	UFUNCTION(BlueprintCallable, Category = "Fire")
		virtual void Fire();
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerFire();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



};
