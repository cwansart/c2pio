# c2pio

**c2pio** is a command-line tool for downloading files from a given URL. The project is developed in C++ and uses the `libcurl` library for HTTP communication.


## Prerequisites

You'll need CMake (version 3.10 or higher) and a C++ compiler that supports the C++11 standard. The `libcurl` development library must also be installed on your system.

To install the necessary dependencies on Ubuntu or other Debian-based systems, run:

```sh
sudo apt update
sudo apt install cmake build-essential libcurl4-openssl-dev
```

## Compiling

Follow these steps to compile the project:

```sh
git clone https://codeberg.org/cwansart/c2pio.git
cd c2pio
mkdir build
cd build
cmake ..
cmake --build .
```

After compilation, you'll find the executable `c2pio` in the `build` directory.

## Usage

The tool is executed from the command line.

```sh
./c2pio <url> <out-file>
```

  * `<url>`: The URL of the file to download.
  * `<out-file>`: The path to save the file to. If not specified, the filename is extracted from the URL.

## Examples

  * Download a file and save it with a custom name:

    ```sh
    ./c2pio https://example.com/data/file.zip my_file.zip
    ```
