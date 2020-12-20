// Copyright Epic Games, Inc. All Rights Reserved.

#include "TecentCourse_2Character.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "public/TPSProjectile.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
#include "Public/HealthComponent.h"

//////////////////////////////////////////////////////////////////////////
// ATecentCourse_2Character

class ATPSProjectile; 
ATecentCourse_2Character::ATecentCourse_2Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	
	healthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
	healthComp->OnHealthChanged.AddDynamic(this, &ATecentCourse_2Character::OnHealthChanged);
}


//////////////////////////////////////////////////////////////////////////
// Input

void ATecentCourse_2Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	

	PlayerInputComponent->BindAxis("MoveForward", this, &ATecentCourse_2Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATecentCourse_2Character::MoveRight);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATecentCourse_2Character::Fire);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ATecentCourse_2Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ATecentCourse_2Character::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ATecentCourse_2Character::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ATecentCourse_2Character::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ATecentCourse_2Character::OnResetVR);
}


void ATecentCourse_2Character::Fire()
{
	if (GetLocalRole() < ROLE_Authority) {
		ServerFire();
	}
	/*isFire = true;
	//GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Red, TEXT("OK"));
	// 尝试发射物体。
	if (ProjectileClass)
	{
		// 获取摄像机变换。
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);
		//UE_LOG(LogTemp, Log, TEXT("%d,%d"));
		// 将 MuzzleOffset 从摄像机空间变换到世界空间。
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CameraRotation;
		// 将准星稍微上抬。
		MuzzleRotation.Pitch += 10.0f;
		UWorld* World = GetWorld();
		if (World)
		{
			//GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Red, TEXT("123"));
			// 在枪口处生成发射物。
			ATPSProjectile* Projectile = World->SpawnActor<ATPSProjectile>(ProjectileClass, CameraLocation, CameraRotation);
			if (Projectile)
			{
				//GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Red, TEXT("456"));
				// 设置发射物的初始轨道。
				FVector LaunchDirection = MuzzleRotation.Vector();
				//Projectile->FireInDirection(LaunchDirection);
			}
		}
	}*/
	/*FHitResult Hit;
	FVector CameraLocation;
	FRotator CameraRotation;
	GetActorEyesViewPoint(CameraLocation, CameraRotation);
	FVector TraceEnd = CameraLocation + (CameraRotation.Vector() * 10000);
	FCollisionQueryParams QueryParams;
	// 忽略玩家
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = true;
	if (GetWorld()->LineTraceSingleByChannel(Hit, CameraLocation, TraceEnd, ECC_Visibility)) {
		DrawDebugLine(GetWorld(), CameraLocation, TraceEnd, FColor::Red, false, 2, 0, 2);
		AActor* HitActor = Hit.GetActor();
		AActor* myOwner = GetOwner();
		UGameplayStatics::ApplyPointDamage(HitActor, 20, CameraRotation.Vector(), Hit, myOwner->GetInstigatorController(), this, DamageType);
	}

	*/
}

void ATecentCourse_2Character::OnHealthChanged(UHealthComponent* OwnerHealthComp, float Health, float HealthDelta, const UDamageType* OwnerDamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (isDied) {
		return;
	}
	if (Health <= 0 && !isDied) {
		isDied = true;
		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		DetachFromControllerPendingDestroy();
	}
}

FVector ATecentCourse_2Character::GetPawnViewLocation() const
{
	if (FollowCamera) {
		return FollowCamera->GetComponentLocation();
	}
	return Super::GetPawnViewLocation();
}

void ATecentCourse_2Character::ServerFire_Implementation()
{
	Fire();
}

bool ATecentCourse_2Character::ServerFire_Validate()
{
	return true;
}

void ATecentCourse_2Character::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ATecentCourse_2Character::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ATecentCourse_2Character::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ATecentCourse_2Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATecentCourse_2Character::LookUpAtRate(float Rate)
{
	GEngine->AddOnScreenDebugMessage(0, 0.5f, FColor::Yellow, TEXT("123"));
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ATecentCourse_2Character::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ATecentCourse_2Character::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ATecentCourse_2Character::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATecentCourse_2Character,isDied);
}
