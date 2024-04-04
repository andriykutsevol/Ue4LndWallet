$lnd1_newaddr = "bcrt1qam8r8yk5na26fm9vzve8hm5euarnkv23qjvr2c"
$send_toaddr_str = '{\"jsonrpc\": \"1.0\", \"id\":\"curltest\", \"method\": \"sendtoaddress\", \"params\": [\"' + $lnd1_newaddr + '\", 10, \"donation1\", \"seans outpost1\" ] }'
C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/libcurl-vc-x64-release-static-ipv6-sspi-winssl/bin/curl.exe -k -H "content-type: text/plain;" -u "ok:ok" -d $send_toaddr_str "http://127.0.0.1:18443/"
Start-Sleep -s 2
