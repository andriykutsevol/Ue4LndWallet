# Logging level for all subsystems {trace, debug, info, warn, error, critical}

$lnd2 = Start-Process powershell {C:/Users/Andriy/Documents/Unreal_Projects/LNDWallet/Plugins/LNDWalletPlugin/Source/LNDWalletPlugin/lnd/bin/lnd.exe --lnddir=C:/Users/Andriy/Documents/lnd_python_regtest/src/lnd_data --debuglevel=debug --listen=localhost:9736 --rpclisten=localhost:10010 --restlisten=localhost:8081 --bitcoin.active --bitcoin.regtest --bitcoin.node=bitcoind --bitcoind.rpcuser=ok --bitcoind.rpcpass=ok --bitcoind.zmqpubrawblock=tcp://127.0.0.1:28332 --bitcoind.zmqpubrawtx=tcp://127.0.0.1:28333}

Start-Sleep -s 5

<#------------------------------------#>

$lncli_path = "C:/Users/Andriy/Documents/Unreal_Projects/LNDWallet/Plugins/LNDWalletPlugin/Source/LNDWalletPlugin/lnd/bin/"


$lncli_exe = "C:/Users/Andriy/Documents/Unreal_Projects/LNDWallet/Plugins/LNDWalletPlugin/Source/LNDWalletPlugin/lnd/bin/lncli.exe"
$lnddir_10010 = "C:\Users\Andriy\Documents\lnd_python_regtest\src\lnd_data"

Write-Output "Unlock wallet for node 2: "
Start-Process -Wait -FilePath $lncli_path/lncli.exe -ArgumentList "--rpcserver=localhost:10010 --lnddir=C:\Users\Andriy\Documents\lnd_python_regtest\src\lnd_data --chain bitcoin --network regtest unlock"

Write-Output "Waiting after wallet unlocking 5s"
Start-Sleep -s 5

$lnd2_getinfo_json = cmd /c $lncli_exe --rpcserver=localhost:10010 --lnddir=$lnddir_10010 --chain bitcoin --network regtest getinfo
Start-Sleep -s 5
$lnd2_getinfo_object = $lnd2_getinfo_json | ConvertFrom-Json
Write-Output "lnd2 identity_pubkey:"
$lnd2_identity_pubkey = $lnd2_getinfo_object.identity_pubkey
Write-Output $lnd2_identity_pubkey
