// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "LNDWalletGameMode.h"
#include "LNDWalletCharacter.h"
#include "UObject/ConstructorHelpers.h"

#include "LNDNode.h"

ALNDWalletGameMode::ALNDWalletGameMode()
{

	UE_LOG(LogTemp, Warning, TEXT("ALNDWalletGameMode::ALNDWalletGameMode()"));

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	//this->lndnode = NewObject<ULNDNode>((UObject*)GetTransientPackage(), ULNDNode::StaticClass());


}
