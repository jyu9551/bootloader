@echo off

setlocal enabledelayedexpansion

set FILL_PATTERN=0x00
set HEXVIEW_PATH=..\Hexview\hexview.exe
set FILE_NAME_APP=SCU_Gen6_Training

set APP_ADDR_START=0x80100000
set APP_ADDR_END=0x8017FFBF
set APP_ADDR_SIGN=0x8017FFC0

If exist %FILE_NAME_APP%.sx (
	del %FILE_NAME_APP%.sx
)
If exist %FILE_NAME_APP%.bin (
	del %FILE_NAME_APP%.bin
)

set CMD_APP_SRE=%HEXVIEW_PATH% SCU_Gen6.hex /s /XS:32 -o %FILE_NAME_APP%.sre
!CMD_APP_SRE!

set CMD_APP_SX=%HEXVIEW_PATH% %FILE_NAME_APP%.sre /s /FP:%FILL_PATTERN% /FR:!APP_ADDR_START!-!APP_ADDR_END! /XS:32 -o %FILE_NAME_APP%.sx
!CMD_APP_SX!

set CMD_APP_BIN=%HEXVIEW_PATH% %FILE_NAME_APP%.sx /s /XN -o %FILE_NAME_APP%.bin
!CMD_APP_BIN!

:: 명령어 실행 및 결과 가져오기
for /f "tokens=*" %%i in ('CertUtil -hashfile %FILE_NAME_APP%.bin SHA256') do (
    set /a count+=1
    set "line[!count!]=%%i"
)
:: 3번째 행 출력
set signature=!line[2]!

set CMD_APP_SX_2=%HEXVIEW_PATH% %FILE_NAME_APP%.bin /s /remap:0x00000000-0x7FFBF,!APP_ADDR_START!,0x7FFC0,!APP_ADDR_START! /XS:32 -o %FILE_NAME_APP%_temp.sx
!CMD_APP_SX_2!
set CMD_SIGNATURE=%HEXVIEW_PATH% %FILE_NAME_APP%_temp.sx /s /FR:!APP_ADDR_SIGN!,32 /FP:%signature% /XS:32 -o %FILE_NAME_APP%_sign.sx
!CMD_SIGNATURE!
set CMD_APP_BIN_SIGN=%HEXVIEW_PATH% %FILE_NAME_APP%_sign.sx /s /XN -o %FILE_NAME_APP%_sign.bin
!CMD_APP_BIN_SIGN!

del %FILE_NAME_APP%_temp.sx

pause
