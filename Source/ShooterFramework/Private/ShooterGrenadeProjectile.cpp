#include "ShooterGrenadeProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
#include "DrawDebugHelpers.h"

static TAutoConsoleVariable<bool> CVarDebugGrenade(
	TEXT("Shooter.DebugGrenage"),
	false,
	TEXT("Whether to enable grenade debugging info."));

#endif // !(UE_BUILD_SHIPPING || UE_BUILD_TEST)

AShooterGrenadeProjectile::AShooterGrenadeProjectile()
{
	GetMovementComp()->bShouldBounce = true;
	GetMovementComp()->bSimulationEnabled = true;
	GetMovementComp()->Bounciness = 0.4;
	GetMovementComp()->InitialSpeed = 2000.0f;
	GetMovementComp()->MaxSpeed = 3000;
	GetMovementComp()->Velocity = FVector::ZeroVector;
	GetMovementComp()->bRotationFollowsVelocity = false;
	GetMovementComp()->ProjectileGravityScale = 1.0f;

	GrenadeMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrenadeMesh"));
	GrenadeMeshComp->SetupAttachment(GetInterpolatedSceneComp());

	RotationComp = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("GrenadeRotation"));
	RotationComp->SetUpdatedComponent(GrenadeMeshComp);
}

void AShooterGrenadeProjectile::InitVelocity(const FVector& ThrowDirection, const FVector& ParentVelocity) const
{
	const float ResultSpeed = GetMovementComp()->InitialSpeed + FVector::DotProduct(ThrowDirection, ParentVelocity);
	GetMovementComp()->Velocity = ThrowDirection * ResultSpeed;
	GetMovementComp()->UpdateComponentVelocity();
}

void AShooterGrenadeProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	GetMovementComp()->OnProjectileBounce.AddDynamic(this, &AShooterGrenadeProjectile::OnBounced);
}

void AShooterGrenadeProjectile::PostInitProperties()
{
	Super::PostInitProperties();

	RotationComp->RotationRate = RotationRate;
}

void AShooterGrenadeProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		// TODO set timer after throw, not after spawning
		SetLifeSpan(TimeToExplode);
	}
}

void AShooterGrenadeProjectile::Destroyed()
{
	if (HasAuthority())
	{
		UGameplayStatics::ApplyRadialDamage(this, ExplosionDamage, GetActorLocation(), ExplosionRadius, DamageType, TArray<AActor*>(), this, InstigatorController.Get());
	}

	if (ExplosionTemplate)
	{
		// TODO Provide hit result for a decal after explosion
		GetWorld()->SpawnActor<AActor>(ExplosionTemplate, GetActorTransform());
	}

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (CVarDebugGrenade.GetValueOnAnyThread())
	{
		const FColor Color = HasAuthority() ? FColor::Green : FColor::Red;
		DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 12, Color, false, 5.0f);
	}
#endif	
}

void AShooterGrenadeProjectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Update rotation rate based on velocity
	const float Scale = GetVelocity().Size()/GetMovementComp()->GetMaxSpeed();
	RotationComp->RotationRate = RotationRate * Scale;
}

float AShooterGrenadeProjectile::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if (DamageThreshold < Damage)
	{
		Destroy();
		return Damage;
	}

	return 0.0f;
}

void AShooterGrenadeProjectile::OnBounced(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	if (BounceSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, BounceSound, GetActorLocation());
	}
}
