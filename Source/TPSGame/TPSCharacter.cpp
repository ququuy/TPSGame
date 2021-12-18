// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSCharacter.h"

#include "Net/UnrealNetwork.h"

// Sets default values
ATPSCharacter::ATPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CurrentWeaponNum = 1;
	CurrentWeaponType = true;

	isGrenading = false;
	isFiring = false;
	CanShoot = true;

}

void ATPSCharacter::ServerTossGrenade_Implementation()
{
	ClientTossGrenade();	
}

void ATPSCharacter::ClientTossGrenade_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("grenade: %d"), isGrenading));
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
					isGrenading = true;
					//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("toss grenade!!!!!!!")));
					AnimInstance->Montage_Play(GrenadeMontage);
				}
			}
		}
	}
	
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
	// First, Calculate Bullet Transform Locally
	FVector CameraLocation;
	FRotator CameraRotation;
	GetActorEyesViewPoint(CameraLocation, CameraRotation);

	MuzzleOffset.Set(50.0f, 3.0f, -50.0f);

	FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
	FRotator MuzzleRotation = CameraRotation;
	MuzzleRotation.Pitch += 0.0f;

	// Server get the event
	ServerFire(MuzzleLocation, MuzzleRotation);
}

void ATPSCharacter::ServerFire_Implementation(const FVector& MuzzleLocation, const FRotator& MuzzleRotation)
{
	ClientFire(MuzzleLocation, MuzzleRotation);	
}

void ATPSCharacter::ClientFire_Implementation(const FVector& MuzzleLocation, const FRotator& MuzzleRotation)
{
	StartShooting(MuzzleLocation, MuzzleRotation);
}


void ATPSCharacter::StartShooting(const FVector& MuzzleLocation, const FRotator& MuzzleRotation)
{
	if (!CanShoot) return;
	if (BulletClass)
	{

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

            	
				CharacterMesh = GetMesh();
				if (UAnimInstance* AnimInstance = CharacterMesh->GetAnimInstance())
				{
					if (FireMontage)
					{
						isFiring = true;
						//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Fire Montage Playing")));
						AnimInstance->Montage_Play(FireMontage);
					}
				}
            }
		}
	}
}

void ATPSCharacter::StopShooting()
{
	isFiring = false;	
}

void ATPSCharacter::OnFireFinished()
{
	StopShooting();
}



void ATPSCharacter::TossGrenade()
{
	ServerTossGrenade();
	
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

		isGrenading = false;
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("end grenade: %d"), isGrenading));

		
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

