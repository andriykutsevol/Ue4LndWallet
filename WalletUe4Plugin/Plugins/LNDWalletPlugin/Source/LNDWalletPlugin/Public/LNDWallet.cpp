// Fill out your copyright notice in the Description page of Project Settings.

#include "LNDWallet.h"

#include <lnrpc/rpc.grpc.pb.h>

ULNDWallet::ULNDWallet() {
	UE_LOG(LogTemp, Warning, TEXT("ULNDWallet::ULNDWallet()"));

	this->SeedPhrase = "";			// TODO. Try to make it work like the Newaddress.
	this->WalletPassword = "";
	this->NewAddress = "";			// TODO. Can we remove this from the workflow. Because
									// there is a function that returns Newaddress.
	this->Node = nullptr;

}

void ULNDWallet::BeginDestroy() {
	Super::BeginDestroy();

	UE_LOG(LogTemp, Warning, TEXT("ULNDWallet::BeginDestroy()"));
}


bool ULNDWallet::setup_lnd_node(ULNDNode* lnd_node) {

	this->Node = lnd_node;
	UE_LOG(LogTemp, Warning, TEXT("ULNDWallet::setup_lnd_node(): %s"), *this->Node->basedir);

	return true;
}

// TODO. It is mandatory to display the result of the operation on the graphical interface.
bool ULNDWallet::remove_wallet() {
	UE_LOG(LogTemp, Warning, TEXT("ULNDWallet::remove_wallet()"));

	//TODO make the same for each function
	if (this->Node == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("ERROR: ULNDWallet::remove_wallet(): you have to call setup_lnd_node() first"));
		return false;
	}

	return this->Node->cleanup_data_dir();
}


bool ULNDWallet::gen_seed_phrase() {

	if (this->Node->is_node_proc_running()) {
		this->SeedPhrase = "";

		lnrpc::GenSeedRequest genseedRequest;
		lnrpc::GenSeedResponse genseedResponse;

		grpc::ClientContext context;
		this->Node->unlocker_stub->GenSeed(&context, genseedRequest, &genseedResponse);
		UE_LOG(LogTemp, Warning, TEXT("cipher_seed_mnemonic size: %d"), genseedResponse.cipher_seed_mnemonic().size());

		for (int i = 0; i < 24; i++) {
			FString seed_word(genseedResponse.cipher_seed_mnemonic().Get(i).c_str()); // TODO. Make it robust when no seed was generated.
			UE_LOG(LogTemp, Warning, TEXT("cipher_seed_mnemonic: %s"), *seed_word);

			this->seed_phrase_vector.push_back(seed_word);
			this->SeedPhrase += seed_word + " ";
		}

		this->SeedPhrase = this->SeedPhrase.TrimStartAndEnd();
		return true;
	}



	return false;
}

// TODO. Is it worth to set the password in the way as in unlock_wallet() does.
// TODO. lnrpc::InitWalletResponse return admin_macaroon... maybe it is better use it rather than wait for file created
// as it is done in this->Node->get_macaroon();
bool ULNDWallet::initialize_wallet() {
	UE_LOG(LogTemp, Warning, TEXT("ULNDWallet::initialize_wallet()"));

	if (this->WalletPassword == "") {
		UE_LOG(LogTemp, Warning, TEXT("ERROR: initialize_wallet(): this->WalletPassword is not set"));
		return false;
	}

	lnrpc::InitWalletRequest initwalletreq;
	auto wpswd = TCHAR_TO_ANSI(*this->WalletPassword);
	initwalletreq.set_wallet_password(wpswd, strlen(wpswd));
	UE_LOG(LogTemp, Warning, TEXT("wallet password: %s"), *this->WalletPassword);


	this->gen_seed_phrase();

	for (int i = 0; i < 24; i++) {
		initwalletreq.add_cipher_seed_mnemonic(TCHAR_TO_ANSI(*this->seed_phrase_vector[i]));
	}

	grpc::ClientContext context;
	lnrpc::InitWalletResponse initwalletresp;
	this->Node->unlocker_stub->InitWallet(&context, initwalletreq, &initwalletresp);

	// TODO. Handle errors correctly.
	this->Node->get_macaroon();

	this->Node->WalletStatus = "ON";

	return true;
}


