// Fill out your copyright notice in the Description page of Project Settings.


#include "CannonBall.h"

#include "VehicleEnemy.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

class AVehicleEnemy;
// Sets default values
ACannonBall::ACannonBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(Collision);

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	BulletMesh->SetupAttachment(Collision);
	BulletMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement -> InitialSpeed = 7000.f;
	ProjectileMovement -> ProjectileGravityScale = 0.f;

	Collision -> OnComponentHit.AddDynamic(this, &ACannonBall::OnHit);
}

// Called when the game starts or when spawned
void ACannonBall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACannonBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACannonBall::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	
	
	if(AVehicleEnemy* EnemyVehicle = Cast<AVehicleEnemy>(OtherActor))
	{
		UKismetSystemLibrary::PrintString(this, "Hit Cannon");
		EnemyVehicle -> SetHealth(EnemyVehicle -> GetHealth() - 1);
		if(EnemyVehicle -> GetHealth() <= 0)
		{
			EnemyVehicle -> Destroy();
		}
	}
	Destroy();
}