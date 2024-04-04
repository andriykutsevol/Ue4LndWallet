Remove-Item 'C:\Users\Andriy\Documents\Unreal_Projects\GrpcStatic_1_28_x\Plugins\lndplugin\Source\lndplugin\lnd\bin\lnd.exe' -ErrorAction SilentlyContinue
Remove-Item 'C:\Users\Andriy\Documents\Unreal_Projects\GrpcStatic_1_28_x\Plugins\lndplugin\Source\lndplugin\lnd\bin\lncli.exe' -ErrorAction SilentlyContinue

Set-Location -Path C:\Users\Andriy\go\src\github.com\lightningnetwork\lnd\cmd\lncli
Start-Process -Wait powershell {go build ./}
Move-Item -Path .\lncli.exe -Destination C:\Users\Andriy\Documents\Unreal_Projects\GrpcStatic_1_28_x\Plugins\lndplugin\Source\lndplugin\lnd\bin\lncli.exe

Set-Location -Path C:\Users\Andriy\go\src\github.com\lightningnetwork\lnd\cmd\lnd
Start-Process -Wait powershell {go build ./}
Move-Item -Path .\lnd.exe -Destination C:\Users\Andriy\Documents\Unreal_Projects\GrpcStatic_1_28_x\Plugins\lndplugin\Source\lndplugin\lnd\bin\lnd.exe

Set-Location -Path C:\Users\Andriy\Documents\lnd_python_regtest\src