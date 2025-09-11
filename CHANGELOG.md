# C+ IDE Changelog

All notable changes to C+ IDE will be documented in this file.

## [2025.1.1] - 2025-09-06 "File Association & Project Explorer Update"

### New Features

- **File Association Support** - .cplus files automatically open with C+ IDE
- **Command-Line File Opening** - Launch with file path: `"C+ IDE.exe" "file.cplus"`
- **"Open With" Menu Integration** - C+ IDE appears in Windows "Open With" dialog
- **Context Menu Support** - "Edit with C+ IDE" option in right-click menu
- **Registry File** - `register_cplus_files.reg` for manual file association
- **Automatic Registration** - File associations registered on first IDE launch
- **Direct File Loading** - NO DELAY when opening files from Windows Explorer
- **Project Explorer** - Visual Studio-style project management system
- **Project Files (.cplusproj)** - Simple line-based format for organizing files
- **Tree View Interface** - Professional project structure display
- **Project Integration** - Seamless integration with File menu operations

### Technical Implementation

- Modified `wWinMain()` to parse command-line arguments for file paths
- Added `OpenFileDirectly()` function for programmatic file opening
- Added `RegisterFileAssociation()` function for Windows registry integration
- Integrated `SHChangeNotify()` to refresh Windows file associations
- Enhanced InitInstance to handle startup file loading automatically
- Implemented Project Explorer with TreeView control (25% width allocation)
- Added comprehensive project management functions (New, Open, Save, Add/Remove files)
- Integrated Common Controls library for TreeView functionality
- Enhanced WM_CREATE, WM_SIZE, and WM_NOTIFY message handlers for project UI

### Key Capabilities

- **Zero-Delay Opening**: Files open immediately when double-clicked in Explorer
- **Smart Path Handling**: Supports quoted and unquoted file paths in command line
- **Extension Validation**: Only .cplus files are processed from command line
- **Registry Integration**: Uses HKEY_CURRENT_USER for user-specific associations
- **Icon Association**: C+ IDE icon displays for .cplus files in Explorer
- **Project Management**: Create, open, save projects with automatic file tracking
- **Tree Navigation**: Double-click files in project to open, right-click for context menu
- **Auto-Update**: Projects automatically update when files are saved or opened

## [2025.1.0] - 2025-09-06 "Foundation Release"

### Core Features

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

### [2025.1.2] - Planned

- Enhanced syntax highlighting with C+ keyword support
- Code completion and IntelliSense features
- Auto-save functionality
- Advanced debugging features (watch variables, call stack)
- Project templates and wizards

### [2025.2.0] - Future Vision

- C+ Language 0.2 specification
- .CPlus framework (.NET integration)
- Plugin system architecture
- Cross-platform support (Linux, macOS)

---

For detailed release information, see RELEASE_NOTES_v1.0.md
