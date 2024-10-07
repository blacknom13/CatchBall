#pragma once

#include "Gender.generated.h"

UENUM(BlueprintType)
enum class EGender: uint8
{
	None	UMETA(DisplayName="None"),
	Male	UMETA(DisplayName="Male"),
	Female	UMETA(DisplayName="Female")
};
