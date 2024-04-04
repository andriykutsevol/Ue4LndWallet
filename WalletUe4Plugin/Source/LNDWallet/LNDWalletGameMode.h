// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "LNDNode.h"

#include "LNDWalletGameMode.generated.h"

UCLASS(Blueprintable, BlueprintType)
class ALNDWalletGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALNDWalletGameMode();

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
		//ULNDNode* lndnode;
};



