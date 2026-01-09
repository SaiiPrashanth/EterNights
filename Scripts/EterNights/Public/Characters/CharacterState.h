#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_EquippedOneHandedWeapon UMETA(DisplayName = "OneHandedWeapon"),
	ECS_EquippedTwoHandedWeapon UMETA(DisplayName = "TwoHandedWeapon")
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_UnOccupied,
	EAS_Attacking, 
	EAS_Equipping
};

UENUM(BlueprintType)
enum class EDeathPose : uint8 {
	EDP_Alive,
	EDP_DeathPose1,
	EDP_DeathPose2
};

UENUM(BlueprintType)
enum class EEnemyState : uint8 {
	EES_Patrolling,
	EES_Chasing,
	EES_Attacking
};