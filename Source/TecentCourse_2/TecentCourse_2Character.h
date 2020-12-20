// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "TecentCourse_2Character.generated.h"

class UDamageType;
UCLASS(config=Game)
class ATecentCourse_2Character : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	ATecentCourse_2Character();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
		class UHealthComponent* healthComp;
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)
		TSubclassOf<UDamageType> DamageType;
	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "IsFire")
		bool isFire;


protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	UFUNCTION(BlueprintCallable,Category = "Fire")
	void Fire();
	UFUNCTION(Server,Reliable,WithValidation)
	void ServerFire();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	FVector MuzzleOffset;

	UFUNCTION(BlueprintCallable, Category = "Fire")
	void OnHealthChanged(UHealthComponent* OwnerHealthComp, float Health, float HealthDelta, const class UDamageType* OwnerDamageType, class AController* InstigatedBy, AActor* DamageCauser);
	UPROPERTY(BlueprintReadOnly,Replicated, Category = "Player");
	bool isDied;
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class ATPSProjectile> ProjectileClass;
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	virtual FVector GetPawnViewLocation() const override;
};

