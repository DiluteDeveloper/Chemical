xcopy /s /y %~dp0bin\Chemical\Chemical.lib %~dp0Sandbox\
del /s /q %~dp0bin\Sandbox\res
md %~dp0bin\Sandbox\res
xcopy /s /y %~dp0Sandbox\res\ %~dp0bin\Sandbox\res\
PAUSE