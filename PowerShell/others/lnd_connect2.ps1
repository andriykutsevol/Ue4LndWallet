
Remove-Item 'C:\Users\Andriy\Documents\Unreal_Projects\GrpcStatic_1_28_x\Plugins\lndplugin\Source\lndplugin\bitcoind\data\regtest' -Recurse -ErrorAction SilentlyContinue

Remove-Item 'C:\Users\Andriy\Documents\Unreal_Projects\GrpcStatic_1_28_x\Plugins\lndplugin\Source\lndplugin\lnd\data\data_10009' -Recurse -ErrorAction SilentlyContinue
Remove-Item 'C:\Users\Andriy\Documents\Unreal_Projects\GrpcStatic_1_28_x\Plugins\lndplugin\Source\lndplugin\lnd\data\data_10010' -Recurse -ErrorAction SilentlyContinue

Start-Sleep -s 1
<#------------------------------------------#>

$bitcoind = Start-job {C:\Users\Andriy\Documents\Unreal_Projects\GrpcStatic_1_28_x\Plugins\lndplugin\Source\lndplugin\bitcoind\bin\bitcoind.exe --datadir=C:\Users\Andriy\Documents\Unreal_Projects\GrpcStatic_1_28_x\Plugins\lndplugin\Source\lndplugin\bitcoind\data}

Start-Sleep -s 5

$btc_newaddr_json = C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/libcurl-vc-x64-release-static-ipv6-sspi-winssl/bin/curl.exe -k -H "content-type: text/plain;" -u "ok:ok" -d '{\"jsonrpc\": \"1.0\", \"id\":\"curltest\", \"method\": \"getnewaddress\", \"params\": [\"p2sh-segwit\", \"p2sh-segwit\"] }' "http://127.0.0.1:18443/"

$btc_newaddr_obj = $btc_newaddr_json | ConvertFrom-Json

Write-Output "btc_newaddr:"
$btc_newaddr = $btc_newaddr_obj.result
Write-Output $btc_newaddr

Start-Sleep -s 5

<#------------------------------------#>

