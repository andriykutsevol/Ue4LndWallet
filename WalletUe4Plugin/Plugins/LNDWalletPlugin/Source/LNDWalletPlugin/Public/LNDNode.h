// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core.h"
#include "UObject/NoExportTypes.h"
#include "LNDWalletPlugin.h"

#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>

#include <grpcpp/grpcpp.h>
#include <lnrpc/rpc.grpc.pb.h>
#include <lnrpc/routerrpc/router.grpc.pb.h>

#include "LNDNode.generated.h"

/**
 * TODO. IS EditAnywhere, BlueprintReadWrite used correctly.
 */
UCLASS(BlueprintType, Blueprintable)
class LNDWALLETPLUGIN_API ULNDNode : public UObject
{
	GENERATED_BODY()

private:

	FProcHandle proc;
	uint32 procid;

	FString get_base_dir();
	FString get_data_dir();
	FString get_exe_path();
	FString get_command_line();

	std::shared_ptr<grpc::ChannelCredentials> get_ssl_channel_credentials();
	std::stringstream convert_macaroon_to_hex(char* filename);


public:

	ULNDNode();
	void BeginDestroy() override;

	bool cleanup_data_dir();

	FString basedir;
	FString datadir;
	FString exepath;

	std::string get_macaroon();
	std::string Macaroon;


	std::shared_ptr<grpc_impl::Channel> channel;

	std::unique_ptr<lnrpc::Lightning::Stub> wallet_stub;
	std::unique_ptr<lnrpc::WalletUnlocker::Stub> unlocker_stub;
	std::unique_ptr<routerrpc::Router::Stub> router_stub;

	// This can be set to true only from LNDWallet.cpp, and to false only from LNDNode.cpp.
	// It is because Wallet Unlocking happens only when NODE shutdown.
	// TODO: is this true.
	// TODO. Consider making it Enum type.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LNDNode_Plugin)
		FString WalletStatus;

	// TODO. Consider making it Enum type.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LNDNode_Plugin)
		FString NodeStatus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LNDNode_Plugin)
		FString DebugLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LNDNode_Plugin)
		FString ListenHostPort;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LNDNode_Plugin)
		FString RPCListenHostPort;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LNDNode_Plugin)
		FString TCPListenHostPort;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LNDNode_Plugin)
		FString BitcoindRPCUser;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LNDNode_Plugin)
		FString BitcoindRPCPass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LNDNode_Plugin)
		FString Bitcoindzmqpubrawblock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LNDNode_Plugin)
		FString Bitcoindzmqpubrawtx;

	UFUNCTION(BlueprintCallable, Category = LNDNode_Plugin)
		FString node_run();

	UFUNCTION(BlueprintCallable, Category = LNDNode_Plugin)
		bool is_node_proc_running();

	UFUNCTION(BlueprintCallable, Category = LNDNode_Plugin)
		bool node_stop();





	//Для этого нужно создавать отдельный blueprint на основе этого класса.
	//UFUNCTION(BlueprintImplementableEvent)
		//void node_status_updated();	
};
