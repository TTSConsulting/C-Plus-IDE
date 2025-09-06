# C+ IDE - Advanced C+ Development Environment

![C+ IDE Version](https://img.shields.io/badge/C%2B%20IDE-v1.0-blue)
![C+ Language](https://img.shields.io/badge/C%2B%20Language-v0.1-green)
![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey)
![License](https://img.shields.io/badge/License-Proprietary-red)

## Overview

C+ IDE is a professional, feature-rich integrated development environment specifically designed for the C+ programming language. Built with modern Windows APIs and featuring real-time compilation, debugging capabilities, and an intuitive user interface.

## Features

### 🎯 **Core Development Features**
- **Syntax-Aware Editor** with line numbers and auto-scrolling
- **Real-Time Compilation** with g++ 6.3.0 integration
- **Interactive Debugging** with GDB 7.6.1 support
- **File Management** for .cplus files (Create, Open, Save)
- **Terminal Output** with timestamped compilation results
- **Professional UI** with resizable panels and modern fonts

### 🔧 **C+ Language Support**
- **C+ to C++ Translation Engine**
  - `func main()` → `int main()` conversion
  - `printLine()` → `std::cout` translation
  - Automatic include statements injection
- **Real Code Execution** - Actually compiles and runs your C+ programs
- **Error Handling** with detailed compiler feedback

### 🛠️ **Development Tools**
- **Version-Controlled Compilation** (g++ 6.3.0 required)
- **Professional Debugging** (GDB 7.6.1 required)
- **Project Templates** with starter code
- **Multiple Output Formats** (GUI and CLI project support)

## System Requirements

### **Required Software**
| Component | Version | Purpose |
|-----------|---------|---------|
| **g++ Compiler** | `6.3.0` | C+ code compilation |
| **GDB Debugger** | `7.6.1` | Interactive debugging |
| **MinGW** | Latest | Windows GCC toolchain |
| **Windows** | 10/11 | Operating system |

### **Installation Prerequisites**
1. **MinGW with g++ 6.3.0**
   ```bash
   mingw-get install gcc g++
   ```

2. **GDB 7.6.1 Debugger**
   ```bash
   mingw-get install gdb
   ```

3. **Visual Studio Build Tools** (for IDE compilation)

## Quick Start

### 1. **Launch C+ IDE**
```bash
"C+ IDE.exe"
```

### 2. **Create Your First C+ Program**
- Press `Ctrl+N` or go to **File > New File**
- Save as `hello.cplus`
- Write your C+ code:

```cpp
// Hello World in C+
#include <iostream>
using namespace std;

func main() {
    printLine("Hello, C+ World!");
    return 0;
}
```

### 3. **Run Your Program**
- Press `F5` or go to **Run > Run**
- Watch real-time compilation and execution in terminal

### 4. **Debug Your Code**
- Press `F10` or go to **Run > Debug**
- Step through your code with GDB integration

## Keyboard Shortcuts

| Shortcut | Action | Description |
|----------|--------|-------------|
| `Ctrl+N` | New File | Create new .cplus file |
| `Ctrl+O` | Open File | Open existing .cplus file |
| `Ctrl+S` | Save File | Save current file |
| `F5` | Run | Compile and execute program |
| `F10` | Debug | Debug with GDB integration |
| `F6` | GUI Project | Create GUI project |
| `F9` | Open Project | Open .cplusproj file |
| `F11` | CLI Project | Create CLI project |

## Menu Reference

### **File Menu**
- **New File (Ctrl+N)** - Create new C+ source file
- **Open File (Ctrl+O)** - Open existing .cplus file
- **Save File (Ctrl+S)** - Save current file
- **Exit** - Close IDE

### **Make Menu**
- **GUI Project (F6)** - Create graphical application project
- **CLI Project (F11)** - Create command-line application project

### **Open Menu**
- **C+ Project (F9)** - Open .cplusproj project file

### **Run Menu**
- **Run (F5)** - Compile and execute current file
- **Debug (F10)** - Debug current file with GDB

## C+ Language Reference

### **Basic Syntax**
```cpp
// Function declaration
func main() {
    // C+ uses func instead of int for main
    return 0;
}

// Output function
printLine("Text to display");
// Automatically converts to: std::cout << "Text to display" << std::endl;
```

### **File Extensions**
- **Source Files**: `.cplus`
- **Project Files**: `.cplusproj` (planned)

## Architecture

### **Component Overview**
```
C+ IDE
├── Win32 Application Framework
├── Text Editor with Line Numbers
├── Terminal Output Window
├── C+ Language Translator
├── g++ Compiler Integration
├── GDB Debugger Interface
└── File Management System
```

### **Translation Process**
1. **Parse** C+ source code
2. **Translate** C+ syntax to C++
3. **Verify** compiler version (g++ 6.3.0)
4. **Compile** with debug symbols
5. **Execute** or **Debug** as requested

## Version History

### **Version 1.0** (Current)
- ✅ Complete C+ IDE implementation
- ✅ Real compilation and execution
- ✅ GDB debugging integration
- ✅ Professional UI with line numbers
- ✅ Version-controlled toolchain
- ✅ File management system
- ✅ Terminal output with timestamps

### **C+ Language 0.1** (Current)
- ✅ `func main()` syntax
- ✅ `printLine()` function
- ✅ C++ interoperability
- ✅ Standard library integration

## Technical Specifications

### **Compiler Integration**
- **Target Compiler**: g++ (MinGW) 6.3.0
- **Compilation Flags**: Standard C++ with debug symbols (`-g`)
- **Output Format**: Windows PE executable
- **Error Handling**: Real-time compiler output capture

### **Debugger Integration**
- **Target Debugger**: GDB 7.6.1
- **Debug Features**: Breakpoints, variable inspection, step execution
- **Automation**: Script-based debugging sessions
- **Output**: Real-time debug session capture

### **File Handling**
- **Encoding**: UTF-8 with Windows line endings
- **Auto-save**: Manual save required
- **Backup**: None (user responsible)
- **Project Files**: Support planned for .cplusproj

## Troubleshooting

### **Common Issues**

#### **"g++ version 6.3.0 is required!"**
**Solution**: Install correct g++ version
```bash
mingw-get install gcc=6.3.0
```

#### **"GDB version 7.6.1 is required for debugging!"**
**Solution**: Install correct GDB version
```bash
mingw-get install gdb=7.6.1
```

#### **Compilation Fails**
**Possible Causes**:
- MinGW not in PATH
- Wrong compiler version
- Invalid C+ syntax

**Solution**: Check About dialog for requirements

### **Getting Help**
1. Check **Help > About** for version requirements
2. Verify PATH includes `C:\MinGW\bin`
3. Test compiler: `g++ --version`
4. Test debugger: `gdb --version`

## Development

### **Building from Source**
```bash
# Requirements
- Visual Studio 2022 Preview
- Windows SDK
- MSBuild tools

# Build command
msbuild "C+ IDE.sln" /p:Configuration=Debug /p:Platform=x64
```

### **Project Structure**
```
C+ IDE/
├── C+ IDE.cpp          # Main application source
├── C+ IDE.h           # Header definitions
├── C+ IDE.rc          # Resource file (dialogs, menus)
├── framework.h        # Windows framework headers
├── resource.h         # Resource constants
├── C+ IDE.ico         # Application icon
└── README.md          # This file
```

## License

**Proprietary Software**  
Copyright (c) 2025 TTSConsulting  
All rights reserved.

## Contact

**Repository**: [C-Plus-IDE](https://github.com/TTSConsulting/C-Plus-IDE)  
**Organization**: TTSConsulting  

---

**C+ IDE v1.0** - *Advanced C+ Development Environment*  
*Built for professional C+ development on Windows*