$lnd1 = Start-job {C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/lnd/bin/lnd.exe --debuglevel=debug --listen=localhost:9735 --rpclisten=localhost:10009 --restlisten=localhost:8080 --bitcoin.active --bitcoin.regtest --bitcoin.node=bitcoind --bitcoind.rpcuser=ok --bitcoind.rpcpass=ok --bitcoind.zmqpubrawblock=tcp://127.0.0.1:28332 --bitcoind.zmqpubrawtx=tcp://127.0.0.1:28333  --lnddir=C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/lnd/data/data_10009}

$lnd2 = Start-job {C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/lnd/bin/lnd.exe --debuglevel=debug --listen=localhost:9736 --rpclisten=localhost:10010 --restlisten=localhost:8081 --bitcoin.active --bitcoin.regtest --bitcoin.node=bitcoind --bitcoind.rpcuser=ok --bitcoind.rpcpass=ok --bitcoind.zmqpubrawblock=tcp://127.0.0.1:28332 --bitcoind.zmqpubrawtx=tcp://127.0.0.1:28333  --lnddir=C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/lnd/data/data_10010}

Start-Sleep -s 5

<#------------------------------------#>

Write-Output "Create wallet for node 1: "
Start-Process -Wait -FilePath C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/lnd/bin/lncli.exe -ArgumentList "--rpcserver=localhost:10009 --lnddir=C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/lnd/data/data_10009 --chain bitcoin --network regtest create"

Write-Output "Create wallet for node 2: "
Start-Process -Wait -FilePath C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/lnd/bin/lncli.exe -ArgumentList "--rpcserver=localhost:10010 --lnddir=C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/lnd/data/data_10010 --chain bitcoin --network regtest create"

Start-Sleep -s 20

<#------------------------------------#>

$lnd1_getinfo_json = C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/lnd/bin/lncli.exe --rpcserver=localhost:10009 --lnddir=C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/lnd/data/data_10009 --chain bitcoin --network regtest getinfo
$lnd1_getinfo_object = $lnd1_getinfo_json | ConvertFrom-Json
Write-Output "lnd1 identity_pubkey:"
$lnd1_identity_pubkey = $lnd1_getinfo_object.identity_pubkey
Write-Output $lnd1_identity_pubkey

$lnd2_getinfo_json = C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/lnd/bin/lncli.exe --rpcserver=localhost:10010 --lnddir=C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/lnd/data/data_10010 --chain bitcoin --network regtest getinfo
$lnd2_getinfo_object = $lnd2_getinfo_json | ConvertFrom-Json
Write-Output "lnd2 identity_pubkey:"
$lnd2_identity_pubkey = $lnd2_getinfo_object.identity_pubkey
Write-Output $lnd2_identity_pubkey

<#------------------------------------#>

$lnd1_newaddr_json = C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/lnd/bin/lncli.exe --rpcserver=localhost:10009 --lnddir=C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/lnd/data/data_10009 --chain bitcoin --network regtest newaddress p2wkh
$lnd1_newaddr_object = $lnd1_newaddr_json | ConvertFrom-Json
Write-Output "lnd1_newaddr:"
$lnd1_newaddr = $lnd1_newaddr_object.address
Write-Output $lnd1_newaddr

$lnd2_newaddr_json = C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/lnd/bin/lncli.exe --rpcserver=localhost:10010 --lnddir=C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/lnd/data/data_10010 --chain bitcoin --network regtest newaddress p2wkh
$lnd2_newaddr_object = $lnd2_newaddr_json | ConvertFrom-Json
Write-Output "lnd2_newaddr:"
$lnd2_newaddr = $lnd2_newaddr_object.address
Write-Output $lnd2_newaddr

<#------------------------------------#>

$btc_generatetoaddress_req = '{\"jsonrpc\": \"1.0\", \"id\":\"curltest\", \"method\": \"generatetoaddress\", \"params\": [510, ' + '\"' + $btc_newaddr + '\"] }'
C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/libcurl-vc-x64-release-static-ipv6-sspi-winssl/bin/curl.exe -k -H "content-type: text/plain;" -u "ok:ok" -d $btc_generatetoaddress_req   "http://127.0.0.1:18443/"
Start-Sleep -s 2

<#------------------------------------#>

$send_toaddr_str = '{\"jsonrpc\": \"1.0\", \"id\":\"curltest\", \"method\": \"sendtoaddress\", \"params\": [\"' + $lnd1_newaddr + '\", 1, \"donation1\", \"seans outpost1\" ] }'
C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/libcurl-vc-x64-release-static-ipv6-sspi-winssl/bin/curl.exe -k -H "content-type: text/plain;" -u "ok:ok" -d $send_toaddr_str "http://127.0.0.1:18443/"
Start-Sleep -s 2


$send_toaddr_str2 = '{\"jsonrpc\": \"1.0\", \"id\":\"curltest\", \"method\": \"sendtoaddress\", \"params\": [\"' + $lnd2_newaddr + '\", 1, \"donation2\", \"seans outpost2\" ] }'
C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/libcurl-vc-x64-release-static-ipv6-sspi-winssl/bin/curl.exe -k -H "content-type: text/plain;" -u "ok:ok" -d $send_toaddr_str2 "http://127.0.0.1:18443/"
Start-Sleep -s 2




$btc_generatetoaddress_req = '{\"jsonrpc\": \"1.0\", \"id\":\"curltest\", \"method\": \"generatetoaddress\", \"params\": [10, ' + '\"' + $btc_newaddr + '\"] }'
C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/libcurl-vc-x64-release-static-ipv6-sspi-winssl/bin/curl.exe -k -H "content-type: text/plain;" -u "ok:ok" -d $btc_generatetoaddress_req   "http://127.0.0.1:18443/"
Start-Sleep -s 4

<#------------------------------------#>

C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/lnd/bin/lncli.exe --rpcserver=localhost:10009 --lnddir=C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/lnd/data/data_10009 --chain bitcoin --network regtest walletbalance

C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/lnd/bin/lncli.exe --rpcserver=localhost:10010 --lnddir=C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/lnd/data/data_10010 --chain bitcoin --network regtest walletbalance

Start-Sleep -s 2
<#------------------------------------#>

$connect_str = $lnd2_identity_pubkey + "@localhost:9736"
Write-Output "connect_str:"
Write-Output $connect_str

C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/lnd/bin/lncli.exe --rpcserver=localhost:10009 --lnddir=C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/lnd/data/data_10009 --chain bitcoin --network regtest connect $connect_str 

Start-Sleep -s 5
<#------------------------------------#>

C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/lnd/bin/lncli.exe --rpcserver=localhost:10009 --lnddir=C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/lnd/data/data_10009 --chain bitcoin --network regtest listpeers 
Start-Sleep -s 2
<#------------------------------------#>

C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/lnd/bin/lncli.exe --rpcserver=localhost:10009 --lnddir=C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/lnd/data/data_10009 --chain bitcoin --network regtest openchannel --node_key $lnd2_identity_pubkey --local_amt 10000000

<#------------------------------------#>







<#------------------------------------#>

Write-Output "lnd shutdown"
$lnd1 | Stop-Job
$lnd2 | Stop-Job
Start-Sleep -s 2
$lnd1 | Remove-Job
$lnd2 | Remove-Job

Start-Sleep -s 2

Write-Output "bitcoind shutdown"
$bitcoind | Stop-Job
Start-Sleep -s 2
$bitcoind | Remove-Job


