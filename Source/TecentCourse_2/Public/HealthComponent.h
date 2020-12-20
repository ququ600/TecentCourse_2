// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignature, UHealthComponent*, HealthComp, float, Health, float, HealthDelta, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);

UCLASS( ClassGroup=(Health), meta=(BlueprintSpawnableComponent) )
class TECENTCOURSE_2_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();
	UPROPERTY(BlueprintReadOnly, Replicated,Category = "HealthComponent")
		float health;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HealthComponent")
		float defaulthealth;
	UFUNCTION()
	void HandleOnTakeAnyDamage( AActor* DamagedActor, float Damage,const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintAssignable,Category="Events")
	FOnHealthChangedSignature OnHealthChanged;
};
