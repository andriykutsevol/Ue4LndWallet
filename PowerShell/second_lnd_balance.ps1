$lncli_exe = "C:/Users/Andriy/Documents/Unreal_Projects/LNDWallet/Plugins/LNDWalletPlugin/Source/LNDWalletPlugin/lnd/bin/lncli.exe"
$lnddir_10010 = "C:\Users\Andriy\Documents\lnd_python_regtest\src\lnd_data"


$chan_balance_10010_json = cmd /c $lncli_exe --rpcserver=localhost:10010 --lnddir=$lnddir_10010 --chain bitcoin --network regtest channelbalance
$chan_balance_10010_obj = $chan_balance_10010_json | ConvertFrom-Json

$chan_balance_10010 = $chan_balance_10010_obj.balance

Write-Output "-------chan_balance_10010--------:"
Write-Output $chan_balance_10010