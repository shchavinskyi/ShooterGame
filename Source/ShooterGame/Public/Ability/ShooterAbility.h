
#pragma once

#include "CoreMinimal.h"

#include "ShooterAbility.generated.h"

/**
* 	An abstraction different character abilities.
*/
UCLASS(Abstract)
class UShooterAbility : public UObject
{
	GENERATED_BODY()
	
public:
	UShooterAbility();
	
	// TODO Remove character dependency
	// Now is used to get 1P or 3P mesh
	class AShooterCharacter* GetAvatar() const;

	// Return true if avatar has authority 
	bool HasAuthority() const;

	// Return true when ability is active 
	FORCEINLINE bool IsActive() const { return bIsActive; };

	// Return true when ability can be activated 
	virtual bool CanActivate() const;
	
	virtual void Activate();
	virtual void End();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	float Cooldown = -1.0f;

private:
	bool bIsActive = false;
	float LastActivationTime = FLT_MIN;
};