// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSCharacter.h"

// Sets default values
ATPSCharacter::ATPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CurrentWeaponNum = 1;
	CurrentWeaponType = true;

}

// Called when the game starts or when spawned
void ATPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	check(GEngine != nullptr);
    
    // Display a debug message for five seconds. 
    // The -1 "Key" value argument prevents the message from being updated or refreshed.
    //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using FPSCharacter."));
	
}

// Called every frame
void ATPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATPSCharacter::Fire);
	PlayerInputComponent->BindAction("TossGrenade", IE_Pressed, this, &ATPSCharacter::TossGrenade);
	
	PlayerInputComponent->BindAction("GetWeapon1", IE_Pressed, this, &ATPSCharacter::WeaponReloading);
}


void ATPSCharacter::Fire()
{
	if (BulletClass)
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		MuzzleOffset.Set(100.0f, 5.0f, -2.0f);

		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CameraRotation;
		MuzzleRotation.Pitch += 8.0f;

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.Instigator = GetInstigator();
            
            // Spawn the projectile at the muzzle.
            ABullet* Bullet= World->SpawnActor<ABullet>(BulletClass, MuzzleLocation, MuzzleRotation, SpawnParams);
            if (Bullet)
            {
                // Set the projectile's initial trajectory.
                FVector LaunchDirection = MuzzleRotation.Vector();
                Bullet->FireInDirection(LaunchDirection);
            }
		}
	}
}

void ATPSCharacter::TossGrenade()
{
	if (GrenadeClass)
	{
          	
          	//HitWorldLocation.X, HitWorldLocation.Y, HitWorldLocation.Z));    
		Grenade = GetWorld()->SpawnActor<AGrenade>(GrenadeClass);
		if (Grenade)
		{
			CharacterMesh = GetMesh();
			Grenade->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("GrenadeSocket"));
			if (UAnimInstance* AnimInstance = CharacterMesh->GetAnimInstance())
			{
				if (GrenadeMontage)
				{
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("toss grenade!!!!!!!")));
					AnimInstance->Montage_Play(GrenadeMontage);
				}
			}
		}
	}
	
}

void ATPSCharacter::OnGrenadeReleased()
{
	//ABullet* Grenade = GetWorld()->SpawnActor<ABullet>(GrenadeClass);
	if (Grenade)
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);
		CameraRotation.Pitch += 10;

		FVector ForwardVector = CameraRotation.Vector() * 2500.f;
		Grenade->GrenadeMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		Grenade->GrenadeMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Grenade->GrenadeMesh->SetSimulatePhysics(true);
		Grenade->GrenadeMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);

		
		Grenade->GrenadeMesh->AddImpulse(ForwardVector);

		
	}
	
}


void ATPSCharacter::WeaponReloading()
{
	
	CharacterMesh = GetMesh();
	//Grenade->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("GrenadeSocket"));
	if (UAnimInstance* AnimInstance = CharacterMesh->GetAnimInstance())
	{
		if (ReloadingMontage)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Reloading Montage playing")));
			AnimInstance->Montage_Play(ReloadingMontage);
		}
	}
}

