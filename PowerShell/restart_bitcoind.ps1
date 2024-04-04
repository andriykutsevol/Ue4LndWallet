Remove-Item 'C:\Users\Andriy\Documents\Unreal_Projects\GrpcStatic_1_28_x\Plugins\lndplugin\Source\lndplugin\bitcoind\data\regtest' -Recurse -ErrorAction SilentlyContinue
Start-Sleep -s 1

$bitcoind = Start-Process powershell {C:\Users\Andriy\Documents\Unreal_Projects\GrpcStatic_1_28_x\Plugins\lndplugin\Source\lndplugin\bitcoind\bin\bitcoind.exe --datadir=C:\Users\Andriy\Documents\Unreal_Projects\GrpcStatic_1_28_x\Plugins\lndplugin\Source\lndplugin\bitcoind\data}


Start-Sleep -s 5


$curl_exe = "C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/libcurl-vc-x64-release-static-ipv6-sspi-winssl/bin/curl.exe"


$btc_newaddr_json = cmd /c $curl_exe -k -H "content-type: text/plain;" -u "ok:ok" -d '{\"jsonrpc\": \"1.0\", \"id\":\"curltest\", \"method\": \"getnewaddress\", \"params\": [\"p2sh-segwit\", \"p2sh-segwit\"] }' "http://127.0.0.1:18443/"

$btc_newaddr_obj = $btc_newaddr_json | ConvertFrom-Json

Write-Output "btc_newaddr:"
$btc_newaddr = $btc_newaddr_obj.result
Write-Output $btc_newaddr

Start-Sleep -s 5

$btc_generatetoaddress_req = '{\"jsonrpc\": \"1.0\", \"id\":\"curltest\", \"method\": \"generatetoaddress\", \"params\": [501, ' + '\"' + $btc_newaddr + '\"] }'
cmd /c $curl_exe -k -H "content-type: text/plain;" -u "ok:ok" -d $btc_generatetoaddress_req "http://127.0.0.1:18443/"

Write-Output  "Waiting after generate 501"
Start-Sleep -s 20
Write-Output  "Waiting complete"
