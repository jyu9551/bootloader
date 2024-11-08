# modify.ps1
$InputFile = 'SCU_Gen6_Training.bin'
$OutputFile = 'SCU_Gen6_Training_encrypt.bin'

$bytes = Get-Content -Path $InputFile -Encoding Byte
foreach ($i in 0..($bytes.Length - 1)) {
    $bytes[$i] = [byte](($bytes[$i] + 5) % 256)
}
Set-Content -Path $OutputFile -Value $bytes -Encoding Byte