# PACMAN

The pacman game made in C language with "allegro" library.
A school project for the 3rd year of high school 05/2019.

### Prerequisites

You need CodeBlocks IDE and MinGW configured for Windows.

### Installing

You have to download allegro lib files, depending on your MinGW version, from https://www.allegro.cc/files/.
Then extract the files in the project directory, for example: z:\allegro\:
Open the project in CodeBlocks and:
Start a new project (File→New→Project→Empty project)
● Right-click the project file and go to Build Options
● Right-click the project name, go to Search directories → compiler → add, and add z:\allegro\include
● For Search directories → Linker add z:\allegro\bin
● For Debug go to Linker settings → add and select liballegro5.x.x-monolith-mt-debug.a from the directory z:\allegro\lib
● For Release go to Linker settings → add and add liballegro5.x.x-monolith-mt.a from z:\allegro\lib

### Break down into end to end tests

Explain what these tests test and why

```
Give an example
```

### And coding style tests

Explain what these tests test and why

```
Give an example
```

## Built With

* [Dropwizard](http://www.dropwizard.io/1.0.2/docs/) - The web framework used
* [Maven](https://maven.apache.org/) - Dependency Management
* [ROME](https://rometools.github.io/rome/) - Used to generate RSS Feeds

## Authors

* **Billie Thompson** - *Initial work* - [PurpleBooth](https://github.com/PurpleBooth)

See also the list of [contributors](https://github.com/your/project/contributors) who participated in this project.

## Acknowledgments

* Hat tip to anyone whose code was used
* Inspiration
* etc
