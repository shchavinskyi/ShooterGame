

#pragma once

#include "CoreMinimal.h"
#include "ShooterAbility.h"
#include "ShooterGrenadeAbility.generated.h"

UCLASS(Abstract, Blueprintable)
class UShooterGrenadeAbility : public UShooterAbility
{
	GENERATED_BODY()
protected:
	virtual void Activate() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "GrenadeAbility|Animation")
	UAnimMontage* Throw1P;

	UPROPERTY(EditDefaultsOnly, Category = "GrenadeAbility|Animation")
	UAnimMontage* Throw3P;

	UPROPERTY(EditDefaultsOnly, Category = "GrenadeAbility")
	float GrenadeUpRotation = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "GrenadeAbility")
	TSubclassOf<class AShooterGrenadeProjectile> GrenadeClass;
};
