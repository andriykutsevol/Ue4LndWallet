// Fill out your copyright notice in the Description page of Project Settings.

#include "LNDNode.h"


#include <lnrpc/rpc.grpc.pb.h>


#pragma warning (disable : 4244)
#pragma warning (disable : 4125)// decimal digit terminates...
#pragma warning (disable : 4647)// behavior change __is_pod...
#pragma warning (disable : 4668)// 'symbol' is not defined as a preprocessor macro...
#pragma warning (disable : 4456)// declaration of 'size' hides previous local declaration
#pragma warning (disable : 4577)// 'noexcept' used with no exception handling mode specified
#pragma warning (disable : 4946)// reinterpret_cast used between related classes
#pragma warning (disable : 4005)// 'TEXT': macro redefinition
#pragma warning (disable : 4582)// constructor is not implicitly called
#pragma warning (disable : 4583)// destructor is not implicitly called
#pragma warning (disable : 4800)// Implicit conversion from 'type' to bool. Possible information loss


ULNDNode::ULNDNode() {

	UE_LOG(LogTemp, Warning, TEXT("ULNDNode::ULNDNode()"));

	this->DebugLevel = "debug";	//Valid levels are {trace, debug, info, warn, error, critical}
	this->ListenHostPort = "localhost:9735";
	this->RPCListenHostPort = "localhost:10009";
	this->TCPListenHostPort = "localhost:8080";
	this->BitcoindRPCUser = "ok";
	this->BitcoindRPCPass = "ok";
	this->Bitcoindzmqpubrawblock = "tcp://127.0.0.1:28332";
	this->Bitcoindzmqpubrawtx = "tcp://127.0.0.1:28333";
	this->NodeStatus = "OFF";
	this->WalletStatus = "OFF";
	
	this->basedir = this->get_base_dir();
	this->datadir = this->get_data_dir();
	this->exepath = this->get_exe_path();

}

bool create_channel() {


	return true;
}


void ULNDNode::BeginDestroy() {
	Super::BeginDestroy();

	this->node_stop();

	UE_LOG(LogTemp, Warning, TEXT("ULNDNode::BeginDestroy()"));
}

FString ULNDNode::get_base_dir() {
	FString dir;
	dir = FPaths::Combine(FPaths::ProjectDir(), TEXT("Plugins"), TEXT("LNDWalletPlugin"), TEXT("Source"), TEXT("LNDWalletPlugin"), TEXT("lnd"));
	if (FPaths::DirectoryExists(dir)) {
		return FPaths::ConvertRelativePathToFull(dir);
	}else{
		return "Error: get_base_dir()";
	}
	
}

FString ULNDNode::get_data_dir() {
	FString dir;
	dir = FPaths::Combine(this->basedir, TEXT("data"));
	if (FPaths::DirectoryExists(dir)) {
		return FPaths::ConvertRelativePathToFull(dir);
	}
	else {
		return "Error: get_data_dir()";
	}

}


bool ULNDNode::cleanup_data_dir() {
	UE_LOG(LogTemp, Warning, TEXT("ULNDNode::cleanup_data_dir()"));

	bool is_folder_deleted;

	is_folder_deleted = IFileManager::Get().DeleteDirectory(*this->datadir, false, true);
	is_folder_deleted = IFileManager::Get().DeleteDirectory(*this->datadir, false, true);
	is_folder_deleted = IFileManager::Get().DeleteDirectory(*this->datadir, false, true);

	FPlatformProcess::Sleep(1);
	is_folder_deleted = IFileManager::Get().DeleteDirectory(*this->datadir, false, true);
	FPlatformProcess::Sleep(1);
	is_folder_deleted = IFileManager::Get().DeleteDirectory(*this->datadir, false, true);

	if (is_folder_deleted) {
		UE_LOG(LogTemp, Warning, TEXT("lnd datadir was removed"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("ERROR: Cannot Remove lnd datadir"));
	}

	bool is_folder_recreated = false;

	if (is_folder_deleted) {

		FString path = FPaths::Combine(this->basedir, TEXT("data"));
		is_folder_recreated = IFileManager::Get().MakeDirectory(*path);

		if (is_folder_recreated) {
			UE_LOG(LogTemp, Warning, TEXT("lnd datadir was recreated"));
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("ERROF: Cannot recreate lnd datadir"));
		}
	}

	//IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	//if (!PlatformFile.DirectoryExists(*path)) {
	//	UE_LOG(LogTemp, Warning, TEXT("lnd datadir was created"));
	//	is_folder_recreated = true;
	//} else {
	//	UE_LOG(LogTemp, Warning, TEXT("ERROF: unable to create lnd datadir"));
	//	is_folder_recreated = false;
	//}

	return is_folder_recreated;
}


