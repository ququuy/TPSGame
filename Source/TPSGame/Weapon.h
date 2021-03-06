// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class TPSGAME_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Collision Component
	UPROPERTY(VisibleDefaultsOnly, Category = "Collision")
	USphereComponent* CollisionComponent;
	
	// Movement Component
	//UPROPERTY(VisibleAnywhere, Category = "Projectile Movement")
	//UProjectileMovementComponent* ProjectileMovementComponent;

	// Projectile mesh
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	//UStaticMeshComponent* MeshComponent;
	USkeletalMeshComponent* MeshComponent;
	// Projectile material 
	UPROPERTY(VisibleDefaultsOnly, Category = "Material")
	UMaterialInstanceDynamic* MaterialInstance;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	int WeaponType;

	//UFUNCTION(BlueprintNativeEvent, Category = "Event Hit")
	//void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION(BlueprintCallable)
	int OnHit(FVector HitWorldLocation);
};
