#include "Ability/ShooterAbility.h"

UShooterAbility::UShooterAbility()
{
	
}

AShooterCharacter* UShooterAbility::GetAvatar() const
{
	return CastChecked<AShooterCharacter>(GetOuter());
}

bool UShooterAbility::HasAuthority() const
{
	return GetAvatar()->HasAuthority();
}
