# SnowFall Engine

SnowFall Engine or SFEngine is a fork of Id Tech 4. The overall goal is to move the engine to a point that standalone games can be more easily created and modernize the dated tooling associated with Id Tech 4. I will do this by introducing better tooling, improving the net code and improving the renderer/backend so that it can support higher entity counts/larger worlds.

## Features

- GLFW & GLEW support
- CMake
- Lightweight
- OpenGL renderer
- Variable frame rate support
- BFG OpenAL sound engine
- 64 bit support

## Supported Platforms

- Windows
- I plan on adding Linux support eventually, its just not a priority at the moment.

## FAQ

**How do I get started with using the engine?**

> As part of the repo I have provided a series of documentation that should help navigate around the engine and start building projects. [Here is a link to the getting started page.](docs/GettingStarted.md)
You can also view the build instructions [here](docs/BuildGuide.md)
> 

**What are the major goals of the engine?**

> Included in the docs folder there is a roadmap that should hopefully allow for you to get a better understanding of what to expect.
> 

**Do you plan on supporting Radiant or other map editors?**

> Probably not, I want to create my own tooling so my focus will be on that. If the tools do work with the fork then great feel free to use them! The only map editor I would probably consider is Trenchbroom as it has more modern workflows and key bindings.
> 

**I’ve found a bug or want to contribute to the engine?**

> If you have found any bugs with the engine simply raise an issue in the Github page and I will take a look at it when I can. If you would like to submit new features or bugfixes create a pull request and I can take a look at your changes, Please make sure that your code is commented the pull request specific in the goal/issue fixed.
>