// Fill out your copyright notice in the Description page of Project Settings.


#include "SMG.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"
ASMG::ASMG(){
	ShootSpeed = 0.1f;
}
void ASMG::Fire()
{
	if (GetLocalRole() < ROLE_Authority) {
		ServerFire();
	}
	AActor* MyOwner = GetOwner();
	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Red, TEXT("123"));
	if (MyOwner) {
		FVector CameraLocation;
		FRotator CameraRotation;
		MyOwner->GetActorEyesViewPoint(CameraLocation, CameraRotation);
		FHitResult Hit;
		FVector TraceEnd = CameraLocation + (CameraRotation.Vector() * 10000);
		FCollisionQueryParams QueryParams;
		// ºöÂÔÍæ¼Ò
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(MyOwner);
		QueryParams.bTraceComplex = true;
		if (GetWorld()->LineTraceSingleByChannel(Hit, CameraLocation, TraceEnd, ECC_Visibility, QueryParams)) {
			DrawDebugLine(GetWorld(), CameraLocation, TraceEnd, FColor::Red, false, 2, 0, 2);
			AActor* HitActor = Hit.GetActor();
			UGameplayStatics::ApplyPointDamage(HitActor, 20, CameraRotation.Vector(), Hit, MyOwner->GetInstigatorController(), this, DamageType);
		}
	}
}
