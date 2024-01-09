## Build

### Windows (x64)

##### Requirements:

- Python 3.8 or higher

- CMake 3.25 or higher

- MinGW toolchain v. 11 or higher. You can use MSVC toolchain if you wish, in that case, uncomment conan profile path in `build.py` and comment old profile path

- Qt version 6.5.0 or higher with this optional modules:

  - Qt Location
  - Qt Positioning

- Internet connection

  

##### Build steps:

- Install Conan:

  

  ```sh
  pip install conan
  ```

  Alternatively, download the Conan package via the [Conan website](https://conan.io/downloads)

- If not present, add `CMake`, `Ninja` and `g++` in **PATH**.
- Optionally, add Qt's binaries (such as `windeployqt` in **PATH**)

- Run build script to install all Conan dependencies:

  ```sh
  python build.py
  ```

- Run CMake with this options:

  ```cmake
  -DCMAKE_PREFIX_PATH=C:\Qt\6.7.0\mingw_64 --preset "conan-release"
  ```

> Replace `CMAKE_PREFIX_PATH` with actual path to Qt Library. If you are building with Qt Creator or have Qt installed in `PATH`, you can skip this step.

- Run `windeployqt` to install all required `.dll`s or place them manually:

  ```sh
  cd $build_directory # your actual build directory here
  windeployqt .
  ```

  TODO: 

  - replace hardcoded profiles in build.py with args
  - add rust part in description