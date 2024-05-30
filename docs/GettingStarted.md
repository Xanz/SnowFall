# Getting Started

Welcome to the getting started page here you will find a series of links to various different documentation as well as some useful information if you are considering using the engine. First of all if you would like to know what the engine’s future plans are you can take a look here. (TODO: will add this later.)

[***Click here for the build instructions.***](BuildGuide.md)

Before you start using the engine you should have a read of the following sections to get a better understanding of what to expect and the current status of how easy the engine is to use.

## Should you use this engine?

Currently I would say no, While the engine is in a good state in terms of being able to be built extremely easily the tooling is lacking. The documentation is also not in a good state at the moment because of how early the project is. The engine is heavily reliant on the Doom 3 assets. Until I can decouple the engine from the Doom 3 codebase it is probably best to either wait or take a look at more mature engines such as Unreal engine or Unity.

## How hard is the engine to use?

The engine exposes functionality to Doomscript which should make it easier for non-programmers to make changes to the game without needing to rebuild any C++ source. However this does require the scripter to learn a language that has no auto completion tools and very little documentation other than random guides around the internet.

Exporting new models is currently non documented so this will likely require some level of understanding how Id Tech 4 handles 3d models.

The C++ codebase is overall quite clean and an experienced programmer can likely understand what is happening by using the engine for a few weeks. I am actively working on cleaning this codebase up over time by reducing the amount of dead code and removing long forgotten dependencies.