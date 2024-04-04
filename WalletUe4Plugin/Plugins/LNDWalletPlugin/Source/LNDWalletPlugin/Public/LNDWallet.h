// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LNDNode.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include <grpcpp/grpcpp.h>
#include <lnrpc/rpc.grpc.pb.h>
#include <lnrpc/routerrpc/router.grpc.pb.h>

#include "LNDWallet.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class LNDWALLETPLUGIN_API ULNDWallet : public UObject
{
	GENERATED_BODY()

private:
	bool gen_seed_phrase();
	std::vector<FString> seed_phrase_vector;


public:
	ULNDWallet();
	void BeginDestroy() override;

	ULNDNode* Node;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LNDNode_Plugin)
		FString SeedPhrase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LNDNode_Plugin)
		FString WalletPassword;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LNDNode_Plugin)
		FString NewAddress;


	UFUNCTION(BlueprintCallable, Category = LNDNode_Plugin)
		bool setup_lnd_node(ULNDNode* lnd_node);

	UFUNCTION(BlueprintCallable, Category = LNDNode_Plugin)
		bool remove_wallet();

	UFUNCTION(BlueprintCallable, Category = LNDNode_Plugin)
		bool initialize_wallet();

	UFUNCTION(BlueprintCallable, Category = LNDNode_Plugin)
		bool unlock_wallet(FString password);

	UFUNCTION(BlueprintCallable, Category = LNDNode_Plugin)
		FString get_new_address();

	UFUNCTION(BlueprintCallable, Category = LNDNode_Plugin)
		FString get_ballance();

	UFUNCTION(BlueprintCallable, Category = LNDNode_Plugin)
		bool connect_peer(FString hostport, FString pubkey);

	UFUNCTION(BlueprintCallable, Category = LNDNode_Plugin)
		FString list_peers();

	UFUNCTION(BlueprintCallable, Category = LNDNode_Plugin)
		FString open_channel(FString pubkey, FString loc_amt_str);

	UFUNCTION(BlueprintCallable, Category = LNDNode_Plugin)
		FString pending_channels();

	UFUNCTION(BlueprintCallable, Category = LNDNode_Plugin)
		FString opened_channels();

	UFUNCTION(BlueprintCallable, Category = LNDNode_Plugin)
		FString send_payment_v2(FString payment_request);


	UFUNCTION(BlueprintCallable, Category = LNDNode_Plugin)
		FString channel_balances();

	UFUNCTION(BlueprintCallable, Category = LNDNode_Plugin)
		void append_info_text(FString info);
	
};
