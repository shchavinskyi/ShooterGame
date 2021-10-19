#include "ShooterProjectileBase.h"

#include "DrawDebugHelpers.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AShooterProjectileBase::AShooterProjectileBase()
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->AlwaysLoadOnClient = true;
	CollisionComp->AlwaysLoadOnServer = true;
	CollisionComp->bTraceComplexOnMove = false;
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	CollisionComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	SetRootComponent(CollisionComp);
	
	InterpolatedSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("InterpolatedComp"));
	InterpolatedSceneComp->SetupAttachment(CollisionComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	MovementComp->SetUpdatedComponent(CollisionComp);
	MovementComp->SetInterpolatedComponent(InterpolatedSceneComp);
	MovementComp->bInterpMovement = true;
	MovementComp->bInterpRotation = false;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;

	bReplicates = true;
	SetReplicatingMovement(true);
}

void AShooterProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CollisionComp->MoveIgnoreActors.Add(GetInstigator());

	InstigatorController = GetInstigatorController();
}

void AShooterProjectileBase::PostNetReceiveVelocity(const FVector& NewVelocity)
{
	MovementComp->Velocity = NewVelocity;
}

void AShooterProjectileBase::PostNetReceiveLocationAndRotation()
{
	const FRepMovement& LocalRepMovement = GetReplicatedMovement();
	const FVector NewLocation = FRepMovement::RebaseOntoLocalOrigin(LocalRepMovement.Location, this);
	MovementComp->MoveInterpolationTarget(NewLocation, LocalRepMovement.Rotation);
}

