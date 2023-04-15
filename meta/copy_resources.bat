:: Argument 1 is the directory of the resource folder to be copied
:: Argument 2 is the directory to paste the resource folder
:: Both arguments are relative to the project root.
md %2
xcopy /s /y %1\ %2\