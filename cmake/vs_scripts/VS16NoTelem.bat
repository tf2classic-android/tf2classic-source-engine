@echo off

fltmc >nul 2>&1 || (
    echo This batch script requires administrator privileges. Right-click on
    echo the script and select "Run as administrator".
    goto :die
)

rem Change this path if you are using Community or Professional editions
set "VS_INSTALL_DIR=%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Enterprise"
if not defined ProgramFiles(x86) (
    set "VS_INSTALL_DIR=%ProgramFiles%\Microsoft Visual Studio\2019\Enterprise"
)

set "VS_POLICIES_KEY=HKEY_LOCAL_MACHINE\SOFTWARE\Policies\Microsoft\VisualStudio"
set "VS_POLICIES_FEEDBACK_KEY=%VS_POLICIES_KEY%\Feedback"
set "VS_POLICIES_SQM_KEY=%VS_POLICIES_KEY%\SQM"
set "VS_TELEMETRY_KEY=HKEY_CURRENT_USER\Software\Microsoft\VisualStudio\Telemetry"

rem Disable feedback in Visual Studio
reg add "%VS_POLICIES_FEEDBACK_KEY%" /v DisableFeedbackDialog /t REG_DWORD /d 1 /f
reg add "%VS_POLICIES_FEEDBACK_KEY%" /v DisableEmailInput /t REG_DWORD /d 1 /f
reg add "%VS_POLICIES_FEEDBACK_KEY%" /v DisableScreenshotCapture /t REG_DWORD /d 1 /f

rem Disable PerfWatson
reg add "%VS_POLICIES_SQM_KEY%" /v OptIn /t REG_DWORD /d 0 /f

rem Disable telemetry
reg add "%VS_TELEMETRY_KEY%" /v TurnOffSwitch /t REG_DWORD /d 1 /f

rem Also considering adding these hostnames to your C:\Windows\system32\drivers\etc\hosts
rem - vortex.data.microsoft.com
rem - dc.services.visualstudio.com
rem - visualstudio-devdiv-c2s.msedge.net
rem - az667904.vo.msecnd.net
rem - az700632.vo.msecnd.net
rem - sxpdata.microsoft.com
rem - sxp.microsoft.com

rem Delete telemetry directories
rmdir /s /q "%AppData%\vstelemetry" 2>nul
rmdir /s /q "%LocalAppData%\Microsoft\VSApplicationInsights" 2>nul
rmdir /s /q "%ProgramData%\Microsoft\VSApplicationInsights" 2>nul
rmdir /s /q "%Temp%\Microsoft\VSApplicationInsights" 2>nul
rmdir /s /q "%Temp%\VSFaultInfo" 2>nul
rmdir /s /q "%Temp%\VSFeedbackIntelliCodeLogs" 2>nul
rmdir /s /q "%Temp%\VSFeedbackPerfWatsonData" 2>nul
rmdir /s /q "%Temp%\VSFeedbackVSRTCLogs" 2>nul
rmdir /s /q "%Temp%\VSRemoteControl" 2>nul
rmdir /s /q "%Temp%\VSTelem" 2>nul
rmdir /s /q "%Temp%\VSTelem.Out" 2>nul

rem For a noticable improvement in responsiveness, I also recommend disabling any
rem of the default extensions you don't actually use, for example I disabled:
rem - VS Live Share
rem - Dotnet Extensions for Test Explorer
rem - Microsoft Studio Test Platform
rem - Test Adapter for Google Tests
rem - Test Adapter for Boost.Test

:die
echo.
pause
exit
