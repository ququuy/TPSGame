// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Bullet.h"
#include "CoreMinimal.h"
#include "Grenade.h"
#include "GameFramework/Character.h"
#include "TPSCharacter.generated.h"

UCLASS()
class TPSGAME_API ATPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSCharacter();

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	/**** Shooting ****/
	// Projectile class to spawn.
    UPROPERTY(EditAnywhere, Category = "Shoot Projectile")
    TSubclassOf<class ABullet> BulletClass;


	UPROPERTY(BlueprintReadWrite, Category = "Shoot Projectile")
	USkeletalMeshComponent* CharacterMesh;

	/*** Grenade ***/
    UPROPERTY(EditAnywhere, Category = "Shoot Projectile")
    TSubclassOf<class AGrenade> GrenadeClass;
	
    UPROPERTY(EditAnywhere, Category = "Animation")
    class UAnimMontage* GrenadeMontage;
	
    UPROPERTY(EditAnywhere, Category = "Animation")
    class UAnimMontage* FireMontage;

	/*** Reloading ***/	
    UPROPERTY(EditAnywhere, Category = "Animation")
    class UAnimMontage* ReloadingMontage;
public:	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Gameplay")
	bool CanShoot;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	/**** Shooting ****/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool isFiring;
	
	UFUNCTION(BlueprintCallable)
	void Fire();
	
	UFUNCTION( Server, Reliable )
	void ServerFire(const FVector& MuzzleLocation, const FRotator& MuzzleRotation);
	UFUNCTION( NetMulticast, Reliable )
	void ClientFire(const FVector& MuzzleLocation, const FRotator& MuzzleRotation);
	//void ClientFire();

	
	UFUNCTION(BlueprintCallable)
	void StartShooting(const FVector& MuzzleLocation, const FRotator& MuzzleRotation);
	UFUNCTION(BlueprintCallable)
	void StopShooting();
	
	UFUNCTION(BlueprintCallable)
	void OnFireFinished();

	// Gun muzzle offset from the camera location.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
    FVector MuzzleOffset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool CurrentWeaponType;
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int CurrentWeaponNum;

	/**** Grenade ***/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool isGrenading;

	UFUNCTION( Server, Reliable )
	void ServerTossGrenade();
	UFUNCTION( NetMulticast, Reliable )
	void ClientTossGrenade();

	//void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	AGrenade* Grenade;
	
	UFUNCTION(BlueprintCallable)
	void TossGrenade();
	
	UFUNCTION(BlueprintCallable)
	void OnGrenadeReleased();

	/*** Change weapon***/
	UFUNCTION(BlueprintCallable)
	void WeaponReloading();
};