FString ULNDNode::get_exe_path() {
	FString lnd_exe_path;
	lnd_exe_path = FPaths::Combine(this->basedir, TEXT("bin"), TEXT("lnd.exe"));
	lnd_exe_path = FPaths::ConvertRelativePathToFull(lnd_exe_path);

	if (FPaths::FileExists(lnd_exe_path)) {
		return lnd_exe_path;
	}else{
		return "Error: get_exe_path()";
	}
}

FString ULNDNode::get_command_line() {

	FString cmdl;
	cmdl = "--lnddir=" + this->datadir;
	cmdl += " --debuglevel=" + this->DebugLevel;
	cmdl += " --listen=" + this->ListenHostPort;
	cmdl += " --rpclisten=" + this->RPCListenHostPort;
	cmdl += " --restlisten=" + this->TCPListenHostPort;

	cmdl += " --bitcoin.active";
	cmdl += " --bitcoin.regtest";
	cmdl += " --bitcoin.node=bitcoind";

	cmdl += " --bitcoind.rpcuser=" + this->BitcoindRPCUser;
	cmdl += " --bitcoind.rpcpass=" + this->BitcoindRPCPass;
	cmdl += " --bitcoind.zmqpubrawblock=" + this->Bitcoindzmqpubrawblock;
	cmdl += " --bitcoind.zmqpubrawtx=" + this->Bitcoindzmqpubrawtx;

	return cmdl;

}


FString ULNDNode::node_run() {
	// Для этого нужно создавать отдельный blueprint на основе этого класса.
	//this->node_status_updated();

	// ISSUE https://github.com/lightningnetwork/lnd/issues/4466
	FString ch_backup_fname = FPaths::Combine(this->datadir, TEXT("data"), TEXT("chain"), TEXT("bitcoin"), TEXT("regtest"), TEXT("channel.backup"));
	IFileManager::Get().Delete(*ch_backup_fname, false, false, true);
	IFileManager::Get().Delete(*ch_backup_fname, false, false, true);
	IFileManager::Get().Delete(*ch_backup_fname, false, false, true);


	UE_LOG(LogTemp, Warning, TEXT("this->DebugLevel: %s"), *this->DebugLevel);
	UE_LOG(LogTemp, Warning, TEXT("this->basedir: %s"), *this->basedir);
	UE_LOG(LogTemp, Warning, TEXT("this->datadir: %s"), *this->datadir);
	UE_LOG(LogTemp, Warning, TEXT("this->exepath: %s"), *this->exepath);
	UE_LOG(LogTemp, Warning, TEXT("this->cmdline: %s"), *this->get_command_line());

	if (this->is_node_proc_running()) {
		return this->NodeStatus;
	}

	this->proc = FPlatformProcess::CreateProc(*this->exepath, *this->get_command_line(), true, false, true, &this->procid, 0, nullptr, nullptr, nullptr);
	//FPlatformProcess::Sleep(3);
	if (!this->proc.IsValid()) {
		UE_LOG(LogTemp, Warning, TEXT("ULNDNode::node_run(); Error"));
		this->NodeStatus = "OFF";
	}else {
		this->NodeStatus = "ON";
	}


	std::shared_ptr<grpc::ChannelCredentials> chancreds_shard_ptr = this->get_ssl_channel_credentials();
	FString s = this->RPCListenHostPort;
	this->channel = grpc::CreateChannel(std::string(TCHAR_TO_UTF8(*s)), chancreds_shard_ptr);

	this->wallet_stub = lnrpc::Lightning::NewStub(this->channel);
	this->unlocker_stub = lnrpc::WalletUnlocker::NewStub(this->channel);
	this->router_stub = routerrpc::Router::NewStub(this->channel);

	return this->NodeStatus;
}

