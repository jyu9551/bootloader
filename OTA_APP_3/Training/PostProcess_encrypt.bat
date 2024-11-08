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
If exist signature_temp.bin (
	del signature_temp.bin
)
If exist signature_temp.hex (
	del signature_temp.hex
)

set CMD_APP_SX=%HEXVIEW_PATH% %FILE_NAME_APP%.sre /s /FP:%FILL_PATTERN% /FR:!APP_ADDR_START!-!APP_ADDR_END! /XS:32 -o %FILE_NAME_APP%.sx
!CMD_APP_SX!

set CMD_APP_BIN=%HEXVIEW_PATH% %FILE_NAME_APP%.sx /s /XN -o %FILE_NAME_APP%.bin
!CMD_APP_BIN!

set "psScript=PostProcess_encrypt.ps1"
powershell -ExecutionPolicy Bypass -File "%psScript%"

copy /b %FILE_NAME_APP%_key_0x11.bin + %FILE_NAME_APP%.bin %FILE_NAME_APP%_temp.bin

:: 명령어 실행 및 결과 가져오기
for /f "tokens=*" %%i in ('CertUtil -hashfile %FILE_NAME_APP%_temp.bin SHA256') do (
    set /a count+=1
    set "line[!count!]=%%i"
)
:: 3번째 행 출력
set signature=!line[2]!
echo %signature%

echo %signature% > signature_temp.hex
certutil -decodehex signature_temp.hex signature_temp.bin

copy /b %FILE_NAME_APP%_encrypt.bin + signature_temp.bin %FILE_NAME_APP%_encrypt_key.bin

del %FILE_NAME_APP%_temp.bin
del signature_temp.hex
del signature_temp.bin


::echo %signature% | certutil -decodehex - %FILE_NAME_APP%_encrypt_key.bin

::echo !signature!

::set CMD_APP_SX_2=%HEXVIEW_PATH% %FILE_NAME_APP%_encrypt.bin /s /remap:0x00000000-0x7FFBF,!APP_ADDR_START!,0x7FFC0,!APP_ADDR_START! /XS:32 -o %FILE_NAME_APP%_encrypt_temp.sx
::!CMD_APP_SX_2!
::echo !CMD_APP_SX_2!
::echo aaaaa

::set CMD_ENCRYPT_SIGNATURE=%HEXVIEW_PATH% %FILE_NAME_APP%_encrypt_temp.sx /s /FR:!APP_ADDR_SIGN!,32 /FP:%signature% /XS:32 -o %FILE_NAME_APP%_encrypt_key_sign.sx
::!CMD_ENCRYPT_SIGNATURE!
::echo bbb
::set CMD_APP_BIN_ENCRYPT_SIGN=%HEXVIEW_PATH% %FILE_NAME_APP%_encrypt_key_sign.sx /s /XN -o %FILE_NAME_APP%_encrypt_key_sign.bin
::!CMD_APP_BIN_ENCRYPT_SIGN!
::echo ccc

::del %FILE_NAME_APP%_encrypt_temp.sx
::del %FILE_NAME_APP%_encrypt_key_temp.bin

pause