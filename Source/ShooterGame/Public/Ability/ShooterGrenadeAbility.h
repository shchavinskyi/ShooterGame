

#pragma once

#include "CoreMinimal.h"
#include "ShooterAbility.h"
#include "ShooterGrenadeAbility.generated.h"

UCLASS(Abstract, Blueprintable)
class UShooterGrenadeAbility : public UShooterAbility
{
	GENERATED_BODY()

	UShooterGrenadeAbility();

protected:
	virtual void Activate() override;
	virtual bool CanActivate() const override;
	virtual void End() override;


private:
	void SpawnProjective() const;
	void PlayAnimation();

	UAnimMontage* GetMontageToPlay() const;

	void OnMontageEnded(class UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
	
	UPROPERTY(EditDefaultsOnly, Category = "GrenadeAbility|Animation")
	UAnimMontage* Throw1P;

	UPROPERTY(EditDefaultsOnly, Category = "GrenadeAbility|Animation")
	UAnimMontage* Throw3P;

	UPROPERTY(EditDefaultsOnly, Category = "GrenadeAbility|Animation")
	FName ThrowNotifyName;

	UPROPERTY(EditDefaultsOnly, Category = "GrenadeAbility")
	float GrenadeUpRotation = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "GrenadeAbility")
	TSubclassOf<class AShooterGrenadeProjectile> GrenadeClass;
};
