$curl_exe = "C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/libcurl-vc-x64-release-static-ipv6-sspi-winssl/bin/curl.exe"


$btc_newaddr = "2N7r8yMwh3n8m6tafh6nt4qfoeyZyYFyrMd"

$btc_generatetoaddress_req = '{\"jsonrpc\": \"1.0\", \"id\":\"curltest\", \"method\": \"generatetoaddress\", \"params\": [10, ' + '\"' + $btc_newaddr + '\"] }'
cmd /c $curl_exe -k -H "content-type: text/plain;" -u "ok:ok" -d $btc_generatetoaddress_req "http://127.0.0.1:18443/"

Write-Output  "Waiting after generate 1"
Start-Sleep -s 5
Write-Output  "Waiting complete"