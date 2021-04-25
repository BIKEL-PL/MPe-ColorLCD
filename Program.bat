@echo off
REM Program script for ST-LINK Utility CLI v.2.3

:PROGRAMING_DATA
"%ST-LINK_Utility%\ST-LINK_CLI.exe" -c swd -ME -p "%1%" 0x08000000 -rst
IF ERRORLEVEL 13 GOTO ERROR_13_HANDLER
IF ERRORLEVEL 12 GOTO ERROR_12_HANDLER
IF ERRORLEVEL 11 GOTO ERROR_11_HANDLER
IF ERRORLEVEL 10 GOTO ERROR_10_HANDLER
IF ERRORLEVEL 9 GOTO ERROR_9_HANDLER
IF ERRORLEVEL 8 GOTO ERROR_8_HANDLER
IF ERRORLEVEL 7 GOTO ERROR_7_HANDLER
IF ERRORLEVEL 6 GOTO ERROR_6_HANDLER
IF ERRORLEVEL 5 GOTO ERROR_5_HANDLER
IF ERRORLEVEL 4 GOTO ERROR_4_HANDLER
IF ERRORLEVEL 3 GOTO ERROR_3_HANDLER
IF ERRORLEVEL 2 GOTO ERROR_2_HANDLER
IF ERRORLEVEL 1 GOTO ERROR_1_HANDLER
IF ERRORLEVEL 0 GOTO EXIT

:ERROR_1_HANDLER
echo Command arguments error
goto PAUSE_AND_EXIT

:ERROR_2_HANDLER
echo Connexion problem
goto PAUSE_AND_EXIT

:ERROR_3_HANDLER
echo Command not available for the connected target
goto PAUSE_AND_EXIT

:ERROR_4_HANDLER
echo Error occurred while writing data to the specified memory address
goto PAUSE_AND_EXIT

:ERROR_5_HANDLER
echo Cannot read memory from the specified memory address
goto PAUSE_AND_EXIT

:ERROR_6_HANDLER
echo Cannot reset MCU
goto PAUSE_AND_EXIT

:ERROR_7_HANDLER
echo Failed to run application
goto PAUSE_AND_EXIT

:ERROR_8_HANDLER
echo Failed to halt the core
goto PAUSE_AND_EXIT

:ERROR_9_HANDLER
echo Failed to perform a single instruction step
goto PAUSE_AND_EXIT

:ERROR_10_HANDLER
echo Failed to set/clear a breakpoint
goto PAUSE_AND_EXIT

:ERROR_11_HANDLER
echo Unable to erase one or more flash sectors
goto PAUSE_AND_EXIT

:ERROR_12_HANDLER
echo Flash programming/verification error
goto PAUSE_AND_EXIT

:ERROR_13_HANDLER
echo Option bytes programming error
goto PAUSE_AND_EXIT

:PAUSE_AND_EXIT
pause
goto EXIT

:EXIT