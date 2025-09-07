import platform
import os
import requests
from requests import Response

TargetedVulkanSDKVersion: str = "1.4.321.0"
Platform: str = platform.system()
UserName: str = os.path.expanduser('~')
WindowsPathToVulkanSDK: str = f"C:/VulkanSDK/{TargetedVulkanSDKVersion}"
MacOSPathToVulkanSDK: str = f"/Users/{UserName}/VulkanSDK/{TargetedVulkanSDKVersion}"
WindowsVulkanSDKDownloadURL: str = f"https://sdk.lunarg.com/sdk/download/1.4.321.0/windows/vulkansdk-windows-X64-{TargetedVulkanSDKVersion}.exe"
MacOSVulkanSDKDownloadURL: str = f"https://sdk.lunarg.com/sdk/download/1.4.321.0/mac/vulkansdk-macos-{TargetedVulkanSDKVersion}.zip"

def CheckIfLibraryDependenciesPresent(vulkanLibraryDirectory: str, libraryDependencies: list[str]) -> bool:
    areAllLibraryDependenciesPresent = True
    for library in libraryDependencies:
        if not os.path.isfile(f"{vulkanLibraryDirectory}/{library}"):
            areAllLibraryDependenciesPresent = False
            print(f"{library} is not present but is needed!")

    if not areAllLibraryDependenciesPresent:
        print("Not all Vulkan SDK library dependencies are present in the current VulkanSDK directory!")
    return areAllLibraryDependenciesPresent

def CheckIfVulkanSDKExists() -> bool:

    if Platform == "Windows":
        if os.path.isdir(WindowsPathToVulkanSDK):

            # Check for needed libraries
            vulkanLibraryDirectory: str = f"{WindowsPathToVulkanSDK}/Lib"
            libraryDependencies: list[str] = ["vulkan-1.lib", "SPIRV-Toolsd.lib", "SPIRV-Tools-diffd.lib", "SPIRV-Tools-optd.lib", "glslangd.lib", "glslang-default-resource-limitsd.lib"]

            isLibraryDependenciesPresent: bool = CheckIfLibraryDependenciesPresent(vulkanLibraryDirectory=vulkanLibraryDirectory, libraryDependencies=libraryDependencies)
            return isLibraryDependenciesPresent
        else:
            print(f"Vulkan SDK target version {TargetedVulkanSDKVersion} was not found in the expected directory! ({WindowsPathToVulkanSDK})")
            return False


    elif Platform == "Darwin": # macOS
        if os.path.isdir(MacOSPathToVulkanSDK):
            # Check for needed libraries
            vulkanLibraryDirectory: str = f"{MacOSPathToVulkanSDK}/Lib"
            libraryDependencies: list[str] = ["libvulkan.1.dylib", "libSPIRV-Tools.a", "libSPIRV-Tools-diff.a", "libSPIRV-Tools-opt.a", "libglslang.a", "libglslang-default-resource-limits.a"]

            isLibraryDependenciesPresent: bool = CheckIfLibraryDependenciesPresent(vulkanLibraryDirectory=vulkanLibraryDirectory, libraryDependencies=libraryDependencies)
            return isLibraryDependenciesPresent
        else:
            return False

    else:
        print("Platform not supported!")
        exit()

def DownloadVulkanSDK():
    PathToInstaller: str = ""
    VulkanSDKDownloadURL: str = ""
    response: Response

    if Platform == "Windows":
        PathToInstaller = f"{WindowsPathToVulkanSDK}/vulkansdk-windows-X64-{TargetedVulkanSDKVersion}.exe"
        VulkanSDKDownloadURL = f"{WindowsVulkanSDKDownloadURL}"

    elif Platform == "Darwin": # macOS
        PathToInstaller = f"{MacOSPathToVulkanSDK}/vulkansdk-macos-{TargetedVulkanSDKVersion}.zip"
        VulkanSDKDownloadURL = f"{MacOSVulkanSDKDownloadURL}"
    else:
        print("Platform not supported!")
        exit()

    if os.path.isfile(PathToInstaller):
        print("Installer is already downloaded! \nRun the installer to set the Vulkan SDK up for use by Astral Engine!\n"
              f"The installer file path location is {PathToInstaller}")
        exit()

    response = requests.get(f"{VulkanSDKDownloadURL}", stream=True)

    if response.status_code != 200:
        print(f"Failed to download the Vulkan SDK from the LunarG website! GET error code: {response.status_code}")
        exit()

    os.makedirs(f"{PathToInstaller}", exist_ok=True)
    with open(PathToInstaller, "wb") as file:
        for chunk in response.iter_content(chunk_size=8192):
            file.write(chunk)
    print(f"Downloaded the Vulkan SDK version {TargetedVulkanSDKVersion} successfully! \nRun the installer to set the Vulkan SDK up for use by Astral Engine!\n"
          f"The installer file path location is {PathToInstaller}")

if __name__ == "__main__":
    isVulkanSDKPresent: bool = CheckIfVulkanSDKExists()

    if not isVulkanSDKPresent:
        print("Downloading the Vulkan SDK!")
        DownloadVulkanSDK()
    else:
        print("Vulkan SDK and library dependencies successfully found!")