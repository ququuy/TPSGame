// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ShootProjectileSceneComponent"));
	}
	if (!CollisionComponent)
	{
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		
		CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("ShootHit")); // projectile respond connected to Engine-Collision
		//CollisionComponent->OnComponentHit.AddDynamic(this, &AShootProjectile::OnHit); // Interact with world, Event called when hits
		//CollisionComponent->OnComponentHit.AddDynamic(this, &AShootProjectile::OnHit_Implementation); // Interact with world, Event called when hits
		
		CollisionComponent->InitSphereRadius((15.0f));
		RootComponent = CollisionComponent;
	}
	
	if (!ProjectileMovementComponent)
	{
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		ProjectileMovementComponent->InitialSpeed = 3000.0f;
		ProjectileMovementComponent->MaxSpeed = 3000.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = true;
		ProjectileMovementComponent->Bounciness = 0.3f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}

	if (!ProjectileMeshComponent)
	{
		ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("'/Game/ThirdPersonActor/Bullet/Sphere.Sphere'"));
        if(Mesh.Succeeded())
        {
            ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
        }
		//static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("'/Game/FPProjectile/ProjectileMaterial.ProjectileMaterial'"));
		static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("'/Game/ThirdPersonActor/Bullet/BulletMaterial.BulletMaterial'"));
    	if (Material.Succeeded())
    	{
			ProjectileMaterialInstance = UMaterialInstanceDynamic::Create(Material.Object, ProjectileMeshComponent);
    		//ProjectileMaterialInstance->SetVectorParameterValue(FName("ColorProjectile"), FLinearColor(1.0, 1.0, 1.0, 1.0));
    	}
    	ProjectileMeshComponent->SetMaterial(0, ProjectileMaterialInstance);
    	ProjectileMeshComponent->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));
    	ProjectileMeshComponent->SetupAttachment(RootComponent);
	}

	InitialLifeSpan = 3.0f; // delete after 3s
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Function that initializes the projectile's velocity in the shoot direction.
void ABullet::FireInDirection(const FVector& ShootDirection)
{
    ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void ABullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != this && OtherComponent->IsSimulatingPhysics())
	{
	    OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
		//APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
		//PlayerCharacter->AddScore(2);
	}

   //Destroy();
}	
