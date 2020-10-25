// Fill out your copyright notice in the Description page of Project Settings.


#include "Target.h"
#include "Kismet/GameplayStatics.h"
#include "TargetGameMode.h"
// Sets default values
ATarget::ATarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCOmponent"));
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = CollisionComponent;
	MeshComponent->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 2500.0f+((-1.0 + (2 * (rand() / (float)RAND_MAX)))*800);
	ProjectileMovementComponent->MaxSpeed = 5000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 0.3f;

	
}

void ATarget::OnHit(UPrimitiveComponent* HitComponent, AActor* otherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (otherActor->ActorHasTag("Projectile") ){

		Destroy();
		MyGameMode->IncreaseScore();
		GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Yellow, "OnHit---------");
	}
	
}

// Called when the game starts or when spawned
void ATarget::BeginPlay()
{
	Super::BeginPlay();
	MyGameMode = Cast<ATargetGameMode>(UGameplayStatics::GetGameMode(this));
	MeshComponent->OnComponentHit.AddDynamic(this, &ATarget::OnHit);
}

// Called every frame
void ATarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

