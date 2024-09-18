# Arsenal [download](https://github.com/Viceroyy/Arsenal/releases/download/v1.0.0/Arsenal-Release.dll)

[![Discord](https://img.shields.io/discord/1267955231636062259?logo=Discord&label=discord)](https://discord.gg/4apwXtJZPH)

Free open-source Windows cheat software for **Counter-Strike Source** game. Designed as an internal cheat - [Dynamic-link library](https://en.wikipedia.org/wiki/Dynamic-link_library) (DLL) loadable into game process. Compatible with the Steam version of the game.

### Notable things
This cheat disables sv_pure, servers being able to issue commands to your client, removes event delays, and fixes general input delay all automatically and without options in the menu or the config.

## Features
*   **Exploits** - things not possible when legit
*   **ESP** - show additional information about players and game world as text
*   **Materials** - draw the players in a custom material, aka chams
*   **Outlines** - draw an outline on players, aka glow
*   **Visuals** - miscellaneous visual options
*   **Misc** - miscellaneous features
*   **Config** - JSON-based configuration system

<details>

*   **Exploits** - things not possible when legit
    *   **No recoil** - remove recoil punch effect
    *   **No spread** - remove random bullet spread
    *   **Sequence freezing** - instant bomb/grenade explosion etc.
      
*   **ESP** - show additional information about players and game world as text
    1.  *Local, Teammates, Enemies*

    2.  *Planted C4, Dropped weapons*

    *   **Enabled** - on / off master switch
    *   **Box** - draw 2D box over player model
    *   **Name** - draw player name
    *   **Health** - draw player health
    *   **Health bar** - draw rectangle indicating player health
    *   **Armor** - draw player armor
    *   **Armor bar** - draw rectangle indicating player armor
    *   **Money** - draw player money
    *   **Weapon** - draw player equipped weapon
    *   **Arrows** - draw arrow to player when out of fov

*   **Materials** - draw the players in a custom material, aka chams
    1.  *Local, Teammates, Enemies*

    2.  *Planted C4, Dropped weapons*

    *   **Enabled** - on / off master switch
    *   **No depth** - removes depth on materials, allowing to see through walls
    *   **Alpha** - modify material alpha

*   **Outlines** - draw an outline on players, aka glow
    1.  *Local, Teammates, Enemies*

    2.  *Planted C4, Dropped weapons*

    *   **Enabled** - on / off master switch
    *   **Bloom amount** - modify the bloom amount of outlines, aka thickness
    *   **Alpha** - modify outlines alpha
    
*   **Visuals** - miscellaneous visual options
    *   **No visual recoil** - remove visual recoil punch effect
    *   **No interpolation** - remove player interpolation for accuracy, you will see entities having jerky movement/animation
    *   **No DSP** - remove all sound effects/reverberation like deaf/echo/ricochete effects
    *   **No post processing** - remove post processing effects
    *   **No ragdolls** - remove ragdolls when players die
    *   **No angle forcing** - remove server being able to change your view angles
    *   **No convar queries** - remove server being able to query the values of convars from our client
    *   **No screen effects** - remove fade/shake/rumble effects
    *   **No MOTD** - remove message of the day panel when joining a server
    *   **Thirdperson** - apply thirdperson camera at will
    *   **Disable fog** - disables fog in the world
    *   **Disable sky fog** - disables fog in the skybox
    *   **Distance prop alpha** - makes the props see-through when near
    *   **World modulation** - apply color to skybox and/or world or night mode
    *   **Viewmodel FOV** - change view model FOV \[*70*-*120*\] (70 - default viewmodel, higher values - further away viewmodel)
    *   **FOV** - change view FOV \[*90*-*120*\] (90 - default fov, higher values - further away fov)
    *   **Crosshair on snipers** - show crosshair on sniper rifles
    *   **Low graphics** - makes the game look like N64 games
    *   **Spread circle** - show the radius of spread in a circle
    *   **Crosshair** - draw simple + crosshair

*   **Misc** - miscellaneous features
    *   **Auto strafe** - automatically strafe in air following mouse movement
    *   **Bunny hop** - automatically simulate space bar press / release while jump button is being held; increases movement speed
    *   **Spectator list** - show players who are spectating you
    *   **Backtrack** - exploit lag compensation and kill players where they were in the past
    *   **Fake latency** - choose between optimized which keeps your scoreboard ping around 150ms or a custom value to extend the backtracking window by
    *   **Playerlist** - mark players to quickly recognize them

*   **Config** - JSON-based configuration system
    *   **Create config** - create new configuration file
    *   **Load** - load selected configuration file
    *   **Save** - save selected configuration file
    *   **Delete** - delete selected configuration file

*   **Other**
    *   **Notification system** - notifies the user about stuff when needed

</details>

## Getting started

### Downloading the DLL

If you haven't already, download the DLL by clicking [here](https://github.com/Viceroyy/Arsenal/releases/download/v1.0.0/Arsenal-Release.dll). Download size ~600 kB.

### Loading / Injecting into game process

Open your favorite [DLL injector](https://en.wikipedia.org/wiki/DLL_injection) and just inject `Arsenal-Release.dll` into `hl2.exe` process.

When injected, menu is openable with the <kbd>INSERT</kbd> key. To unload the DLL from the game, press the <kbd>F11</kbd> key.

### Compiling from source

<details>

### Prerequisites
Microsoft Visual Studio 2022 17.11.4 (or newer), platform toolset v143 and Windows SDK 10.0 are required in order to compile Arsenal. You can download VS [here](https://visualstudio.microsoft.com/) (Windows SDK is installed during Visual Studio Setup).

### Downloading

There are two options of downloading the source code:

#### Without [git](https://git-scm.com)

Choose this option if you want pure source and you're not going to contribute to the repo. Download size ~600 kB.

To download source code this way [click here](https://github.com/Viceroyy/Arsenal/archive/main.zip).

#### With [git](https://git-scm.com)

Choose this option if you're going to contribute to the repo or you want to use version control system. Download size ~2.5 MB. Git is required to step further, if not installed download it [here](https://git-scm.com).

Open git command prompt and enter following command:

    git clone https://github.com/Viceroyy/Arsenal.git

`Arsenal` folder should have been successfully created, containing all the source files.

When you have equipped a copy of the source code, next step is opening **Arsenal.sln** in Microsoft Visual Studio 2022.

Then change build configuration to `Release | x86` and simply press **Build solution**.

If everything went right you should receive `Arsenal-Release.dll`  binary file.

### Instruction sets
If your CPU doesn't support the AVX2 instruction set, you can use SSE2 instructions instead in project settings. Currently AVX2 instructions are selected in project settings.

</details>

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
*   [Osiris CS:GO](https://github.com/danielkrupinski/Osiris/tree/csgo) - where I shamelessly stole this readme