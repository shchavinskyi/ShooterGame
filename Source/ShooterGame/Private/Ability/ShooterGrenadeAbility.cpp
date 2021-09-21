


#include "Ability/ShooterGrenadeAbility.h"

#include "ShooterGrenadeProjectile.h"
#include "ShooterWeapon.h"

void UShooterGrenadeAbility::Activate()
{
	AShooterCharacter* Avatar = GetAvatar();
	
	if (HasAuthority())
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Instigator = Avatar;
		SpawnParameters.Owner = Avatar;

		FRotator GrenadeRotation = Avatar->GetBaseAimRotation();
		GrenadeRotation.Pitch += GrenadeUpRotation;

		// TODO spawn from socket location
		const FVector SpawnLocation = Avatar->GetActorLocation() + Avatar->GetBaseAimRotation().Vector() * 150.0f + Avatar->GetActorUpVector() * 50.0f;

		const AShooterGrenadeProjectile* Grenade = GetWorld()->SpawnActor<AShooterGrenadeProjectile>(GrenadeClass, SpawnLocation, FRotator::ZeroRotator, SpawnParameters);
		Grenade->InitVelocity(GrenadeRotation.Vector(), Avatar->GetVelocity());
	}

	if (UAnimInstance* AnimInstance = Avatar->GetPawnMesh()->GetAnimInstance())
	{
		AnimInstance->StopAllMontages(0.0f);

		// Attach/detach weapon + different animation for first person
		if (Avatar->IsFirstPerson())
		{
			Avatar->GetWeapon()->DetachMeshFromPawn();

			AnimInstance->Montage_Play(Throw1P);
			if (FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(Throw1P))
			{
				TWeakObjectPtr<AShooterCharacter> WeakAvatar(Avatar); 
				MontageInstance->OnMontageEnded.BindLambda([WeakAvatar](UAnimMontage*, bool)
				{
					if (WeakAvatar.IsValid())
					{
						WeakAvatar->GetWeapon()->AttachMeshToPawn();	
					}
				});
			}
		}
		else
		{
			AnimInstance->Montage_Play(Throw3P);
		}
	}
}
