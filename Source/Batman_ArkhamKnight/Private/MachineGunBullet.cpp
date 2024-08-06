// Fill out your copyright notice in the Description page of Project Settings.


#include "MachineGunBullet.h"

#include "VehicleEnemy.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AMachineGunBullet::AMachineGunBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(Collision);

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	BulletMesh->SetupAttachment(Collision);
	BulletMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Collision -> OnComponentBeginOverlap.AddDynamic(this, &AMachineGunBullet::OnBeingOverlap);
}

// Called when the game starts or when spawned
void AMachineGunBullet::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMachineGunBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * Speed * DeltaTime);
}

void AMachineGunBullet::OnBeingOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(AVehicleEnemy* EnemyVehicle = Cast<AVehicleEnemy>(OtherActor))
	{
		UKismetSystemLibrary::PrintString(this, "Hit MachineGun");
		EnemyVehicle -> SetHealth(EnemyVehicle -> GetHealth() - 1);
		if(EnemyVehicle -> GetHealth() <= 0)
		{
			EnemyVehicle -> Destroy();
		}
	}
	
	Destroy();
}