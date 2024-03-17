# MathVizAnimator

[![build](https://github.com/codingwithmagga/mathvizanimator/actions/workflows/build.yml/badge.svg)](https://github.com/codingwithmagga/mathvizanimator/actions)
[![mega-linter](https://github.com/codingwithmagga/mathvizanimator/actions/workflows/mega-linter.yml/badge.svg)](https://github.com/codingwithmagga/mathvizanimator/actions)
[![codecov](https://codecov.io/github/codingwithmagga/mathvizanimator/graph/badge.svg?token=NNOP09V20E)](https://codecov.io/github/codingwithmagga/mathvizanimator)

**MathVizAnimator** is a program to create mathematical based animations and render them into a video file. It is highly inspired by [manim](https://www.manim.community/).

The program is written in C++ extensively using the [Qt](https://www.qt.io/) Library. For rendering the [FFmpeg](https://ffmpeg.org/) library is used and text can be created and added by using [LaTeX](https://www.latex-project.org/). 

## Features

* GUI to create math based animation videos

* Geometry shapes like a circle or a rectangle can be added via Drag & Drop to the video

* Multiple properties to change style of geometry shapes like color, border width or opacity

* LaTeX interface to create svg images which can be added to the video

* Animations like FadeIn or FadeOut

* Interface to create videos by using FFmpeg

## Getting Started

These instructions will get you a copy of the project up and running on your local
machine for development and testing purposes. Since this is just an alpha version, there exists no release or any option to install the program at the moment. Please also have a look at the current [documentation](https://mathvizanimator.readthedocs.io/en/latest/).

> ***Important Note:*** As stated above the program is currently under development and in an really early (alpha) stage. Be aware that using the program for
important or long tasks is not recommended. For example there is no autosave function, not even a check if there are changes when closing the program. Crashes
can occur at any time. Please keep that in mind. If you discover an issue (which is very likely) or have an idea for a new feature (which wouldn't surprise me)
please have a look at [CONTRIBUTING.md](CONTRIBUTING.md).

If you want to use MathVizAnimator on MacOS please be aware of the following note.

> ***Important Note for MacOS:*** Currently only Linux and Windows builds are tested live. There is a github action to build and run tests on MacOS, but it was never executed there directly. If you want to run MathVizAnimator on MacOS be aware that there could be unknown bugs and behavior. Please open an [Issue](https://github.com/codingwithmagga/mathvizanimator/issues/new/choose) if any problems occur.

### Prerequisites

* **Qt library v6.5** - found at [https://www.qt.io/](https://www.qt.io/)

* **CMake v3.24+** - found at [https://cmake.org/](https://cmake.org/)

* **C++ Compiler** - needs to support at least the **C++20** standard, i.e. *MSVC*,
*GCC*, *Clang*

> ***Note:*** *You also need to be able to provide ***CMake*** a supported
[generator](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html).*

* **FFmpeg** - found at [https://ffmpeg.org/](https://ffmpeg.org/)

* **LaTeX** - Depends on your OS, see [https://www.latex-project.org/get/](https://www.latex-project.org/get/)

### Prerequisites for the documentation

* **doxygen** - found at [https://www.doxygen.nl/](https://www.doxygen.nl/)

* **sphinx** - found at [https://www.sphinx-doc.org/en/master/](https://www.sphinx-doc.org/en/master/)

## Building the project

First clone the project to your system.

```bash
git clone https://github.com/codingwithmagga/mathvizanimator
```

Check out that all of the [Prerequisites](#prerequisites) above are installed on your system.
After that for building the project, is all you need to do running a similar **CMake** routine
to the the one below:

```bash
mkdir build/ && cd build/
cmake ..
cmake --build .
```

> ***Note:*** It might be easier to just use an IDE (like [QtCreator](https://www.qt.io/product/development-tools)) for this task.

## Generating the documentation

There are some additional dependencies for the documentation, see [Prerequisites for the documentation](#prerequisites-for-the-documentation).
In order to generate documentation for the project, you need to configure the build
to use Doxygen. This is easily done, by modifying the workflow shown above as follows:

```bash
mkdir build/ && cd build/
cmake .. -D BUILD_DOCUMENTATION=1
cmake --build .
```

> ***Note:*** It my be easier to set these CMake variable in your IDE.

## Running the tests

This project uses the [QTest library](https://doc.qt.io/qt-6/qtest-overview.html)
for unit and integration testing. 

```bash
cd build          # if not in the build directory already
ctest
```

All tests will be executed in the CI workflow of this project.

## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our how you can
become a contributor and the process for submitting pull requests to us.

## Versioning

This project makes use of [SemVer](http://semver.org/) for versioning.

## Authors

* **Marco Johann Schoos** - [@codingwithmagga](https://github.com/codingwithmagga)

## License

This project is licensed under the [GPL-3.0 license](https://www.gnu.org/licenses/gpl-3.0.de.html) - see the
[LICENSE](LICENSE) file for details
