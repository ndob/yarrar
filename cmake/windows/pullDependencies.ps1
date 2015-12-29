Set-Variable ReplaceFilesWithoutAsking -option Constant -value 0x14

$workingDirectory = (Get-Location).Path
$externalDirectory = $workingDirectory + "\external\"

# Grabbed from http://blogs.msdn.com/b/jasonn/archive/2013/06/11/8594493.aspx
function Download-File($url, $targetFile)
{
    "Downloading $url"
    $uri = New-Object "System.Uri" "$url"
    $request = [System.Net.HttpWebRequest]::Create($uri)
    $request.set_Timeout(15000) #15 second timeout
    $response = $request.GetResponse()
    $totalLength = [System.Math]::Floor($response.get_ContentLength()/1024)
    $responseStream = $response.GetResponseStream()
    $targetStream = New-Object -TypeName System.IO.FileStream -ArgumentList $targetFile, Create
    $buffer = new-object byte[] 10KB
    $count = $responseStream.Read($buffer,0,$buffer.length)
    $downloadedBytes = $count
    while ($count -gt 0)
    {
        [System.Console]::Write("Downloaded {0}K of {1}K", [System.Math]::Floor($downloadedBytes/1024), $totalLength)
        $targetStream.Write($buffer, 0, $count)
        $count = $responseStream.Read($buffer,0,$buffer.length)
        $downloadedBytes = $downloadedBytes + $count
    }
    "`nFinished Download"
    $targetStream.Flush()
    $targetStream.Close()
    $targetStream.Dispose()
    $responseStream.Dispose()
} 


function Download-Dependency($url, $output)
{
    $outputPath = $externalDirectory + $output

    if (Test-Path $outputPath)
    {
        Write-Host File already downloaded. Using cached: $output
    }
    else
    {
        Download-File $url $outputPath
        Write-Host Downloaded $output
    }
}

function Prepare-Zipped-Dependency($name, $zippedFile)
{
    $input = $externalDirectory + $zippedFile
    $destination = $externalDirectory + $name
    $tmpExtractPath = $externalDirectory + "tmp_" + $name
    $shell = New-Object -com Shell.Application
    $zip = $shell.Namespace($input)

    if (-Not(Test-Path $tmpExtractPath))
    {
        mkdir $tmpExtractPath
    }

    foreach($item in $zip.items())
    {
        $shell.Namespace($tmpExtractPath).CopyHere($item, $ReplaceFilesWithoutAsking)
    }

    if (Test-Path $destination)
    {
       Write-Host File already extracted. Using cached: $destination
       return
    }

    # If there is only one directory that has been extracted -> remove the excess subdir
    $subdir = Get-ChildItem $tmpExtractPath

    # $subdir is DirectoryInfo only, if it's the only file.
    # If there are more files $subdir is a list of those files.
    if($subdir -is [System.IO.DirectoryInfo])
    {
        Move-Item -Path $subdir.FullName -Destination $destination
        Remove-Item $tmpExtractPath -Recurse
    }
    else
    {
        Move-Item -Path $tmpExtractPath -Destination $destination
    }
}

function Download-And-Prepare-Zipped-Dependency($url, $name)
{
    $outputFilename = "_tmp_download_" + $name + ".zip"
    Download-Dependency -Url $url -Output $outputFilename
    Prepare-Zipped-Dependency -Name $name -ZippedFile $outputFilename
}

function Download-And-Prepare-Self-Extracting-Dependency($url, $name)
{
    $outputFilename = "_tmp_download_" + $name + ".exe"
    Download-Dependency -Url $url -Output $outputFilename

    $destination = $externalDirectory + $name
    if (Test-Path $destination)
    {
       Write-Host File already extracted. Using cached: $destination
       return
    }

    $command = $externalDirectory + $outputFilename + " -y -gm2"
    Invoke-Expression $command 
}

Write-Host Preparing dependencies...

if (-Not(Test-Path $externalDirectory))
{
    mkdir $externalDirectory
}

Download-And-Prepare-Zipped-Dependency -Url "http://netix.dl.sourceforge.net/project/glew/glew/1.13.0/glew-1.13.0-win32.zip" -Name "glew"
Download-And-Prepare-Zipped-Dependency -Url "https://github.com/glfw/glfw/releases/download/3.1.2/glfw-3.1.2.bin.WIN64.zip" -Name "glfw"
Download-And-Prepare-Self-Extracting-Dependency -Url "http://netassist.dl.sourceforge.net/project/opencvlibrary/opencv-win/3.1.0/opencv-3.1.0.exe" -Name "opencv"

Write-Host Dependencies prepared.
