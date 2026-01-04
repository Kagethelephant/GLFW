# OpenGl Planet Strategy Game

## Overview
This is not really notes about the game but notes about the process and steps i am taking to develop the project so i can reference this later.

## Adding the GLFW Library
I liked the method of adding GLFW as a git submodule and building the library paralell with the game. This way i can pull a new instance of the repo if i want to update GLFW

When you clone the repo you should use `git clone --recurse-submodules <repo>` to clone GLFW as well.
If you forgot to use recursive method, you can use `git submodule init` to bring them in after
To update the submodules you can use `git submodule update`

## CMakeLists.txt
