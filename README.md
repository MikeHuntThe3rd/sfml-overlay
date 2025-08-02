# Setup And Uusage Guide

## Release
- navigate to releases and download the zip for the latest version
- after unzipping it run the exe once to create the json files
- data.json requires you to input the direct path for all images used in the program 
format them like so:
```C:\\documents\\example_folder\\images\\image.png```

## Source
- clone or download the repo wherever youd'd like
- download the Visual C++ 17 (2022) of sfml 3.0 from [here](https://www.sfml-dev.org/download/sfml/3.0.0/)
- copy the include/ and lib/ folders from sfml 3 and put them in the root of your sfml-overly folder
- next in VS 2022 run the debug or release version (this is expected to crash the first time)
- lastly locate the bin folder in sfml 3 and copy  the window graphics and system .dll-s into x64
  the final folder structure should look something like this:
```plaintext
sfml-overlay/
├── libs/          # All libraries go here 
│   ├── libglfw.a       # Linux/macOS static
│   ├── glfw3.lib       # Windows static
│   └── libglfw.dll.a   # MinGW import lib
├── include/        # Header files (.h)
│   └── header.h
├── src/            # Source files (.cpp)
│   └── source.cpp
├── main.cpp        # Main program
└── Makefile
```