bool ULNDNode::is_node_proc_running() {
	bool is_running = false;
	if (FPlatformProcess::IsProcRunning(this->proc)) {
		this->NodeStatus = "ON";
		//this->WalletStatus = "ON";  // Remember? Only LNDWallet.cpp can set this to "ON"
		is_running = true;
	}else{
		this->NodeStatus = "OFF";
		this->WalletStatus = "OFF";
		is_running = false;
	}

	return is_running;
}

bool ULNDNode::node_stop() {

	// TODO. It does not works. Why.
	//grpc::ClientContext context;
	//lnrpc::StopRequest req;
	//lnrpc::StopResponse resp;
	//this->wallet_stub->StopDaemon(&context, req, &resp);

	FPlatformProcess::TerminateProc(this->proc);
	return true;

}


std::shared_ptr<grpc::ChannelCredentials> ULNDNode::get_ssl_channel_credentials() {

	FString cert_file_path = FPaths::Combine(this->datadir, TEXT("tls.cert"));
	grpc::SslCredentialsOptions ssl_opts;
	bool tls_was_created = false;

	// Waiting for tls.cert file.
	for (int i=0; i < 300; i++) {

		FPlatformProcess::Sleep(0.1);

		if (FPaths::FileExists(cert_file_path)) {
			std::ifstream ifs(TCHAR_TO_UTF8(*cert_file_path));

			std::string root_cert((std::istreambuf_iterator<char>(ifs)),
				(std::istreambuf_iterator<char>()));

			ssl_opts = { root_cert, "", "" };
			tls_was_created = true;
			break;
		}
	}

	if (!tls_was_created) {
		UE_LOG(LogTemp, Warning, TEXT("Error: Cannot find tls.cert file"));
		ssl_opts = { "Error: Cannot find tls.cert file", "", "" };
	}

	return grpc::SslCredentials(ssl_opts);
}


std::stringstream ULNDNode::convert_macaroon_to_hex(char* filename) {

	std::ifstream file(filename, std::ios::binary | std::ios::ate);

	file.unsetf(std::ios::skipws);

	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<char> buffer(size);

	std::stringstream ss;
	ss << std::hex;

	if (file.read(buffer.data(), size))
	{

		for (int i = 0; i < size; i++) {

			int c = (int)(unsigned char)buffer[i];

			ss << std::setw(2) << std::setfill('0') << c;
		}
	}
	return ss;
}

std::string ULNDNode::get_macaroon() {

	bool macaroon_was_created = false;
	std::string macaroon_hex = "Error: Cannot find admin.macaroon file";

	FString macaroon_file_path = FPaths::Combine(this->datadir, TEXT("data"), TEXT("chain"), TEXT("bitcoin"), TEXT("regtest"), TEXT("admin.macaroon"));

	// Waiting for admin.macaroon file.
	for (int i = 0; i < 50; i++) {
	
		FPlatformProcess::Sleep(0.5);

		if (FPaths::FileExists(macaroon_file_path)) {
			macaroon_hex = convert_macaroon_to_hex(TCHAR_TO_UTF8(*macaroon_file_path)).str();
			this->Macaroon = macaroon_hex;
			macaroon_was_created = true;
			break;
		}
	}

	if (!macaroon_was_created) {
		UE_LOG(LogTemp, Warning, TEXT("Error: Cannot find admin.macaroon file"));
	}
	else {
		FString f_macaroon_hex(macaroon_hex.c_str());
		UE_LOG(LogTemp, Warning, TEXT("Macaroon: %s"), *f_macaroon_hex);
	}


	return macaroon_hex;

}