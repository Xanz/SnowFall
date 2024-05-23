# SnowFall Engine

SnowFall Engine or SFEngine is a fork of the Doom 3 BFG engine. The overall goal is to move the engine to a point that standalone games can be more easily created and modernize the dated tooling associated with Id Tech 4/5. I will do this by introducing better tooling, improving the net code and improving the renderer/backend so that it can support higher entity counts/larger worlds.

## Features

- GLFW & GLEW support
- CMake build system
- Lightweight
- Simplified console commands
- OpenGL renderer

## Supported Platforms

- Windows
- I plan on adding Linux support eventually, its just not a priority at the moment.

## FAQ

**How do I get started with using the engine?**

> As part of the repo I have provided a series of documentation that should help navigate around the engine and start building projects. Here is a link to the getting started page. (TODO: add documentation)
> 

**What are the major goals of the engine?**

> Included in the docs folder there is a roadmap that should hopefully allow for you to get a better understanding of what to expect.
> 

**Do you plan on supporting Radiant or other map editors?**

> Probably not, I want to create my own tooling so my focus will be on that. If the tools do work with the fork then great feel free to use them!
> 

**Why was the engine based on the original BFG source instead of other community forks?**

> To put it simply freedom, I want to be able to make changes to the engine that could cause serious problems with maintaining previous game projects made with the engine. I also do not want to be constrained by what other people think/want or have key features not merged in and forever abandoned in a separate branch. Code styling/formatting is pretty important to me and I want to be able to control this myself as well.
> 

**Why did you choose to create a BFG fork instead of using a mainstream engine?**

> I’ve used many different engines over the years and they have all had issues/functionality that just caused me to rethink using them. I hope to solve some of these issues, while also moving the technology forward and learning more about engine/game development.
> 

**I’ve found a bug or want to contribute to the engine?**

> If you have found any bugs with the engine simply raise an issue in the Github page and I will take a look at it when I can. If you would like to submit new features or bugfixes create a pull request and I can take a look at your changes, Please make sure that your code is commented and the pull request is specific in the goal/issue fixed.
>
