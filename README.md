# LucrareLicenta

Instalare

1. Se cloneaza codul in Visual Studio
2. Se creaza un Proiect C++ in Visual Studio din codul existent
   ("File" -> "New" -> "Project from existing code..." ->)
   Se selecteaza locatia unde s-a clonat codul si se da un nume pentru proiect.
3. (https://github.com/raysan5/raylib/releases/tag/4.5.0) -> Assets se downloadeaza libraria raylib "raylib-4.5.0_win64_msvc16"
4. Se dezarhiveaza libraria
5. De exemplu "C:\Libraries\raylib-4.5.0_win64_msvc16" poate fi folderul in care este stocata libraria
6. In proiectul din Visual Studio Click dreapta pe Proiect in "Solution Explorer" -> "Properties"
7. "Configuration Properties" -> "C/C++" -> "General" -> "Additional Include Directories" se introduce path-ul spre folderul "include" din raylib
8. Urmarind exemplul care este scris mai sus acest path ar fi "C:\Libraries\raylib-4.5.0_win64_msvc16\include"
9. "Configuration Properties" -> "Linker" -> "General" -> "Additional Library Directories" se introduce path-ul spre folderul "lib" din raylib
10. Urmarind exemplul care este scris mai sus acest path ar fi "C:\Libraries\raylib-4.5.0_win64_msvc16\lib"
11. "Configuration Properties" -> "Linker" -> "Input" -> "Additional Depencies" se scriu urmatoarele 2 nume de fisiere
    "raylib.lib", "winmm.lib", in casuta din dreapta de la "Additional Depencies" textul ar trebui sa fie sub aceasta forma "raylib.lib;winmm.lib;$(CoreLibraryDependencies);%(AdditionalDependencies)"
12. "Apply" -> "Ok"
