# Arsenal [download](https://github.com/Viceroyy/Arsenal/releases/download/v1.0.0/Arsenal-Release.dll)

Free open-source Windows cheat software for **Counter-Strike Source** game. Designed as an internal cheat - [Dynamic-link library](https://en.wikipedia.org/wiki/Dynamic-link_library) (DLL) loadable into game process. Compatible with the Steam version of the game.

## Features
*   **ESP** - show information about players and dropped weapons
*   **Visuals** - miscellaneous visual options
*   **Misc** - miscellaneous features
*   **Config** - JSON-based configuration system

<details>

*   **Exploits** - things not possible when legit
    *   **No recoil** - remove recoil punch effect
    *   **No spread** - remove random bullet spread
      
*   **ESP** - show additional information about players and game world
    1.  *Teammates, Enemies*

    2.  *Planted C4*

    *   **Enabled** - on / off master switch
    *   **Box** - draw 2D box over player model
    *   **Name** - draw player name
    *   **Health** - draw player health
    *   **Health bar** - draw rectangle indicating player health
    *   **Armor** - draw player armor
    *   **Armor bar** - draw rectangle indicating player armor
    *   **Money** - draw player money
    *   **Weapon** - draw player equipped weapon
    
*   **Visuals** - miscellaneous visual options
    *   **No visual recoil** - remove visual recoil punch effect
    *   **Viewmodel FOV** - change view model FOV \[*70*-*120*\] (70 - default viewmodel, higher values - further away viewmodel)
    *   **FOV** - change view FOV \[*90*-*120*\] (90 - default fov, higher values - further away fov)

*   **Misc** - miscellaneous features
    *   **Auto strafe** - automatically strafe in air following mouse movement
    *   **Bunny hop** - automatically simulate space bar press / release while jump button is being held; increases movement speed

*   **Config** - JSON-based configuration system
    *   **Create config** - create new configuration file
    *   **Load** - load selected configuration file
    *   **Save** - save selected configuration file
    *   **Delete** - delete selected configuration file
</details>

## Getting started

### Prerequisites
Microsoft Visual Studio 2022 17.10.3 (or newer), platform toolset v143 and Windows SDK 10.0 are required in order to compile Arsenal. You can download VS [here](https://visualstudio.microsoft.com/) (Windows SDK is installed during Visual Studio Setup).

### Downloading

There are two options of downloading the source code:

#### Without [git](https://git-scm.com)

Choose this option if you want pure source and you're not going to contribute to the repo. Download size ~600 kB.

To download source code this way [click here](https://github.com/Viceroyy/Arsenal/archive/main.zip).

#### With [git](https://git-scm.com)

Choose this option if you're going to contribute to the repo or you want to use version control system. Download size ~4 MB. Git is required to step further, if not installed download it [here](https://git-scm.com).

Open git command prompt and enter following command:

    git clone --depth=1 https://github.com/Viceroyy/Arsenal.git

`Arsenal` folder should have been successfully created, containing all the source files.

### Compiling from source

<details>

When you have equipped a copy of the source code, next step is opening **Arsenal.sln** in Microsoft Visual Studio 2022.

Then change build configuration to `Release | x86` and simply press **Build solution**.

If everything went right you should receive `Arsenal-Release.dll`  binary file.

</details>

### Loading / Injecting into game process

<details>

Open your favorite [DLL injector](https://en.wikipedia.org/wiki/DLL_injection) and just inject `Arsenal-Release.dll` into `hl2.exe` process.

When injected, menu is openable under `INSERT` key.

</details>

### Further optimizations
If your CPU doesn't support the AVX2 instruction set, you can disable it in project settings. Currently AVX2 instructions are selected in project settings.

## FAQ

### How do I open menu?
Press <kbd>INSERT</kbd> while focused on CSS window.

### Where is my config file saved?
Configuration files are saved inside `Arsenal` folder in your game folder (`C:\Program Files (x86)\Steam\Steamapps\Common\Counter-Strike Source\Arsenal`). The config is in human readable format and can be edited (e.g, using notepad). Sometimes after updates configuration file needs to be deleted and recreated.

## Acknowledgments

*   [spook953](https://github.com/spook953)
*   [Lak3](https://github.com/Lak3)
*   [whydoIhax/usernameunavalible0](https://github.com/usernameunavalible0)
*   and others..

## See also
*   [css-internal-base](https://github.com/usernameunavalible0/css-internal-base) - base of this cheat
*   [SEOwnedDE](https://github.com/spook953/SEOwnedDE-public) - menu of this cheat
*   [Osiris CS:GO](https://github.com/danielkrupinski/Osiris/blob/csgo) - where I shamelessly got this readme
