# Setup And Usage Guide

## Release
- navigate to releases and download the zip for the latest version
- after unzipping it run the exe once to create the json files
- data.json requires you to input the direct path for all images used in the program 
format them like so:
```plaintext
C:\\documents\\example_folder\\images\\image.png
```

## Source
- clone or download the repo wherever you'd like
- download the Visual C++ 17 (2022) of sfml 3.0 from [here](https://www.sfml-dev.org/download/sfml/3.0.0/)
- copy the include/ and lib/ folders from sfml 3 and put them in the root of your sfml-overlay folder
- next in VS 2022 run the debug or release version (this is expected to crash the first time)
- lastly locate the bin folder in sfml 3 and copy  the window graphics and system .dll-s into x64
  Note: debug requires the -d version of the DLLs 
  the final folder structure should look something like this:
```plaintext
sfml-overlay/
├── include/
├── lib/
├── x64/
│   ├── Debug/
│   |    ├── sfml-system-d-3.dll
│   |    ├── sfml-graphics-d-3.dll
│   |    └── sfml-window-d-3.dll
│   └── Release/
│        ├── sfml-system-3.dll
│        ├── sfml-graphics-3.dll
│        └── sfml-window-3.dll
└── nothing else requires modification
```
