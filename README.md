# AFoP Mod Manager

# /!\ CURRENTLY NOT WORKING - ONLY SOURCE CODE /!\

## F.A.Q.
### What is this?

This is the repository that contains the Frontiers of Pandora Mod Manager.
This tool will allow you to simply drag and drop your mods into a folder, enter a word and have the commands work instantly.

### Will there be a User Interface to this some day?

Yes! The UI will soon be implemented. I currently put the focus on the functions of the tool and getting it to work as seamlessly as possibe.
If you encounter any bugs, please open an issue on this very repository. 

## Usage
You will not need to download anything else before usage. Great thanks to Patrick Mollohan, who generously accepted for me to redistribute his Ultimate Snowdrop ModLoader (https://github.com/patrickmollohan/Universal-Snowdrop-Modloader)!
Everything will be installed and configured in a way that you will just need to launch the game and not do anything else.

This tool can seem quite overwhelming for many people at first if they don't know how to use a command line.
The idea is that you enter a command that allows you to... say turn mods on or off. 
**Here is the current list of commands:**
- `exit`:
    * This command allows you to exit the program
- `help`:
    * This command shows you a list of all commands that are currently available and their usage.
- `enable <MOD_ID>`:
    * This command turns on the given mod, identified by it's unique Mod-ID.
- `disable <MOD_ID>`: 
    * This command turns off the given mod, identified by it's unique Mod-ID.
- `package <name> <path>`:
    * This command allows mod creators to package their mods into *.afopmod files. A new universal file format to make modding easier.\n
      More information later.
- `depackage <name> <output-path>`:
    * This command allows anyone to depackage the mod they downloaded into the given output-directory.
- `info <MOD_ID>`:
    * This command displays information about a mod package.
- `launch`: 
    * This command allows you to launch the game.
- `modless-launch`:
    * This command launches the game without any modifications.


## The .afopmod packages:
The .afopmod packages are an addition that is supposed to simplify the modification of the game and is basically just the same as a ZIP-File, just without compression.
Later on, a compression algorithm will be implemented to minimize the size of these packages to further simplify diffusion of mods via all kinds of websites.
These packages will be able to be depackaged by everyone who has access to the AFoP Mod Manager. They are not supposed to be encrypted and/or protected in any way.