// TODO. If chain is set to "regtest", then if you restart bitcoind, 
// the unlock_wallet() function
// gives an "Birthday block has already been verified" warning, and the LND shuts down.
// Why?
bool ULNDWallet::unlock_wallet(FString password) {

	if (!this->Node->is_node_proc_running()) {
		UE_LOG(LogTemp, Warning, TEXT("ERROR: LND node is not running"));
		return false;
	}

	if (this->WalletPassword == "") {
		UE_LOG(LogTemp, Warning, TEXT("ERROR: unlock_wallet(): wallet password is not set"));
		return false;
	}

	grpc::ClientContext context;

	lnrpc::UnlockWalletRequest wunlock_req;
	//wunlock_req.set_wallet_password("okokokok");
	wunlock_req.set_wallet_password(TCHAR_TO_ANSI(*password));


	lnrpc::UnlockWalletResponse wunlock_resp;

	this->Node->unlocker_stub->UnlockWallet(&context, wunlock_req, &wunlock_resp);

	// TODO. Handle errors correctly.
	this->Node->get_macaroon();

	this->Node->WalletStatus = "ON";

	return true;

}


// This will not work until the Node finishes scanning (5 minutes for 501 blocks).
// ... in lnd.log:
// LNWL: Catching up block hashes to height 501, this might take a while
// LNWL : Done catching up block hashes
// LNWL : Finished rescan for 0 addresses(synced to block 4c07cd6b5ad, height 501)
// ISSUE: https://github.com/lightningnetwork/lnd/issues/2295
// ISSUE: https://github.com/lightningnetwork/lnd/issues/760
FString ULNDWallet::get_new_address() {

	lnrpc::NewAddressRequest newaddrreq;
	newaddrreq.set_type(lnrpc::AddressType::WITNESS_PUBKEY_HASH);

	lnrpc::NewAddressResponse newaddrresp;

	grpc::ClientContext context;
	context.AddMetadata("macaroon", this->Node->Macaroon);

	this->Node->wallet_stub->NewAddress(&context, newaddrreq, &newaddrresp);

	this->NewAddress = FString(newaddrresp.address().c_str());

	UE_LOG(LogTemp, Warning, TEXT("New Address: %s"), *this->NewAddress);

	return this->NewAddress;

}


FString ULNDWallet::get_ballance() {


	grpc::ClientContext context;
	context.AddMetadata("macaroon", this->Node->Macaroon);

	lnrpc::WalletBalanceResponse resp;
	const lnrpc::WalletBalanceRequest req;

	this->Node->wallet_stub->WalletBalance(&context, req, &resp);

	//UE_LOG(LogTemp, Warning, TEXT("total_balance: %d, confirmed_balance: %d, unconfirmed_balance: %d"), resp.total_balance(), resp.confirmed_balance(), resp.unconfirmed_balance());
	TArray< FStringFormatArg > args;
	args.Add(FStringFormatArg(resp.total_balance()));
	args.Add(FStringFormatArg(resp.confirmed_balance()));
	args.Add(FStringFormatArg(resp.unconfirmed_balance()));

	FString balance = FString::Format(TEXT("{0}\n{1}\n{2}"), args);

	return balance;

}

// Connection to other node works only after wallet synchronization.
bool ULNDWallet::connect_peer(FString hostport, FString pubkey) {

	grpc::ClientContext context;
	context.AddMetadata("macaroon", this->Node->Macaroon);

	UE_LOG(LogTemp, Warning, TEXT("hostport: %s, pubkey: %s"), *hostport, *pubkey);

	lnrpc::ConnectPeerResponse resp;
	lnrpc::ConnectPeerRequest req;

	lnrpc::LightningAddress* peer_addr;
	peer_addr = new lnrpc::LightningAddress;

	peer_addr->set_host(TCHAR_TO_ANSI(*hostport));
	peer_addr->set_pubkey(TCHAR_TO_ANSI(*pubkey));
	
	req.set_allocated_addr(peer_addr);

	

	lnrpc::LightningAddress test_addr = req.addr();
	std::string hs = test_addr.host();
	FString hf(hs.c_str());
	UE_LOG(LogTemp, Warning, TEXT("test_addr.host: %s"), *hf);

	std::string ps = test_addr.pubkey();
	FString pf(ps.c_str());
	UE_LOG(LogTemp, Warning, TEXT("test_addr.pubkey: %s"), *pf);


	this->Node->wallet_stub->ConnectPeer(&context, req, &resp);

	//delete peer_addr; //TODO Now it thwors an Exception. Why?

	return true;
}


