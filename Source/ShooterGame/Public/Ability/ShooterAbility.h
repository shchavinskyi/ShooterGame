
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
	
	virtual void Activate() {};
};