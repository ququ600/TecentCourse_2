// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	defaulthealth = 100;
	//SetIsReplicated(true);
	// ...
}


void UHealthComponent::HandleOnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0) {
		return;
	}
	health = FMath::Clamp(health - Damage, 0.0f, defaulthealth);
	UE_LOG(LogTemp,Log,TEXT("Health Changer %s"),*FString::SanitizeFloat(health));

	OnHealthChanged.Broadcast(this,health,Damage,  DamageType,
									InstigatedBy, DamageCauser);
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	if (GetOwnerRole() == ROLE_Authority) {
		health = defaulthealth;

		AActor* MyOwner = GetOwner();
		if (MyOwner) {
			MyOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleOnTakeAnyDamage);
		}
	}
	// ...
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UHealthComponent, health);

}
