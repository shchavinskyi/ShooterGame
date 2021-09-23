#pragma once

#include "CoreMinimal.h"
#include "ShooterProjectileBase.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "ShooterGrenadeProjectile.generated.h"

UCLASS(Abstract, Blueprintable)
class SHOOTERFRAMEWORK_API AShooterGrenadeProjectile : public AShooterProjectileBase
{
	GENERATED_BODY()

public:
	AShooterGrenadeProjectile();

	void InitVelocity(const FVector& ThrowDirection, const FVector& ParentVelocity) const;

protected:
	virtual void PostInitializeComponents() override;
	virtual void PostInitProperties() override;
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	UFUNCTION()
	void OnBounced(const FHitResult& ImpactResult, const FVector& ImpactVelocity);
	
private:
	UPROPERTY(EditDefaultsOnly, Category = Grenade)
	float TimeToExplode = 2.0f;

	UPROPERTY(EditDefaultsOnly, Category = Grenade)
	FRotator RotationRate;

	UPROPERTY(EditDefaultsOnly, Category = Grenade)
	float ExplosionRadius = 300.0f;

	UPROPERTY(EditDefaultsOnly, Category = Grenade)
	float ExplosionDamage = 50.0f;

	// Amount of damage need to take to explode
	UPROPERTY(EditDefaultsOnly, Category = Grenade)
	float DamageThreshold = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = Grenade)
	TSubclassOf<UDamageType> DamageType;

	// TODO base effect class should be moved to ShooterFramework
	// For now we allow all classes
	UPROPERTY(EditDefaultsOnly, Category = Grenade)
	TSubclassOf<class AActor> ExplosionTemplate;

	UPROPERTY(EditDefaultsOnly, Category=Grenade)
	class USoundCue* BounceSound;
	
	UPROPERTY(VisibleDefaultsOnly, Category=Grenade)
	UStaticMeshComponent* GrenadeMeshComp;

	UPROPERTY(VisibleDefaultsOnly, Category=Grenade)
	URotatingMovementComponent* RotationComp;
};