FString ULNDWallet::list_peers() {

	FString list_of_peers = "None";

	grpc::ClientContext context;
	context.AddMetadata("macaroon", this->Node->Macaroon);

	lnrpc::ListPeersRequest req;
	req.set_latest_error(true);

	lnrpc::ListPeersResponse resp;

	this->Node->wallet_stub->ListPeers(&context, req, &resp);

	if (resp.peers().size() > 0) {

		list_of_peers = "";

		for (int p = 0; p < resp.peers().size(); p++) {

			auto peer = resp.peers().Get(p);

			FString ppk(peer.pub_key().c_str());

			list_of_peers += ppk + "\n";
		}
	}
	return list_of_peers;
}



std::vector<char> HexToBytes(const std::string& hex) {
	std::vector<char> bytes;

	for (unsigned int i = 0; i < hex.length(); i += 2) {
		std::string byteString = hex.substr(i, 2);
		char byte = (char)strtol(byteString.c_str(), NULL, 16);
		bytes.push_back(byte);
	}

	return bytes;
}


//TODO. Get rid of that strange letters in output.
FString ULNDWallet::open_channel(FString pubkey, FString loc_amt_str) {

	FString response = "";

	grpc::ClientContext context;
	context.AddMetadata("macaroon", this->Node->Macaroon);

	lnrpc::OpenChannelRequest req;
	//openchannel_request.set_node_pubkey(getinforesponse2.identity_pubkey().c_str());
	//https://github.com/protocolbuffers/protobuf/issues/5431
	//openchannel_request.set_node_pubkey("\xE0\xA4\xAE\xE0\xA4\xB9\xE0\xA4\xB8\xE0\xA5\x81\xE0\xA4\xB8");

	UE_LOG(LogTemp, Warning, TEXT("pubkey: %s"), *pubkey);

	std::string pk = std::string(TCHAR_TO_UTF8(*pubkey));

	std::vector<char> hexbytes = HexToBytes(pk);
	req.set_node_pubkey(hexbytes.data());

	FString cpk(req.node_pubkey().c_str());
	UE_LOG(LogTemp, Warning, TEXT("Channel node pubkey: %s"), *cpk);
	UE_LOG(LogTemp, Warning, TEXT("Channel node pubkey: %x"), *cpk);

	google::protobuf::int64 loc_amt = FCString::Strtoi64(*loc_amt_str, NULL, 10);
	req.set_local_funding_amount(loc_amt);
	req.set_min_confs(1);


	auto reader = this->Node->wallet_stub->OpenChannel(&context, req);

	lnrpc::OpenStatusUpdate status;
	//lnrpc::PendingUpdate pending;
	lnrpc::ChannelOpenUpdate opened;

	// TODO. The cannel to be opened needs to be confirmed by bitcoind.
	// There fore here we just waiting for the channel to be peinding and move on.
	// Latter the bitcoind will confirm opening transaction.
	// TODO. Trace the channel state in user friendly way.

	while (reader->Read(&status)) {


		
		std::string pending_chan_id_str = status.pending_chan_id();

		if (pending_chan_id_str.size() > 0) {
			FString tmp = FString(pending_chan_id_str.c_str());
			UE_LOG(LogTemp, Warning, TEXT("pending_chan_id: %s"), *tmp);
			context.TryCancel();
			break;
		}else {
			UE_LOG(LogTemp, Warning, TEXT("ERROR: reader->Read(&status): BUT pending_chan_id_str.size() < 0")); //TODO. To handle it correctly.
		}


		
		//opened = status.chan_open();


		//FString pending_txid(pending.txid().c_str());
		//FString funding_txid(opened.channel_point().funding_txid_str().c_str());

		//UE_LOG(LogTemp, Warning, TEXT("pending_txid: %s"), *pending_txid);
		//UE_LOG(LogTemp, Warning, TEXT("funding_txid: %x"), *cpk);

	}

	//UE_LOG(LogTemp, Warning, TEXT("Before Try Cancel"));
	//for (int i = 0; i <= 20; i++) {
	//	UE_LOG(LogTemp, Warning, TEXT("%d"), i);
	//	FPlatformProcess::Sleep(1);
	//}


	//UE_LOG(LogTemp, Warning, TEXT("AFTER Try Cancel"));
	//for (int i = 0; i <= 20; i++) {
	//	UE_LOG(LogTemp, Warning, TEXT("%d"), i);
	//	FPlatformProcess::Sleep(1);
	//}

	UE_LOG(LogTemp, Warning, TEXT("AFTER reader->Read()"));

	return response;
}

