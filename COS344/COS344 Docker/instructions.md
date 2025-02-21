
# COS344 Docker Instructions - 2025

## Introduction

In COS344, OpenGL 3.3 is used together with C++. As can be imagined, the setup can be described as "special" due to the complex nature of installing C++ libraries. As such, to aid students with the setup required for OpenGL, a docker container has been created. This instruction manual outlines the required steps to set-up the docker container, as well as the manual setup that can be followed if you choose to not use the docker container. This manual utilises VSCode and as such, it cannot be guaranteed that any other IDE will work as described. Docker is a platform that uses containerisation to package applications and their dependencies into lightweight, portable containers that run consistently across different environments. It simplifies development, deployment, and scaling by isolating applications from the underlying system, ensuring reproducibility and reducing conflicts.

## Requirements

- [Docker Desktop](https://www.docker.com/products/docker-desktop/)
- [C++](https://isocpp.org/)
- IDE (this manual uses [VSCode](https://code.visualstudio.com/))
- [VSCode Docker Extension](https://marketplace.visualstudio.com/items?itemName=ms-azuretools.vscode-docker)
- [VSCode Dev Containers Extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers)

## Docker Instructions

### Provided Files

You have been provided with the following files:

- dockerfile
- docker-compose.yml
- gladSetup.sh

### File Structure

This manual assumes the following file structure:

``` Markdown
Your COS344 Module Folder
├── docker-compose.yml
├── dockerfile
├── gladSetup.sh
├── instructions.md
└── workspace
```

The `workspace` folder is a folder that will act as a local volume that will be attached to the docker container, such that the edits that you perform in the docker container persists on your local storage. It is recommended that all of the COS344 code examples, practicals, and homework assignments be completed in this folder, each in their own separate subfolder(s).

### Steps

1. After downloading and installing docker desktop, run docker desktop.
2. Open your host operating system's terminal in the `Your COS344 Module Folder`.
3. Run the following command to spin up the docker container: `docker-compose build`.
4. If the docker container was successfully built, it should indicate something like this at the start of the output: `Building 4.1s (14/14) FINISHED` Note the time may be different the first time you build the container.
5. After a successful build, run the following command to start the docker container: `docker-compose up`.
6. If successfully started the following should be displayed:

``` Markdown
Attaching to cpp-dev-1
```

Seeing `Attaching to cpp-dev-1` implies that the docker container is running. This can be verified through the docker desktop UI.

7. Open a new instance of VSCode and press the following key combination: `F1` (alternatively `Ctrl Shift P`). This will opens the command pallet.

8. Type in the following setting: `Remote Explorer: Focus on Dev Containers View` and select this option from the menu. 

9. Once this command has finished executing, you should see a side menu that contains the current containers that is running on your computer. Select the `cos-344-cpp-dev-1` container and attach VSCode to it by clicking on the `->` icon. To confirm that you are connected to the docker container, ensure that `Container cos344docker-cpp-dev` is displayed in the bottom left corner of VSCode.

10. To navigate to your local files, go to `File -> Open Folder` and enter the directory: `/home/dev/workspace/workspace/` and click on ok. (Ensure that you do not click on `show local` as this closes the container.)

11. In your VSCode file explorer, you should see all the files and folders contained in the `Your COS344 Module Folder/workspace` created earlier. Ensure that you are connected to the docker container, by ensuring that `Container cos344docker-cpp-dev` is displayed in the bottom left corner of VSCode.

12. Step 10 can be used to navigate to a specific folder that you would like to work in. You have also been provided with an example program which can be used to ensure the container is correctly configured.

13. In VSCode, navigate to the `DockerExample` folder is contained and open a VSCode terminal in this folder. Run the `make all` command to compile and run the example program. If everything was correctly configured a window should open containing a blue triangle.

## Manual Instillation

This section assumes that a Linux based operating system is used. The instructions detailed below is derived from the following two resources: [OpenGL-Tutorial](https://www.opengl-tutorial.org/beginners-tutorials/tutorial-1-opening-a-window/) and [GLAD Setup](https://medium.com/geekculture/a-beginners-guide-to-setup-opengl-in-linux-debian-2bfe02ccd1e). It is advices that only experienced students perform a manual setup.

### Required Libraries

- cmake
- make
- g++
- libx11-dev
- libxi-dev
- libgl1-mesa-dev
- libglu1-mesa-dev
- libxrandr-dev
- libxext-dev
- libxcursor-dev
- libxinerama-dev
- libxi-dev
- pkg-config
- mesa-utils
- freeglut3-dev
- mesa-common-dev
- libglew-dev
- libglfw3-dev
- libglm-dev
- libao-dev
- libmpg123-dev

### GLFW installation

Execute the following commands to install GLFW:

- `cd /usr/local/lib/`
- `git clone https://github.com/glfw/glfw.git`
- `cd glfw`
- `cmake .`
- `make`
- `sudo make install`

### Glad Files

To aid with some of the low level system setup you are permitted to utilise Glad. This section details the acquisition of the glad files and the correct placement of these files such that the docker container can correctly use them.

1. Go to the following site: [Glad Web Service](https://glad.dav1d.de/).
2. Select the following options:

|Option                                          | Value |
|------------------------------------------------|-------|
|Language                                        | C/C++ |
|Specification                                   | OpenGL|
| Profile                                        | Core  |
| API - gl                                       | 3.3   |
| Leave the remaining API empty                  |       |
| Do not select any extensions                   |       |
| Ensure that only generate a loader is selected |       |

3. After the setup click on the `Generate` button.
4. Download the `glad.zip` file by clicking on the name.
5. Move the glad.zip into the `Your COS344 Module Folder`.
6. Extract the folder such that the file structre is now:

```markdown

Your COS344 Module Folder
├── docker-compose.yml
├── dockerfile
├── glad
│   ├── include
│   │   ├── glad
│   │   │   └── glad.h
│   │   └── KHR
│   │       └── khrplatform.h
│   └── src
│       └── glad.c
├── gladSetup.sh
├── glad.zip
├── instructions.md
└── workspace

```

The glad.c file you will need to manually copy into each OpenGL program. The remaining setup will be covered in the following section.

## Trouble shooting

This section contains solutions to problems that have been found by students using this guide. If you experience a problem that cannot be solved by any of the solutions listed here, please ask on Discord and a lecturer or tutor will assist, or please attend a practical session as soon as possible.

### Cannot start service cpp-dev: Mounts denied

If you experience this error please try the following [solution](https://stackoverflow.com/a/68495984).

## Cannot find -lglfw3: No such file or directory

If you are experiencing this error, please replace the `-lglfw3` in the compile command with `-lglfw`.
