# DfpwmForge

**A lightweight, cross-platform MP3-to-DFPWM converter designed for CC:Tweaked.**

DfpwmForge is a native C++ application built to convert MP3 audio into DFPWM format for use with [CC:Tweaked](https://tweaked.cc/). It handles the audio processing needed to prepare files for playback through CC:Tweaked's speaker peripherals, including stereo-to-mono conversion and 8-bit audio conversion.

While designed with CC:Tweaked compatibility as its primary goal, DfpwmForge can also be used to just generally generate `.dfpwm` audio files for other applications that support the format.

## Development Notes

- **WAV support:** WAV-to-DFPWM conversion is currently in development and is planned for a future update.
- **Sample rate:** DfpwmForge does not currently resample audio to 48 kHz. CC:Tweaked speakers play audio at 48 kHz, so MP3 files with other sample rates may play at an incorrect speed or pitch. For the best playback results, use 48 kHz MP3 files until sample rate conversion is implemented.

## Features

- MP3 to DFPWM conversion for CC:Tweaked
- Audio processing tailored to CC:Tweaked's DFPWM playback requirements
- Stereo-to-mono audio conversion
- 16-bit to 8-bit audio conversion
- Audio decoding using [minimp3](https://github.com/lieff/minimp3)
- Lightweight, native C++17 implementation




## Usage

DfpwmForge is a command-line application that converts MP3 audio files into DFPWM format.

### Basic Conversion

**Windows (PowerShell):**
```powershell
.\dfpwm.exe file.mp3
```

**Linux:**
```bash
./dfpwm file.mp3
```

The converted file is automatically saved alongside the input file, using the same filename with the `.mp3` extension.

Use `-o` to specify a custom output name.

**Windows (PowerShell):**
```powershell
.\dfpwm.exe input.mp3 -o fileName
```

**Linux:**
```bash
./dfpwm input.mp3 -o fileName
```

DfpwmForge automatically appends `.dfpwm` if the specified output filename doesn't already have that extension.

You can specify a path in another directory as well:

**Windows (PowerShell):**
```powershell
.\dfpwm.exe input.mp3 -o "$HOME\Desktop\convertedfile.dfpwm"
```

**Linux:**
```bash
./dfpwm input.mp3 -o ~/Dekstop/convertedfile.dfpwm
```

### Command Syntax

```text
dfpwm <input_path.mp3> -o <output_path>
```

| Argument | Description |
|---|---|
| `input.mp3` | Path to the MP3 file to convert |
| `-o` | Optional flag to specify the output path |
| `output_path` | Custom output filename or path. The `.dfpwm` extension is added automatically when missing |

### Conversion Output

After a successful conversion, DfpwmForge displays the input file, output file, and number of encoded bytes.

### Example

```text
Conversion successful!
Input: sample.mp3
Output: sample.dfpwm
Encoded bytes: 1653750
```



## Project Status

DfpwmForge is under active development.

**Planned:**
- WAV format conversion support



## Building from Source

### Requirements

- CMake 3.20 or newer
- A C++17-compatible compiler
- Git

| Platform | Compiler |
|---|---|
| Windows | Microsoft Visual C++ (MSVC) |
| Linux | GCC or Clang |

### Clone the Repository

```bash
git clone https://github.com/USERNAME/DfpwmForge.git 
cd DfpwmForge
```

### Windows

Install [Visual Studio Build Tools](https://learn.microsoft.com/en-us/cpp/overview/acquire-msvc?view=msvc-170) with the C++ build tools and a Windows SDK.

Configure and build:

```powershell
cmake -S . -B build
cmake --build build --config Release
```

The executable will be generated at:

```text
build/bin/Debug/dfpwm.exe
```

### Linux

Install the required packages using your distribution's package manager.

**Fedora:**

```bash
sudo dnf install cmake gcc-c++ git
```

**Debian / Ubuntu:**

```bash
sudo apt install cmake g++ git
```

Configure and build:

```bash
cmake -S . -B build
cmake --build build --config Release
```

The executable will be generated at:

```text
build/bin/dfpwm
```



## VS Code Development

DfpwmForge includes shared VS Code configuration for Windows and Linux.

1. Install [Visual Studio Code](https://code.visualstudio.com/).
2. Install the C/C++ and CMake Tools extensions.
3. Clone and open the DfpwmForge project folder.
4. Allow CMake Tools to configure the project. Select **Let CMake decide** when prompted.
5. Press **F5** to build and run.



## Project Structure

```text
DfpwmForge/
├── .vscode/        # Shared VS Code configuration
├── external/       # Third-party dependencies
├── include/        # Project header files
├── src/            # C++ source files
├── CMakeLists.txt  # CMake build configuration
├── .gitignore
└── README.md
```



## License

DfpwmForge is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

Third-party components retain their respective licenses. `minimp3` is released under CC0-1.0.
