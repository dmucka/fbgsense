# fbgsense
This archive contains the source code of the "fbgsense" method developed by Daniel Múčka, Masaryk University, Faculty of Informatics.

You can read more in the full text of my [bachelor's thesis](https://is.muni.cz/th/mf5l7/).

The project consists of the following folder structure:

```
fbgsense/
    |
    |- gui/              GUI application based on wxWidgets.
    |- cli/              CLI application.
    |- sm125virtsw/      Virtual switch application.
    |- include/          fbgsense library headers.
    \- src/              fbgsense library implementation.
```
Other files and/or folders contain source codes of the third-party tools and libraries:

 - openGLPlotLive https://github.com/tbattz/openGLPlotLive (fork https://github.com/dady8889/openGLPlotLive.git)
 - gsl-curve-fit https://github.com/Eleobert/gsl-curve-fit
 - cpp-terminal https://github.com/jupyter-xeus/cpp-terminal
 - vcpkg https://github.com/microsoft/vcpkg (fork https://github.com/JackBoosY/vcpkg.git)

These libraries must be downloaded manually or using the Git submodules specified in the file .gitmodules.
The authors and licenses of each library/tool are specified in the respective subfolder in the LICENSE file.

The vcpkg tool is necessary to download other dependencies, such as OpenGL, FFTW, GSL, etc.
These dependencies will be automatically downloaded and built after initial CMake setup.

The application has been built in Visual Studio 2022 on Windows operating system using MSVC compiler.

Documentation is provided using in-code comments and Doxygen generated HTML documentation in the documentation/ folder (open with index.html).

