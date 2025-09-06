# C+ IDE Changelog

All notable changes to C+ IDE will be documented in this file.

## [2025.1.0] - 2025-09-06 "Foundation Release"

### Added

- Complete C+ IDE with real compilation capabilities
- g++ 6.3.0 integration with strict version verification
- GDB 7.6.1 debugging support with automated script generation
- Professional Windows UI with line numbers and resizable panels
- C+ to C++ translation engine (`func main()`, `printLine()`)
- File management system for .cplus files
- Real-time terminal output with compilation logs
- Professional menu system with keyboard shortcuts
- About dialog with version requirements display
- Complete documentation and installation guide
- MSI installer package with setup.exe
- Version-controlled toolchain enforcement

### Features

- **Compilation**: Real C+ code compilation and execution
- **Debugging**: Interactive debugging with breakpoints and variable inspection
- **Editor**: Syntax-aware editor with line numbers and auto-scrolling
- **UI**: Modern Windows interface with professional controls
- **Language**: C+ Language 0.1 specification support
- **Toolchain**: MinGW g++ 6.3.0 and GDB 7.6.1 integration

### Technical Details

- **Platform**: Windows 10/11
- **Framework**: Win32 Desktop API
- **Build System**: Visual Studio 2022 Preview MSBuild
- **Language**: C+ Language 0.1 with C++ interoperability
- **File Types**: .cplus source files, .exe executables

### Known Limitations

- Single file compilation only
- Manual save required (no auto-save)
- Windows platform only
- Requires exact toolchain versions

---

## Future Releases

### [2025.1.1] - Planned

- `.cplusproj` project file support
- Multi-file compilation
- Auto-save functionality
- Enhanced syntax highlighting
- Code completion features

### [2025.2.0] - Future Vision

- C+ Language 0.2 specification
- .CPlus framework (.NET integration)
- Plugin system architecture
- Cross-platform support (Linux, macOS)

---

For detailed release information, see RELEASE_NOTES_v1.0.md
