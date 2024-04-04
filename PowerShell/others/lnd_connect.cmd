@ECHO OFF 

rem ==============================

rmdir /s /q "C:\Users\Andriy\Documents\Unreal_Projects\GrpcStatic_1_28_x\Plugins\lndplugin\Source\lndplugin\bitcoind\data\regtest"

rmdir /s /q "C:\Users\Andriy\Documents\Unreal_Projects\GrpcStatic_1_28_x\Plugins\lndplugin\Source\lndplugin\lnd\data\data_10009"
rmdir /s /q "C:\Users\Andriy\Documents\Unreal_Projects\GrpcStatic_1_28_x\Plugins\lndplugin\Source\lndplugin\lnd\data\data_10010"


rem ==============================

timeout /t 1

rem ==============================

START /B C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/bitcoind/bin/bitcoind.exe --datadir=C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/bitcoind/data > ./logs/bitcoind.txt

timeout /t 3

C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/libcurl-vc-x64-release-static-ipv6-sspi-winssl/bin/curl.exe -k -H "content-type: text/plain;" -u "ok:ok" -d "{\"jsonrpc\": \"1.0\", \"id\":\"curltest\", \"method\": \"getnewaddress\", \"params\": [\"p2sh-segwit\", \"p2sh-segwit\"] }" "http://127.0.0.1:18443/" > ./logs/bitcoind_newaddr.txt

set /p string=<./logs/bitcoind_newaddr.txt

rem Remove quotes
set string=%string:"=%
rem Remove braces
set "string=%string:~1,-1%"
rem Change colon by equal-sign
set "string=%string::==%"
rem Separate parts at comma into individual assignments
set "%string:,=" & set "%"
echo "New bitcoind address: " %result%


<#=================================#>

START /B C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/lnd/bin/lnd.exe --debuglevel=trace --listen=localhost:9735 --rpclisten=localhost:10009 --restlisten=localhost:8080 --bitcoin.active --bitcoin.regtest --bitcoin.node=bitcoind --bitcoind.rpcuser=ok --bitcoind.rpcpass=ok --bitcoind.zmqpubrawblock=tcp://127.0.0.1:28332 --bitcoind.zmqpubrawtx=tcp://127.0.0.1:28333  --lnddir=C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/lnd/data/data_10009 > lnd_10009.txt

timeout /t 5

START /B C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/lnd/bin/lnd.exe --debuglevel=trace --listen=localhost:9736 --rpclisten=localhost:10010 --restlisten=localhost:8081 --bitcoin.active --bitcoin.regtest --bitcoin.node=bitcoind --bitcoind.rpcuser=ok --bitcoind.rpcpass=ok --bitcoind.zmqpubrawblock=tcp://127.0.0.1:28332 --bitcoind.zmqpubrawtx=tcp://127.0.0.1:28333  --lnddir=C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/lnd/data/data_10010 > lnd_10010.txt

timeout /t 5

echo "Create wallet for node 1: "
rem C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/lnd/bin/lncli.exe --rpcserver=localhost:10009 --macaroonpath=C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/lnd/data/data_10009/chain/bitcoin/testnet/admin.macaroon --chain bitcoin --network regtest create
C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/lnd/bin/lncli.exe --rpcserver=localhost:10009 --lnddir=C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/lnd/data/data_10009 --chain bitcoin --network regtest create

echo "Create wallet for node 2: "
rem C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/lnd/bin/lncli.exe --rpcserver=localhost:10010 --macaroonpath=C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/lnd/data/data_10010/chain/bitcoin/testnet/admin.macaroon --chain bitcoin --network regtest create
C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/lnd/bin/lncli.exe --rpcserver=localhost:10010 --lnddir=C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/lnd/data/data_10010 --chain bitcoin --network regtest create

timeout /t 10

rem C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/lnd/bin/lncli.exe --rpcserver=localhost:10010 --lnddir=C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/lnd/data/data_10010 --chain bitcoin --network regtest getinfo > ./logs/node1_getinfo.txt

$JSON = C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/lnd/bin/lncli.exe --rpcserver=localhost:10010 --lnddir=C:/Users/Andriy/Documents/Unreal_Projects/GrpcStatic_1_28_x/Plugins/lndplugin/Source/lndplugin/lnd/data/data_10010 --chain bitcoin --network regtest getinfo

timeout /t 10

rem ===============================
taskkill /IM "lnd.exe" /F
timeout /t 2
taskkill /IM "bitcoind.exe" /F