FString ULNDWallet::pending_channels() {

	std::string response = "";

	grpc::ClientContext context;
	context.AddMetadata("macaroon", this->Node->Macaroon);

	lnrpc::PendingChannelsRequest req;
	lnrpc::PendingChannelsResponse resp;

	this->Node->wallet_stub->PendingChannels(&context, req, &resp);

	auto pending_opened_channels = resp.pending_open_channels();
	auto pending_opened_channels_size = pending_opened_channels.size();

	if (pending_opened_channels_size > 0) {
		for (int i = 0; i < pending_opened_channels_size; i++) {
			response += pending_opened_channels[i].channel().remote_node_pub() + "\n";
		}
	}

	return FString(response.c_str());
}


FString ULNDWallet::opened_channels() {

	std::string response = "";

	grpc::ClientContext context;
	context.AddMetadata("macaroon", this->Node->Macaroon);


	lnrpc::ListChannelsRequest req;
	lnrpc::ListChannelsResponse resp;

	this->Node->wallet_stub->ListChannels(&context, req, &resp);

	auto opened_channels = resp.channels();
	auto opened_channels_size = opened_channels.size();

	if (opened_channels_size > 0) {
		for (int i = 0; i < opened_channels_size; i++) {
			response += opened_channels[i].remote_pubkey() + "\n";
		}
	}

	return FString(response.c_str());
}

FString ULNDWallet::send_payment_v2(FString payment_request) {

	FString payment_hash = "";

	UE_LOG(LogTemp, Warning, TEXT("payment_request: %s"), *payment_request);

	std::string payreq_string = std::string(TCHAR_TO_UTF8(*payment_request));

	grpc::ClientContext context;
	context.AddMetadata("macaroon", this->Node->Macaroon);

	routerrpc::SendPaymentRequest req;

	req.set_payment_request(payreq_string.data());

	req.set_timeout_seconds(60);
	req.set_fee_limit_sat(50000);	// TODO. 1)Get correct fee from bitcoind. 2)What is the minimal possible fee?


	auto reader = this->Node->router_stub->SendPaymentV2(&context, req);

	lnrpc::Payment payment;

	while (reader->Read(&payment)) {

		UE_LOG(LogTemp, Warning, TEXT("reader->Read(&payment)"));

		auto status =  payment.status();

		if (status == payment.SUCCEEDED) {
			context.TryCancel();			// TODO. How to tackle this correctly. Is there is a way to track the context state?
			payment_hash = FString(payment.payment_hash().c_str());
			UE_LOG(LogTemp, Warning, TEXT("status == payment.SUCCEEDED"));
			UE_LOG(LogTemp, Warning, TEXT("payment_hash: %s"), *payment_hash);
			break;
		} else {
			auto tmp = FString(std::to_string(status).c_str());
			UE_LOG(LogTemp, Warning, TEXT("status ==  %s"), *tmp);
		}


	}

	return payment_hash;

}

FString ULNDWallet::channel_balances() {

	FString local_balance = "";

	grpc::ClientContext context;
	context.AddMetadata("macaroon", this->Node->Macaroon);

	lnrpc::ChannelBalanceRequest req;
	lnrpc::ChannelBalanceResponse resp;


	this->Node->wallet_stub->ChannelBalance(&context, req, &resp);

	auto lb = resp.balance();

	local_balance = FString(std::to_string(lb).c_str());

	return local_balance;
}



void ULNDWallet::append_info_text(FString info) {
	UE_LOG(LogTemp, Warning, TEXT("FString info: %s"), *info);
}

