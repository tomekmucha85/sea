## How to setup and run this project
* Download https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-2.0.4-VC.zip
* Download https://www.libsdl.org/release/SDL2-devel-2.0.9-VC.zip
* Download https://github.com/Emotiv/community-sdk/archive/master.zip
* Unzip all in C:\dev directory
* clone repo to C:\dev\sea directory
* Open .sln file in Visual Studio 2017
* Project -> Properties -> VC++ Directories
* Set *Include Directories* to:
```
C:\dev\sea;C:\dev\SDL2-2.0.9\include;C:\dev\SDL2_image-2.0.4\include;C:\dev\community-sdk-master\include;$(IncludePath)
```
* Set *Library Directories* to:
```
C:\dev\community-sdk-master\lib\win64;C:\dev\SDL2-2.0.9\lib\x64;C:\dev\SDL2_image-2.0.4\lib\x64;$(LibraryPath)
```
* Build and run project. It will fail.
* Copy C:\dev\sea\images folder with all it content to C:\dev\sea\x64\Debug
* Copy all .dll files from here:
	* C:\dev\SDL2-2.0.9\lib\x64
	* C:\dev\SDL2_image-2.0.4\lib\x64
	* C:\dev\community-sdk-master\bin\win64
* to here:
	* C:\dev\sea\x64\Debug

* Now project should build and run.