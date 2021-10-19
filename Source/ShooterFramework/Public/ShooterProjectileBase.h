#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "Engine/Classes/GameFramework/Actor.h"
#include "ShooterProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS(Abstract)
class SHOOTERFRAMEWORK_API AShooterProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	AShooterProjectileBase();

protected:
	FORCEINLINE UProjectileMovementComponent* GetMovementComp() const { return MovementComp; }
	FORCEINLINE USphereComponent* GetCollisionComp() const { return CollisionComp; }
	FORCEINLINE USceneComponent* GetInterpolatedSceneComp() const { return InterpolatedSceneComp; }

	virtual void PostInitializeComponents() override;
	virtual void PostNetReceiveVelocity(const FVector& NewVelocity) override;
	virtual void PostNetReceiveLocationAndRotation() override;

	/** controller that fired me (cache for damage calculations) */
	TWeakObjectPtr<AController> InstigatorController;

private:
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	USphereComponent* CollisionComp;
	
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	UProjectileMovementComponent* MovementComp;
	
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	USceneComponent* InterpolatedSceneComp;
};
