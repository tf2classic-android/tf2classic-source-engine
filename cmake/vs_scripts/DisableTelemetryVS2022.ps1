#Requires -RunAsAdministrator

# Disable telemetry in Visual Studio 2022 - https://learn.microsoft.com/en-us/visualstudio/ide/visual-studio-experience-improvement-program?view=vs-2022
New-ItemProperty -Path "HKLM:\SOFTWARE\Wow6432Node\Microsoft\VSCommon\17.0\SQM" -Name "OptIn" -Value "0" -PropertyType "DWORD" -Force

# Delete telemetry directories
Remove-Item -Path "$env:APPDATA\vstelemetry" -Recurse -Force -ErrorAction SilentlyContinue
Remove-Item -Path "$env:LOCALAPPDATA\Microsoft\VSApplicationInsights" -Recurse -Force -ErrorAction SilentlyContinue
Remove-Item -Path "$env:PROGRAMDATA\Microsoft\VSApplicationInsights" -Recurse -Force -ErrorAction SilentlyContinue
Remove-Item -Path "$env:TEMP\Microsoft\VSApplicationInsights" -Recurse -Force -ErrorAction SilentlyContinue
Remove-Item -Path "$env:TEMP\VSFaultInfo" -Recurse -Force -ErrorAction SilentlyContinue
Remove-Item -Path "$env:TEMP\VSFeedbackIntelliCodeLogs" -Recurse -Force -ErrorAction SilentlyContinue
Remove-Item -Path "$env:TEMP\VSFeedbackPerfWatsonData" -Recurse -Force -ErrorAction SilentlyContinue
Remove-Item -Path "$env:TEMP\VSFeedbackVSRTCLogs" -Recurse -Force -ErrorAction SilentlyContinue
Remove-Item -Path "$env:TEMP\VSRemoteControl" -Recurse -Force -ErrorAction SilentlyContinue
Remove-Item -Path "$env:TEMP\VSTelem" -Recurse -Force -ErrorAction SilentlyContinue
Remove-Item -Path "$env:TEMP\VSTelem.Out" -Recurse -Force -ErrorAction SilentlyContinue

# Disable telemetry in .NET
[Environment]::SetEnvironmentVariable('DOTNET_CLI_TELEMETRY_OPTOUT', '1', 'User')

# Disable telemetry in NuGet
[Environment]::SetEnvironmentVariable('NUGET_TELEMETRY_OPTOUT', 'true', 'User')

# Disable Telemetry in Visual Studio - Not documented, not proofen to do anything, copied over from https://gist.github.com/stesee/b43e264a79912fe4640fdd85d492e104

# Define the registry keys
$VS_POLICIES_KEY = "HKLM:\SOFTWARE\Policies\Microsoft\VisualStudio"
$VS_POLICIES_FEEDBACK_KEY = "$VS_POLICIES_KEY\Feedback"
$VS_POLICIES_SQM_KEY = "$VS_POLICIES_KEY\SQM"
$VS_TELEMETRY_KEY = "HKCU:\Software\Microsoft\VisualStudio\Telemetry"

# Disable feedback in Visual Studio
New-ItemProperty -Path $VS_POLICIES_FEEDBACK_KEY -Name "DisableFeedbackDialog" -Value 1 -PropertyType "DWORD" -Force
New-ItemProperty -Path $VS_POLICIES_FEEDBACK_KEY -Name "DisableEmailInput" -Value 1 -PropertyType "DWORD" -Force
New-ItemProperty -Path $VS_POLICIES_FEEDBACK_KEY -Name "DisableScreenshotCapture" -Value 1 -PropertyType "DWORD" -Force

# Disable PerfWatson
New-ItemProperty -Path $VS_POLICIES_SQM_KEY -Name "OptIn" -Value 0 -PropertyType "DWORD" -Force

# Disable telemetry
New-ItemProperty -Path $VS_TELEMETRY_KEY -Name "TurnOffSwitch" -Value 1 -PropertyType "DWORD" -Force