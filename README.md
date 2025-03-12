Tale of Two Wastelands Installer
============

This is the development repository for the installer used in the [Tale of Two Wastelands](http://www.twowastelands.com/) mod.

> [!WARNING]
> This fork of the project is not affiliated with the original developers.

Dependencies
------------
- CMake
- QT6
- [Ryan-rsm-McKenzie::bsa](https://github.com/Ryan-rsm-McKenzie/bsa)
- [rikyoz::bit7z](https://github.com/rikyoz/bit7z)
- [stephenberry::glaze](https://github.com/stephenberry/glaze)

Packages to be replaced:
- BSAsharp          => bsa
- Newtonsoft.Json   => bit7z
- SevenZipSharp     => Already obsolete
- SharpZipLib       => glaze
- structuremap      => Probably not needed
