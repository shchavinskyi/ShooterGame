


#include "Ability/ShooterGrenadeAbility.h"

#include "ShooterGrenadeProjectile.h"
#include "ShooterWeapon.h"

UShooterGrenadeAbility::UShooterGrenadeAbility()
{
	// Default ability cooldown
	Cooldown = 2.0f;
}

void UShooterGrenadeAbility::Activate()
{
	Super::Activate();

	PlayAnimation();
}

bool UShooterGrenadeAbility::CanActivate() const
{
	// TODO Reload also should block grenade or be canceled by it
	return Super::CanActivate() && !GetAvatar()->IsFiring();
}

void UShooterGrenadeAbility::End()
{
	if (IsActive())
	{
		const AShooterCharacter* Avatar = GetAvatar();

		if (Avatar->IsFirstPerson())
		{
			if (AShooterWeapon* CurrentWeapon = Avatar->GetWeapon())
			{
				CurrentWeapon->AttachMeshToPawn();
			}
		}

		if (UAnimInstance* AnimInstance = Avatar->GetPawnMesh()->GetAnimInstance())
		{
			if (HasAuthority())
			{
				AnimInstance->OnPlayMontageNotifyBegin.RemoveAll(this);	
			}
			
			if (FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(GetMontageToPlay()))
			{
				MontageInstance->OnMontageEnded.Unbind();
			}

			AnimInstance->Montage_Stop(0.0f, GetMontageToPlay());
		}
	}

	Super::End();
}



void UShooterGrenadeAbility::SpawnProjective() const
{
	AShooterCharacter* Avatar = GetAvatar();
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = Avatar;
	SpawnParameters.Owner = Avatar;

	FRotator GrenadeRotation = Avatar->GetBaseAimRotation();
	GrenadeRotation.Pitch += GrenadeUpRotation;

	// TODO spawn from socket location
	const FVector SpawnLocation = Avatar->GetActorLocation() + Avatar->GetBaseAimRotation().Vector() * 150.0f + Avatar->GetActorUpVector() * 50.0f;

	AShooterGrenadeProjectile* Grenade = GetWorld()->SpawnActor<AShooterGrenadeProjectile>(GrenadeClass, SpawnLocation, FRotator::ZeroRotator, SpawnParameters);
	Grenade->InitVelocity(GrenadeRotation.Vector(), Avatar->GetVelocity());

	// Add grenade to move ignore actors
	Avatar->GetCapsuleComponent()->MoveIgnoreActors.Add(Grenade);
}

void UShooterGrenadeAbility::PlayAnimation()
{
	const AShooterCharacter* Avatar = GetAvatar();

	if (UAnimInstance* AnimInstance = Avatar->GetPawnMesh()->GetAnimInstance())
	{
		AnimInstance->StopAllMontages(0.0f);

		if (HasAuthority())
		{
			AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UShooterGrenadeAbility::OnMontageNotifyBegin);	
		}

		UAnimMontage* MontageToPlay = GetMontageToPlay();
		AnimInstance->Montage_Play(MontageToPlay);
		if (FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(MontageToPlay))
		{
			MontageInstance->OnMontageEnded.BindUObject(this, &UShooterGrenadeAbility::OnMontageEnded);
		}

		// detach weapon + subscribe for notify
		if (Avatar->IsFirstPerson())
		{
			Avatar->GetWeapon()->DetachMeshFromPawn();
		}
	}
}

UAnimMontage* UShooterGrenadeAbility::GetMontageToPlay() const
{
	return GetAvatar()->IsFirstPerson() ? Throw1P : Throw3P;
}

void UShooterGrenadeAbility::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	End();
}

void UShooterGrenadeAbility::OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	check(HasAuthority());
	
	if (NotifyName == ThrowNotifyName)
	{
		SpawnProjective();
	}
}
