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

bool UShooterAbility::CanActivate() const
{
	return !IsActive() && (LastActivationTime + Cooldown) < GetAvatar()->GetWorld()->GetTimeSeconds();
}

void UShooterAbility::Activate()
{
	check(!IsActive());
	
	bIsActive = true;
	LastActivationTime = GetAvatar()->GetWorld()->GetTimeSeconds();
}

void UShooterAbility::End()
{
	bIsActive = false;
}
