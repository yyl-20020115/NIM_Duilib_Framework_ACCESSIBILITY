# 设置名字列表文件的路径
$namesFilePath = "f.txt"
 
# 设置输出文件的目录路径
$outputDirectory = "."
 
# 确保输出目录存在
if (-not (Test-Path $outputDirectory)) {
    New-Item -ItemType Directory -Path $outputDirectory
}
 
# 读取名字列表文件
$names = Get-Content $namesFilePath
 
# 为每个名字创建一个新文件
foreach ($name in $names) {
    $fileName = "$outputDirectory\$name"
    Set-Content -Path $fileName -Value $fileContent
}