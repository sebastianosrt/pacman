# PACMAN

The pacman game made in C language with "allegro" library.
A school project for the 3rd year of high school 05/2019.

### Prerequisites

You need CodeBlocks IDE and MinGW configured for Windows.

### Installing

You have to download allegro lib files, depending on your MinGW version, from https://www.allegro.cc/files/.


Then extract the files in the project directory, for example: z:\allegro\

Open the project in CodeBlocks and:

Start a new project (File→New→Project→Empty project)

● Right-click the project file and go to Build Options

● Right-click the project name, go to Search directories → compiler → add, and add z:\allegro\include

● For Search directories → Linker add z:\allegro\bin

● For Debug go to Linker settings → add and select liballegro5.x.x-monolith-mt-debug.a from the directory z:\allegro\lib

● For Release go to Linker settings → add and add liballegro5.x.x-monolith-mt.a from z:\allegro\lib

## Built With

* [Allegro](https://www.allegro.cc/) - The graphics library
* [Algif](http://algif.sourceforge.net/) - Gif loader plugin

## Authors

* **Sartor Sebastiano** - [sebastianosrt](https://github.com/sebastianosrt)

* **Karroca Lodovico** - [lerriks](https://github.com/Lerriks)
