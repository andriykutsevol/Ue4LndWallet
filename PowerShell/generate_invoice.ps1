$lncli_exe = "C:/Users/Andriy/Documents/Unreal_Projects/LNDWallet/Plugins/LNDWalletPlugin/Source/LNDWalletPlugin/lnd/bin/lncli.exe"
$lnddir_10010 = "C:\Users\Andriy\Documents\lnd_python_regtest\src\lnd_data"


$pay_req_10010_json = cmd /c $lncli_exe --rpcserver=localhost:10010 --lnddir=$lnddir_10010 --chain bitcoin --network regtest addinvoice --amt=10000
$pay_req_10010_obj = $pay_req_10010_json | ConvertFrom-Json

$pay_req_10010 = $pay_req_10010_obj.payment_request

Write-Output "-------pay_req_10010--------:"
Write-Output $pay_req_10010