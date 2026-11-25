# DfpwmForge

**A lightweight, cross-platform MP3-to-DFPWM converter designed for CC:Tweaked.**

DfpwmForge is a native C++17 command-line application that converts MP3 audio into standard DFPWM format for playback through CC:Tweaked speaker peripherals.

## Project status

DfpwmForge is under active development.

**Next planned feature:** WAV-to-DFPWM conversion.


## Usage

DfpwmForge is a command-line application.

### Basic conversion

**Windows (PowerShell):**

```powershell
.\dfpwm.exe input.mp3
```

**Linux:**

```bash
./dfpwm input.mp3
```

The converted file is automatically saved alongside the input file using the same filename with a `.dfpwm` extension.

### Custom output path

Use `-o` to specify a custom output filename or path.

**Windows (PowerShell):**

```powershell
.\dfpwm.exe input.mp3 -o "$HOME\Desktop\converted.dfpwm"
```

**Linux:**

```bash
./dfpwm input.mp3 -o "$HOME/Desktop/converted.dfpwm"
```

DfpwmForge automatically appends `.dfpwm` when the specified output filename does not already have that extension.

### Paths containing spaces

Quote paths containing spaces.

**Linux:**

```bash
./dfpwm "$HOME/Desktop/Music/my audio.mp3"
```

**Windows (PowerShell):**

```powershell
.\dfpwm.exe "$HOME\Desktop\Music\my audio.mp3"
```

### Command syntax

```text
dfpwm <input_path.mp3> -o <output_path>
```

| Argument | Description |
|---|---|
| `input_path.mp3` | Path to the input MP3 file |
| `-o` | Optional flag for specifying a custom output path |
| `output_path` | Custom output filename or path |

## Conversion output

DfpwmForge displays the input filename, audio information, conversion progress, output path, and encoded file size.

Example:

```text
Input: sample.mp3
[1/4] Decoding MP3       [100%]

Audio: 05:00 | 44100 Hz | Stereo | 16-bit PCM

[2/4] Converting to Mono [100%]
[3/4] Resampling Audio  [100%]
[4/4] Encoding DFPWM    [100%]

Conversion successful!
Output: /path/to/sample.dfpwm
Size: 1,800,000 bytes
```

Progress percentages reflect actual processing progress.

## Building from source

### Requirements

- CMake 3.20 or newer.
- A C++17-compatible compiler.
- Git.

| Platform | Compiler |
|---|---|
| Windows | Microsoft Visual C++ (MSVC) |
| Linux | GCC or Clang |

### Clone the repository

```bash
git clone https://github.com/XEXModz/DfpwmForge
cd DfpwmForge
```

### Windows

Install [Visual Studio Build Tools](https://learn.microsoft.com/en-us/cpp/overview/acquire-msvc?view=msvc-170) with the C++ build tools and Windows SDK.

Configure and build:

```powershell
cmake -S . -B build
cmake --build build --config Release
```

### Linux

**Fedora:**

```bash
sudo dnf install cmake gcc-c++
```

**Debian / Ubuntu:**

```bash
sudo apt install cmake g++
```

Configure and build:

```bash
cmake -S . -B build
cmake --build build
```

The Linux executable is generated at:

```text
build/bin/dfpwm
```

### VS Code development

DfpwmForge includes shared VS Code configuration for Windows and Linux.

1. Install Visual Studio Code.
2. Install the C/C++ and CMake Tools extensions.
3. Clone and open the DfpwmForge project folder.
4. Allow CMake Tools to configure the project.
5. Press F5 to build and run.

## Project structure

```text
DfpwmForge/
├── .vscode/              # Shared VS Code configuration
├── external/             # Third-party dependencies
├── include/
│   ├── audio_data.h      # Shared audio data structure
│   ├── audio_processor.h
│   ├── dfpwm_encoder.h
│   └── mp3_decoder.h
├── src/
│   ├── audio_processor.cpp
│   ├── dfpwm_encoder.cpp
│   ├── main.cpp
│   └── mp3_decoder.cpp
├── CMakeLists.txt
├── .gitignore
├── LICENSE
└── README.md
```
## License

DfpwmForge is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

Third-party components retain their respective licenses. `minimp3` is released under CC0-1.0.
