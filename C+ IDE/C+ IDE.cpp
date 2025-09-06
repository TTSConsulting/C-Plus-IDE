// C+ IDE.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "C+ IDE.h"
#include <commdlg.h>
#include <string.h>
#include <string>
#include <process.h>
#include <io.h>
#include <fcntl.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND hEdit = NULL; // Add this at the top of your file
HWND hLineNumbers = NULL; // Line numbers control
HWND hTerminal = NULL; // Terminal/Compiler output control

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void                UpdateLineNumbers();
void                AppendToTerminal(const WCHAR* text);
void                CompileAndRun();
void                CompileAndDebug();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CIDE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CIDE));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CIDE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CIDE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            // Check if it's the editor content changing
            if (HIWORD(wParam) == EN_CHANGE && LOWORD(wParam) == 1)
            {
                UpdateLineNumbers();
                return 0;
            }
            
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case ID_FILE_NEWFILE:
                {
                    // Add shortcut feedback to terminal
                    AppendToTerminal(L"File > New File (Ctrl+N)");
                    
                    // Handle new .cplus file creation
                    OPENFILENAME ofn;
                    WCHAR szFile[260] = L"";
                    
                    // Initialize OPENFILENAME
                    ZeroMemory(&ofn, sizeof(ofn));
                    ofn.lStructSize = sizeof(ofn);
                    ofn.hwndOwner = hWnd;
                    ofn.lpstrFile = szFile;
                    ofn.nMaxFile = sizeof(szFile);
                    ofn.lpstrFilter = L"C+ Files\0*.cplus\0All Files\0*.*\0";
                    ofn.nFilterIndex = 1;
                    ofn.lpstrFileTitle = NULL;
                    ofn.nMaxFileTitle = 0;
                    ofn.lpstrInitialDir = NULL;
                    ofn.lpstrDefExt = L"cplus";
                    ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
                    
                    // Display the Save dialog box
                    if (GetSaveFileName(&ofn) == TRUE)
                    {
                        // Create the new .cplus file
                        HANDLE hFile = CreateFile(ofn.lpstrFile,
                            GENERIC_WRITE,
                            0,
                            NULL,
                            CREATE_ALWAYS,
                            FILE_ATTRIBUTE_NORMAL,
                            NULL);
                            
                        if (hFile != INVALID_HANDLE_VALUE)
                        {
                            // Write initial C+ template content to the file
                            const char* templateContent = 
                                "// New C+ File\r\n"
                                "// Created with C+ IDE\r\n\r\n"
                                "#include <iostream>\r\n"
                                "using namespace std;\r\n\r\n"
                                "func main() {\r\n"
                                "\r\n"
                                "    return 0;\r\n"
                                "}\r\n";
                                
                            DWORD bytesWritten;
                            WriteFile(hFile, templateContent, (DWORD)strlen(templateContent), &bytesWritten, NULL);
                            CloseHandle(hFile);
                            
                            // Add to terminal output
                            WCHAR terminalMsg[512];
                            swprintf_s(terminalMsg, L"Created new C+ file: %s", wcsrchr(ofn.lpstrFile, L'\\') ? wcsrchr(ofn.lpstrFile, L'\\') + 1 : ofn.lpstrFile);
                            AppendToTerminal(terminalMsg);
                            
                            // Show success message
                            MessageBox(hWnd, L"New C+ file created successfully!", L"Success", MB_OK | MB_ICONINFORMATION);
                        }
                        else
                        {
                            MessageBox(hWnd, L"Failed to create file!", L"Error", MB_OK | MB_ICONERROR);
                        }
                    }
                }
                break;
            case ID_FILE_OPENFILE:
                {
                    // Add shortcut feedback to terminal
                    AppendToTerminal(L"File > Open File (Ctrl+O)");
                    
                    // Handle opening a .cplus file
                    OPENFILENAME ofn;
                    WCHAR szFile[260] = L"";
                    
                    // Initialize OPENFILENAME
                    ZeroMemory(&ofn, sizeof(ofn));
                    ofn.lStructSize = sizeof(ofn);
                    ofn.hwndOwner = hWnd;
                    ofn.lpstrFile = szFile;
                    ofn.nMaxFile = sizeof(szFile);
                    ofn.lpstrFilter = L"C+ Files\0*.cplus\0All Files\0*.*\0";
                    ofn.nFilterIndex = 1;
                    ofn.lpstrFileTitle = NULL;
                    ofn.nMaxFileTitle = 0;
                    ofn.lpstrInitialDir = NULL;
                    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
                    
                    // Display the Open dialog box
                    if (GetOpenFileName(&ofn) == TRUE)
                    {
                        // Open and read the file
                        HANDLE hFile = CreateFile(ofn.lpstrFile,
                            GENERIC_READ,
                            FILE_SHARE_READ,
                            NULL,
                            OPEN_EXISTING,
                            FILE_ATTRIBUTE_NORMAL,
                            NULL);
                            
                        if (hFile != INVALID_HANDLE_VALUE)
                        {
                            // Get file size
                            DWORD fileSize = GetFileSize(hFile, NULL);
                            if (fileSize != INVALID_FILE_SIZE)
                            {
                                // Allocate buffer for file content
                                char* buffer = new char[fileSize + 1];
                                DWORD bytesRead;
                                
                                // Read file content
                                if (ReadFile(hFile, buffer, fileSize, &bytesRead, NULL))
                                {
                                    buffer[bytesRead] = '\0'; // Null terminate
                                    
                                    // Normalize line endings - convert \n to \r\n for Windows Edit control
                                    std::string content(buffer);
                                    std::string normalized = "";
                                    
                                    for (size_t i = 0; i < content.length(); i++)
                                    {
                                        if (content[i] == '\n' && (i == 0 || content[i-1] != '\r'))
                                        {
                                            normalized += "\r\n"; // Add Windows line ending
                                        }
                                        else if (content[i] != '\r' || (i+1 < content.length() && content[i+1] != '\n'))
                                        {
                                            normalized += content[i];
                                        }
                                        else if (content[i] == '\r' && i+1 < content.length() && content[i+1] == '\n')
                                        {
                                            normalized += "\r\n"; // Keep existing Windows line ending
                                            i++; // Skip the \n since we're adding both
                                        }
                                    }
                                    
                                    // Convert normalized string to wide string for the edit control
                                    int wideSize = MultiByteToWideChar(CP_UTF8, 0, normalized.c_str(), -1, NULL, 0);
                                    WCHAR* wideBuffer = new WCHAR[wideSize];
                                    MultiByteToWideChar(CP_UTF8, 0, normalized.c_str(), -1, wideBuffer, wideSize);
                                    
                                    // Set the content in the edit control
                                    SetWindowText(hEdit, wideBuffer);
                                    
                                    // Update line numbers
                                    UpdateLineNumbers();
                                    
                                    // Update window title to show filename
                                    WCHAR title[512];
                                    swprintf_s(title, L"C+ IDE - %s", wcsrchr(ofn.lpstrFile, L'\\') ? wcsrchr(ofn.lpstrFile, L'\\') + 1 : ofn.lpstrFile);
                                    SetWindowText(hWnd, title);
                                    
                                    // Add to terminal output
                                    WCHAR terminalMsg[512];
                                    swprintf_s(terminalMsg, L"Opened file: %s (%lld lines)", wcsrchr(ofn.lpstrFile, L'\\') ? wcsrchr(ofn.lpstrFile, L'\\') + 1 : ofn.lpstrFile, SendMessage(hEdit, EM_GETLINECOUNT, 0, 0));
                                    AppendToTerminal(terminalMsg);
                                    
                                    delete[] wideBuffer;
                                }
                                else
                                {
                                    MessageBox(hWnd, L"Failed to read file!", L"Error", MB_OK | MB_ICONERROR);
                                }
                                
                                delete[] buffer;
                            }
                            CloseHandle(hFile);
                        }
                        else
                        {
                            MessageBox(hWnd, L"Failed to open file!", L"Error", MB_OK | MB_ICONERROR);
                        }
                    }
                }
                break;
            case ID_FILE_SAVEFILE:
                // Add shortcut feedback to terminal
                AppendToTerminal(L"File > Save File (Ctrl+S)");
                
                // Handle saving a .cplus file
                {
                    OPENFILENAME ofn;
                    WCHAR szFile[260] = L"";
                    
                    // Initialize OPENFILENAME
                    ZeroMemory(&ofn, sizeof(ofn));
                    ofn.lStructSize = sizeof(ofn);
                    ofn.hwndOwner = hWnd;
                    ofn.lpstrFile = szFile;
                    ofn.nMaxFile = sizeof(szFile);
                    ofn.lpstrFilter = L"C+ Files\0*.cplus\0All Files\0*.*\0";
                    ofn.nFilterIndex = 1;
                    ofn.lpstrFileTitle = NULL;
                    ofn.nMaxFileTitle = 0;
                    ofn.lpstrInitialDir = NULL;
                    ofn.lpstrDefExt = L"cplus";
                    ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
                    
                    // Display the Save dialog box
                    if (GetSaveFileName(&ofn) == TRUE)
                    {
                        // Get content from the edit control
                        int length = GetWindowTextLength(hEdit);
                        char* buffer = new char[length + 1];
                        GetWindowTextA(hEdit, buffer, length + 1);
                        
                        // Create or overwrite the .cplus file
                        HANDLE hFile = CreateFile(ofn.lpstrFile,
                            GENERIC_WRITE,
                            0,
                            NULL,
                            CREATE_ALWAYS,
                            FILE_ATTRIBUTE_NORMAL,
                            NULL);
                            
                        if (hFile != INVALID_HANDLE_VALUE)
                        {
                            DWORD bytesWritten;
                            WriteFile(hFile, buffer, (DWORD)strlen(buffer), &bytesWritten, NULL);
                            CloseHandle(hFile);
                            
                            // Add to terminal output
                            WCHAR terminalMsg[512];
                            swprintf_s(terminalMsg, L"Saved file: %s (%d bytes)", wcsrchr(ofn.lpstrFile, L'\\') ? wcsrchr(ofn.lpstrFile, L'\\') + 1 : ofn.lpstrFile, bytesWritten);
                            AppendToTerminal(terminalMsg);
                            
                            MessageBox(hWnd, L"File saved successfully!", L"Success", MB_OK | MB_ICONINFORMATION);
                        }
                        else
                        {
                            MessageBox(hWnd, L"Failed to save file!", L"Error", MB_OK | MB_ICONERROR);
                        }
                        
                        delete[] buffer;
                    }
                }
                break;
            case ID_MAKE_MAKE:
                // Add shortcut feedback to terminal
                AppendToTerminal(L"Make > GUI Project (F6)");
                // Handle making a GUI project
                break;
            case ID_MAKE_MAKE32778:
                // Add shortcut feedback to terminal
                AppendToTerminal(L"Make > CLI Project (F11)");
                // Handle making a CLI project
                break;
            case ID__OPEN:
                // Add shortcut feedback to terminal
                AppendToTerminal(L"Open > C+ Project (F9)");
                // Handle opening a .cplus project (.cplusproj)
                break;
            case ID_RUN_RUN:
                // Add shortcut feedback to terminal
                AppendToTerminal(L"Run > Run (F5)");
                // Handle running the current C+ file
                {
                    // Get the current file content to check if there's anything to run
                    int length = GetWindowTextLength(hEdit);
                    if (length > 0) {
                        AppendToTerminal(L"Compiling and running C+ program...");
                        CompileAndRun();
                    } else {
                        AppendToTerminal(L"Error: No code to run. Please open or create a C+ file first.");
                    }
                }
                break;
            case ID_RUN_DEBUG:
                // Add shortcut feedback to terminal
                AppendToTerminal(L"Run > Debug (F10)");
                // Handle debugging the current C+ file
                {
                    int length = GetWindowTextLength(hEdit);
                    if (length > 0) {
                        AppendToTerminal(L"Compiling and debugging C+ program...");
                        CompileAndDebug();
                    } else {
                        AppendToTerminal(L"Error: No code to debug. Please open or create a C+ file first.");
                    }
                }
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_CREATE:
        {
            // Create line numbers control
            hLineNumbers = CreateWindowEx(
                WS_EX_CLIENTEDGE, L"STATIC", L"",
                WS_CHILD | WS_VISIBLE | SS_LEFT,
                10, 10, 50, 400, // Reduced height to make room for terminal
                hWnd, (HMENU)2, hInst, NULL);
                
            // Create main text editor
            hEdit = CreateWindowEx(
                WS_EX_CLIENTEDGE, L"EDIT", L"",
                WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
                65, 10, 735, 400, // Reduced height to make room for terminal
                hWnd, (HMENU)1, hInst, NULL);
                
            // Create terminal/compiler output window
            hTerminal = CreateWindowEx(
                WS_EX_CLIENTEDGE, L"EDIT", L"",
                WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_READONLY,
                10, 420, 790, 180, // Bottom section for terminal output
                hWnd, (HMENU)3, hInst, NULL);
                
            // Set a monospace font for all controls
            HFONT hFont = CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY, FIXED_PITCH | FF_MODERN, L"Consolas");
            SendMessage(hEdit, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hLineNumbers, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hTerminal, WM_SETFONT, (WPARAM)hFont, TRUE);
            
            // Set terminal background to black and text to white for terminal look
            SetWindowText(hTerminal, L"C+ IDE Terminal Output\r\nReady for compilation...\r\n");
            
            UpdateLineNumbers();
        }
        break;
    case WM_SIZE:
        {
            if (hLineNumbers && hEdit && hTerminal)
            {
                int width = LOWORD(lParam);
                int height = HIWORD(lParam);
                
                // Calculate heights: 70% for editor area, 30% for terminal
                int editorHeight = (int)(height * 0.7) - 20;
                int terminalHeight = (int)(height * 0.3) - 10;
                int terminalTop = editorHeight + 25;
                
                // Resize line numbers control
                MoveWindow(hLineNumbers, 10, 10, 50, editorHeight, TRUE);
                
                // Resize editor control (offset by line numbers width)
                MoveWindow(hEdit, 65, 10, width - 75, editorHeight, TRUE);
                
                // Resize terminal control (full width at bottom)
                MoveWindow(hTerminal, 10, terminalTop, width - 20, terminalHeight, TRUE);
                
                UpdateLineNumbers();
            }
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

// Function to update line numbers
void UpdateLineNumbers()
{
    if (!hEdit || !hLineNumbers) return;
    
    // Get the number of lines in the editor
    int lineCount = (int)SendMessage(hEdit, EM_GETLINECOUNT, 0, 0);
    
    // Build line numbers string
    WCHAR lineNumbersText[10000] = L""; // Buffer for line numbers
    WCHAR tempLine[10];
    
    for (int i = 1; i <= lineCount; i++)
    {
        swprintf_s(tempLine, L"%d\r\n", i);
        wcscat_s(lineNumbersText, tempLine);
    }
    
    // Set the line numbers text
    SetWindowText(hLineNumbers, lineNumbersText);
}

// Function to append text to terminal output
void AppendToTerminal(const WCHAR* text)
{
    if (!hTerminal) return;
    
    // Get current content length
    int length = GetWindowTextLength(hTerminal);
    
    // Set selection to end of text
    SendMessage(hTerminal, EM_SETSEL, length, length);
    
    // Add timestamp
    SYSTEMTIME st;
    GetLocalTime(&st);
    WCHAR timestamp[100];
    swprintf_s(timestamp, L"[%02d:%02d:%02d] ", st.wHour, st.wMinute, st.wSecond);
    
    // Append timestamp and text
    SendMessage(hTerminal, EM_REPLACESEL, FALSE, (LPARAM)timestamp);
    SendMessage(hTerminal, EM_REPLACESEL, FALSE, (LPARAM)text);
    SendMessage(hTerminal, EM_REPLACESEL, FALSE, (LPARAM)L"\r\n");
    
    // Scroll to bottom
    SendMessage(hTerminal, EM_SETSEL, GetWindowTextLength(hTerminal), GetWindowTextLength(hTerminal));
    SendMessage(hTerminal, EM_SCROLLCARET, 0, 0);
}

// Function to compile and run C+ code
void CompileAndRun()
{
    if (!hEdit) return;
    
    // Get the code from the editor
    int length = GetWindowTextLength(hEdit);
    if (length == 0) {
        AppendToTerminal(L"Error: No code to compile");
        return;
    }
    
    // Get the text content
    WCHAR* codeBuffer = new WCHAR[length + 1];
    GetWindowText(hEdit, codeBuffer, length + 1);
    
    // Create temporary files
    WCHAR tempDir[MAX_PATH];
    GetTempPath(MAX_PATH, tempDir);
    
    WCHAR tempCppFile[MAX_PATH];
    WCHAR tempExeFile[MAX_PATH];
    swprintf_s(tempCppFile, L"%stemp_cplus.cpp", tempDir);
    swprintf_s(tempExeFile, L"%stemp_cplus.exe", tempDir);
    
    AppendToTerminal(L"Creating temporary files...");
    
    // Convert C+ code to C++ code (basic translation)
    std::wstring cppCode(codeBuffer);
    
    // Replace C+ specific syntax with C++ equivalents
    // Replace "func" with "int" for main function
    size_t pos = cppCode.find(L"func main()");
    if (pos != std::wstring::npos) {
        cppCode.replace(pos, 11, L"int main()");
    }
    
    // Replace printLine() calls with std::cout statements
    pos = 0;
    while ((pos = cppCode.find(L"printLine(", pos)) != std::wstring::npos) {
        // Find the matching closing parenthesis
        size_t openParen = pos + 10; // Length of "printLine("
        size_t closeParen = openParen;
        int parenCount = 1;
        
        while (closeParen < cppCode.length() && parenCount > 0) {
            if (cppCode[closeParen] == L'(') parenCount++;
            else if (cppCode[closeParen] == L')') parenCount--;
            closeParen++;
        }
        
        if (parenCount == 0) {
            // Extract the content between parentheses
            std::wstring content = cppCode.substr(openParen, closeParen - openParen - 1);
            
            // Replace printLine(content) with std::cout << content << std::endl
            std::wstring replacement = L"std::cout << " + content + L" << std::endl";
            cppCode.replace(pos, closeParen - pos, replacement);
            
            // Move position forward to avoid infinite loop
            pos += replacement.length();
        } else {
            // If we can't find matching parenthesis, skip this occurrence
            pos += 10;
        }
    }
    
    // Write the C++ code to temporary file
    HANDLE hFile = CreateFile(tempCppFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        AppendToTerminal(L"Error: Could not create temporary file");
        delete[] codeBuffer;
        return;
    }
    
    // Convert wide string to multibyte for file writing
    int mbLength = WideCharToMultiByte(CP_UTF8, 0, cppCode.c_str(), -1, NULL, 0, NULL, NULL);
    char* mbBuffer = new char[mbLength];
    WideCharToMultiByte(CP_UTF8, 0, cppCode.c_str(), -1, mbBuffer, mbLength, NULL, NULL);
    
    DWORD bytesWritten;
    WriteFile(hFile, mbBuffer, (DWORD)strlen(mbBuffer), &bytesWritten, NULL);
    CloseHandle(hFile);
    delete[] mbBuffer;
    
    AppendToTerminal(L"Verifying g++ compiler version...");
    
    // Add MinGW to PATH if it exists
    WCHAR currentPath[4096];
    GetEnvironmentVariable(L"PATH", currentPath, 4096);
    
    // Check if MinGW is in common locations and add to PATH
    WCHAR mingwPaths[] = L"C:\\MinGW\\bin;C:\\msys64\\mingw64\\bin;C:\\mingw64\\bin;";
    WCHAR newPath[8192];
    swprintf_s(newPath, L"%s%s", mingwPaths, currentPath);
    SetEnvironmentVariable(L"PATH", newPath);
    
    // Check g++ version requirement (must be 6.3.0)
    WCHAR versionCmd[512];
    swprintf_s(versionCmd, L"\"C:\\MinGW\\bin\\g++.exe\" --version");
    
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;
    
    HANDLE hVersionReadPipe, hVersionWritePipe;
    if (CreatePipe(&hVersionReadPipe, &hVersionWritePipe, &sa, 0)) {
        STARTUPINFO versionSi;
        PROCESS_INFORMATION versionPi;
        ZeroMemory(&versionSi, sizeof(versionSi));
        versionSi.cb = sizeof(versionSi);
        versionSi.hStdError = hVersionWritePipe;
        versionSi.hStdOutput = hVersionWritePipe;
        versionSi.dwFlags |= STARTF_USESTDHANDLES;
        
        ZeroMemory(&versionPi, sizeof(versionPi));
        
        if (CreateProcess(NULL, versionCmd, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &versionSi, &versionPi)) {
            CloseHandle(hVersionWritePipe);
            
            // Wait for version check to finish
            WaitForSingleObject(versionPi.hProcess, 5000); // 5 second timeout
            
            // Read version output
            char versionBuffer[1024];
            DWORD versionBytesRead;
            std::string versionOutput;
            
            while (ReadFile(hVersionReadPipe, versionBuffer, sizeof(versionBuffer) - 1, &versionBytesRead, NULL) && versionBytesRead > 0) {
                versionBuffer[versionBytesRead] = '\0';
                versionOutput += versionBuffer;
            }
            
            CloseHandle(hVersionReadPipe);
            CloseHandle(versionPi.hProcess);
            CloseHandle(versionPi.hThread);
            
            // Check if the version contains "6.3.0"
            if (versionOutput.find("6.3.0") == std::string::npos) {
                AppendToTerminal(L"Error: g++ version 6.3.0 is required!");
                AppendToTerminal(L"Current compiler version does not match requirement.");
                
                // Convert version output to wide string and display
                int wideLen = MultiByteToWideChar(CP_UTF8, 0, versionOutput.c_str(), -1, NULL, 0);
                WCHAR* wideVersion = new WCHAR[wideLen];
                MultiByteToWideChar(CP_UTF8, 0, versionOutput.c_str(), -1, wideVersion, wideLen);
                AppendToTerminal(wideVersion);
                delete[] wideVersion;
                
                delete[] codeBuffer;
                return;
            } else {
                AppendToTerminal(L"✓ g++ 6.3.0 verified - proceeding with compilation...");
            }
        } else {
            CloseHandle(hVersionWritePipe);
            CloseHandle(hVersionReadPipe);
            AppendToTerminal(L"Error: Could not check g++ version.");
            delete[] codeBuffer;
            return;
        }
    }
    
    AppendToTerminal(L"Compiling with g++ 6.3.0...");
    
    // Try to compile the code using g++ first, then gcc as fallback
    WCHAR compileCmd[1024];
    WCHAR gccFallbackCmd[1024];
    
    // First try g++ with full path (removed 2>&1 since we're using pipes)
    swprintf_s(compileCmd, L"\"C:\\MinGW\\bin\\g++.exe\" \"%s\" -o \"%s\"", tempCppFile, tempExeFile);
    
    // If g++ fails, try gcc with C++ flags and full path
    swprintf_s(gccFallbackCmd, L"\"C:\\MinGW\\bin\\gcc.exe\" -x c++ -lstdc++ \"%s\" -o \"%s\"", tempCppFile, tempExeFile);
    
    HANDLE hReadPipe, hWritePipe;
    if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0)) {
        AppendToTerminal(L"Error: Could not create pipe for compiler output");
        delete[] codeBuffer;
        return;
    }
    
    // Set up process information
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.hStdError = hWritePipe;
    si.hStdOutput = hWritePipe;
    si.dwFlags |= STARTF_USESTDHANDLES;
    
    ZeroMemory(&pi, sizeof(pi));
    
    // Start the compiler process
    BOOL compilerStarted = FALSE;
    WCHAR* currentCmd = compileCmd;
    
    // Try g++ first
    if (CreateProcess(NULL, compileCmd, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        compilerStarted = TRUE;
        AppendToTerminal(L"Using g++ compiler...");
    } else {
        // If g++ fails, try gcc with C++ flags
        AppendToTerminal(L"g++ not found, trying gcc with C++ support...");
        ZeroMemory(&pi, sizeof(pi)); // Reset process info
        if (CreateProcess(NULL, gccFallbackCmd, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
            compilerStarted = TRUE;
            currentCmd = gccFallbackCmd;
            AppendToTerminal(L"Using gcc compiler with C++ flags...");
        }
    }
    
    if (compilerStarted) {
        CloseHandle(hWritePipe);
        
        // Wait for compilation to finish
        WaitForSingleObject(pi.hProcess, 10000); // 10 second timeout
        
        // Check if compilation was successful
        DWORD exitCode;
        GetExitCodeProcess(pi.hProcess, &exitCode);
        
        // Read compiler output
        char buffer[4096];
        DWORD bytesRead;
        std::string compilerOutput;
        
        while (ReadFile(hReadPipe, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
            buffer[bytesRead] = '\0';
            compilerOutput += buffer;
        }
        
        CloseHandle(hReadPipe);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        
        if (exitCode == 0) {
            AppendToTerminal(L"Compilation successful!");
            
            // Run the executable
            AppendToTerminal(L"Running program...");
            AppendToTerminal(L"--- Program Output ---");
            
            // Execute the compiled program
            STARTUPINFO runSi;
            PROCESS_INFORMATION runPi;
            ZeroMemory(&runSi, sizeof(runSi));
            runSi.cb = sizeof(runSi);
            
            // Create new pipes for program output
            HANDLE hRunReadPipe, hRunWritePipe;
            if (CreatePipe(&hRunReadPipe, &hRunWritePipe, &sa, 0)) {
                runSi.hStdOutput = hRunWritePipe;
                runSi.hStdError = hRunWritePipe;
                runSi.dwFlags |= STARTF_USESTDHANDLES;
                
                ZeroMemory(&runPi, sizeof(runPi));
                
                if (CreateProcess(tempExeFile, NULL, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &runSi, &runPi)) {
                    CloseHandle(hRunWritePipe);
                    
                    // Wait for program to finish
                    WaitForSingleObject(runPi.hProcess, 30000); // 30 second timeout
                    
                    // Read program output
                    char runBuffer[4096];
                    DWORD runBytesRead;
                    
                    while (ReadFile(hRunReadPipe, runBuffer, sizeof(runBuffer) - 1, &runBytesRead, NULL) && runBytesRead > 0) {
                        runBuffer[runBytesRead] = '\0';
                        
                        // Convert to wide string and display
                        int wideLen = MultiByteToWideChar(CP_UTF8, 0, runBuffer, -1, NULL, 0);
                        WCHAR* wideOutput = new WCHAR[wideLen];
                        MultiByteToWideChar(CP_UTF8, 0, runBuffer, -1, wideOutput, wideLen);
                        
                        // Remove the timestamp from program output
                        if (hTerminal) {
                            int termLength = GetWindowTextLength(hTerminal);
                            SendMessage(hTerminal, EM_SETSEL, termLength, termLength);
                            SendMessage(hTerminal, EM_REPLACESEL, FALSE, (LPARAM)wideOutput);
                        }
                        
                        delete[] wideOutput;
                    }
                    
                    CloseHandle(hRunReadPipe);
                    CloseHandle(runPi.hProcess);
                    CloseHandle(runPi.hThread);
                    
                    AppendToTerminal(L"--- End Program Output ---");
                    AppendToTerminal(L"Program execution completed.");
                } else {
                    CloseHandle(hRunWritePipe);
                    CloseHandle(hRunReadPipe);
                    AppendToTerminal(L"Error: Could not run the compiled program");
                }
            }
        } else {
            AppendToTerminal(L"Compilation failed!");
            
            // Show compiler errors
            if (!compilerOutput.empty()) {
                int wideLen = MultiByteToWideChar(CP_UTF8, 0, compilerOutput.c_str(), -1, NULL, 0);
                WCHAR* wideError = new WCHAR[wideLen];
                MultiByteToWideChar(CP_UTF8, 0, compilerOutput.c_str(), -1, wideError, wideLen);
                AppendToTerminal(wideError);
                delete[] wideError;
            }
        }
    } else {
        CloseHandle(hWritePipe);
        CloseHandle(hReadPipe);
        AppendToTerminal(L"Error: Could not start compiler. Make sure g++ is installed and in PATH.");
        AppendToTerminal(L"Install MinGW-w64 or Visual Studio Build Tools to compile C+ code.");
    }
    
    // Clean up temporary files
    DeleteFile(tempCppFile);
    DeleteFile(tempExeFile);
    
    delete[] codeBuffer;
}

// Function to compile and debug C+ code
void CompileAndDebug()
{
    if (!hEdit) return;
    
    // Get the code from the editor
    int length = GetWindowTextLength(hEdit);
    if (length == 0) {
        AppendToTerminal(L"Error: No code to debug");
        return;
    }
    
    // Get the text content
    WCHAR* codeBuffer = new WCHAR[length + 1];
    GetWindowText(hEdit, codeBuffer, length + 1);
    
    // Create temporary files
    WCHAR tempDir[MAX_PATH];
    GetTempPath(MAX_PATH, tempDir);
    
    WCHAR tempCppFile[MAX_PATH];
    WCHAR tempExeFile[MAX_PATH];
    swprintf_s(tempCppFile, L"%stemp_cplus_debug.cpp", tempDir);
    swprintf_s(tempExeFile, L"%stemp_cplus_debug.exe", tempDir);
    
    AppendToTerminal(L"Creating temporary files for debugging...");
    
    // Convert C+ code to C++ code (same translation as CompileAndRun)
    std::wstring cppCode(codeBuffer);
    
    // Replace C+ specific syntax with C++ equivalents
    // Replace "func" with "int" for main function
    size_t pos = cppCode.find(L"func main()");
    if (pos != std::wstring::npos) {
        cppCode.replace(pos, 11, L"int main()");
    }
    
    // Replace printLine() calls with std::cout statements
    pos = 0;
    while ((pos = cppCode.find(L"printLine(", pos)) != std::wstring::npos) {
        // Find the matching closing parenthesis
        size_t openParen = pos + 10; // Length of "printLine("
        size_t closeParen = openParen;
        int parenCount = 1;
        
        while (closeParen < cppCode.length() && parenCount > 0) {
            if (cppCode[closeParen] == L'(') parenCount++;
            else if (cppCode[closeParen] == L')') parenCount--;
            closeParen++;
        }
        
        if (parenCount == 0) {
            // Extract the content between parentheses
            std::wstring content = cppCode.substr(openParen, closeParen - openParen - 1);
            
            // Replace printLine(content) with std::cout << content << std::endl
            std::wstring replacement = L"std::cout << " + content + L" << std::endl";
            cppCode.replace(pos, closeParen - pos, replacement);
            
            // Move position forward to avoid infinite loop
            pos += replacement.length();
        } else {
            // If we can't find matching parenthesis, skip this occurrence
            pos += 10;
        }
    }
    
    // Write the C++ code to temporary file
    HANDLE hFile = CreateFile(tempCppFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        AppendToTerminal(L"Error: Could not create temporary debug file");
        delete[] codeBuffer;
        return;
    }
    
    // Convert wide string to multibyte for file writing
    int mbLength = WideCharToMultiByte(CP_UTF8, 0, cppCode.c_str(), -1, NULL, 0, NULL, NULL);
    char* mbBuffer = new char[mbLength];
    WideCharToMultiByte(CP_UTF8, 0, cppCode.c_str(), -1, mbBuffer, mbLength, NULL, NULL);
    
    DWORD bytesWritten;
    WriteFile(hFile, mbBuffer, (DWORD)strlen(mbBuffer), &bytesWritten, NULL);
    CloseHandle(hFile);
    delete[] mbBuffer;
    
    AppendToTerminal(L"Verifying g++ compiler version for debug build...");
    
    // Add MinGW to PATH if it exists
    WCHAR currentPath[4096];
    GetEnvironmentVariable(L"PATH", currentPath, 4096);
    
    // Check if MinGW is in common locations and add to PATH
    WCHAR mingwPaths[] = L"C:\\MinGW\\bin;C:\\msys64\\mingw64\\bin;C:\\mingw64\\bin;";
    WCHAR newPath[8192];
    swprintf_s(newPath, L"%s%s", mingwPaths, currentPath);
    SetEnvironmentVariable(L"PATH", newPath);
    
    // Check g++ version requirement (must be 6.3.0)
    WCHAR versionCmd[512];
    swprintf_s(versionCmd, L"\"C:\\MinGW\\bin\\g++.exe\" --version");
    
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;
    
    HANDLE hVersionReadPipe, hVersionWritePipe;
    if (CreatePipe(&hVersionReadPipe, &hVersionWritePipe, &sa, 0)) {
        STARTUPINFO versionSi;
        PROCESS_INFORMATION versionPi;
        ZeroMemory(&versionSi, sizeof(versionSi));
        versionSi.cb = sizeof(versionSi);
        versionSi.hStdError = hVersionWritePipe;
        versionSi.hStdOutput = hVersionWritePipe;
        versionSi.dwFlags |= STARTF_USESTDHANDLES;
        
        ZeroMemory(&versionPi, sizeof(versionPi));
        
        if (CreateProcess(NULL, versionCmd, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &versionSi, &versionPi)) {
            CloseHandle(hVersionWritePipe);
            
            // Wait for version check to finish
            WaitForSingleObject(versionPi.hProcess, 5000); // 5 second timeout
            
            // Read version output
            char versionBuffer[1024];
            DWORD versionBytesRead;
            std::string versionOutput;
            
            while (ReadFile(hVersionReadPipe, versionBuffer, sizeof(versionBuffer) - 1, &versionBytesRead, NULL) && versionBytesRead > 0) {
                versionBuffer[versionBytesRead] = '\0';
                versionOutput += versionBuffer;
            }
            
            CloseHandle(hVersionReadPipe);
            CloseHandle(versionPi.hProcess);
            CloseHandle(versionPi.hThread);
            
            // Check if the version contains "6.3.0"
            if (versionOutput.find("6.3.0") == std::string::npos) {
                AppendToTerminal(L"Error: g++ version 6.3.0 is required for debugging!");
                AppendToTerminal(L"Current compiler version does not match requirement.");
                
                // Convert version output to wide string and display
                int wideLen = MultiByteToWideChar(CP_UTF8, 0, versionOutput.c_str(), -1, NULL, 0);
                WCHAR* wideVersion = new WCHAR[wideLen];
                MultiByteToWideChar(CP_UTF8, 0, versionOutput.c_str(), -1, wideVersion, wideLen);
                AppendToTerminal(wideVersion);
                delete[] wideVersion;
                
                delete[] codeBuffer;
                return;
            } else {
                AppendToTerminal(L"✓ g++ 6.3.0 verified - proceeding with debug compilation...");
            }
        } else {
            CloseHandle(hVersionWritePipe);
            CloseHandle(hVersionReadPipe);
            AppendToTerminal(L"Error: Could not check g++ version for debugging.");
            delete[] codeBuffer;
            return;
        }
    }
    
    AppendToTerminal(L"Compiling with debug symbols (-g flag) using g++ 6.3.0...");
    
    // Compile with debug symbols (-g flag)
    WCHAR compileCmd[1024];
    swprintf_s(compileCmd, L"\"C:\\MinGW\\bin\\g++.exe\" -g \"%s\" -o \"%s\"", tempCppFile, tempExeFile);
    
    HANDLE hReadPipe, hWritePipe;
    if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0)) {
        AppendToTerminal(L"Error: Could not create pipe for compiler output");
        delete[] codeBuffer;
        return;
    }
    
    // Set up process information
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.hStdError = hWritePipe;
    si.hStdOutput = hWritePipe;
    si.dwFlags |= STARTF_USESTDHANDLES;
    
    ZeroMemory(&pi, sizeof(pi));
    
    // Start the compiler process
    if (CreateProcess(NULL, compileCmd, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        AppendToTerminal(L"Using g++ compiler with debug symbols...");
        CloseHandle(hWritePipe);
        
        // Wait for compilation to finish
        WaitForSingleObject(pi.hProcess, 10000); // 10 second timeout
        
        // Check if compilation was successful
        DWORD exitCode;
        GetExitCodeProcess(pi.hProcess, &exitCode);
        
        // Read compiler output
        char buffer[4096];
        DWORD bytesRead;
        std::string compilerOutput;
        
        while (ReadFile(hReadPipe, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
            buffer[bytesRead] = '\0';
            compilerOutput += buffer;
        }
        
        CloseHandle(hReadPipe);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        
        if (exitCode == 0) {
            AppendToTerminal(L"Compilation successful!");
            
            // Verify GDB version requirement (must be 7.6.1)
            AppendToTerminal(L"Verifying GDB debugger version...");
            WCHAR gdbVersionCmd[512];
            swprintf_s(gdbVersionCmd, L"\"C:\\MinGW\\bin\\gdb.exe\" --version");
            
            HANDLE hGdbVersionReadPipe, hGdbVersionWritePipe;
            if (CreatePipe(&hGdbVersionReadPipe, &hGdbVersionWritePipe, &sa, 0)) {
                STARTUPINFO gdbVersionSi;
                PROCESS_INFORMATION gdbVersionPi;
                ZeroMemory(&gdbVersionSi, sizeof(gdbVersionSi));
                gdbVersionSi.cb = sizeof(gdbVersionSi);
                gdbVersionSi.hStdError = hGdbVersionWritePipe;
                gdbVersionSi.hStdOutput = hGdbVersionWritePipe;
                gdbVersionSi.dwFlags |= STARTF_USESTDHANDLES;
                
                ZeroMemory(&gdbVersionPi, sizeof(gdbVersionPi));
                
                if (CreateProcess(NULL, gdbVersionCmd, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &gdbVersionSi, &gdbVersionPi)) {
                    CloseHandle(hGdbVersionWritePipe);
                    
                    // Wait for GDB version check to finish
                    WaitForSingleObject(gdbVersionPi.hProcess, 5000); // 5 second timeout
                    
                    // Read GDB version output
                    char gdbVersionBuffer[1024];
                    DWORD gdbVersionBytesRead;
                    std::string gdbVersionOutput;
                    
                    while (ReadFile(hGdbVersionReadPipe, gdbVersionBuffer, sizeof(gdbVersionBuffer) - 1, &gdbVersionBytesRead, NULL) && gdbVersionBytesRead > 0) {
                        gdbVersionBuffer[gdbVersionBytesRead] = '\0';
                        gdbVersionOutput += gdbVersionBuffer;
                    }
                    
                    CloseHandle(hGdbVersionReadPipe);
                    CloseHandle(gdbVersionPi.hProcess);
                    CloseHandle(gdbVersionPi.hThread);
                    
                    // Check if the version contains "7.6.1"
                    if (gdbVersionOutput.find("7.6.1") == std::string::npos) {
                        AppendToTerminal(L"Error: GDB version 7.6.1 is required for debugging!");
                        AppendToTerminal(L"Current debugger version does not match requirement.");
                        
                        // Convert GDB version output to wide string and display
                        int wideLen = MultiByteToWideChar(CP_UTF8, 0, gdbVersionOutput.c_str(), -1, NULL, 0);
                        WCHAR* wideGdbVersion = new WCHAR[wideLen];
                        MultiByteToWideChar(CP_UTF8, 0, gdbVersionOutput.c_str(), -1, wideGdbVersion, wideLen);
                        AppendToTerminal(wideGdbVersion);
                        delete[] wideGdbVersion;
                        
                        delete[] codeBuffer;
                        return;
                    } else {
                        AppendToTerminal(L"✓ GDB 7.6.1 verified - proceeding with debug session...");
                    }
                } else {
                    CloseHandle(hGdbVersionWritePipe);
                    CloseHandle(hGdbVersionReadPipe);
                    AppendToTerminal(L"Error: Could not check GDB version.");
                    delete[] codeBuffer;
                    return;
                }
            }
            
            AppendToTerminal(L"Starting GDB 7.6.1 debugger...");
            AppendToTerminal(L"--- Debug Session ---");
            
            // Create a simple GDB script to run the program and show basic info
            WCHAR gdbScriptFile[MAX_PATH];
            swprintf_s(gdbScriptFile, L"%stemp_gdb_script.txt", tempDir);
            
            HANDLE hGdbScript = CreateFile(gdbScriptFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            if (hGdbScript != INVALID_HANDLE_VALUE) {
                const char* gdbCommands = 
                    "set confirm off\n"
                    "file temp_cplus_debug.exe\n"
                    "break main\n"
                    "run\n"
                    "info locals\n"
                    "continue\n"
                    "quit\n";
                
                DWORD gdbBytesWritten;
                WriteFile(hGdbScript, gdbCommands, (DWORD)strlen(gdbCommands), &gdbBytesWritten, NULL);
                CloseHandle(hGdbScript);
                
                // Run GDB with the script
                WCHAR gdbCmd[1024];
                swprintf_s(gdbCmd, L"\"C:\\MinGW\\bin\\gdb.exe\" --batch --command=\"%s\"", gdbScriptFile);
                
                // Create new pipes for GDB output
                HANDLE hGdbReadPipe, hGdbWritePipe;
                if (CreatePipe(&hGdbReadPipe, &hGdbWritePipe, &sa, 0)) {
                    STARTUPINFO gdbSi;
                    PROCESS_INFORMATION gdbPi;
                    ZeroMemory(&gdbSi, sizeof(gdbSi));
                    gdbSi.cb = sizeof(gdbSi);
                    gdbSi.hStdOutput = hGdbWritePipe;
                    gdbSi.hStdError = hGdbWritePipe;
                    gdbSi.dwFlags |= STARTF_USESTDHANDLES;
                    
                    ZeroMemory(&gdbPi, sizeof(gdbPi));
                    
                    // Change to temp directory for GDB to find the executable
                    if (CreateProcess(NULL, gdbCmd, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, tempDir, &gdbSi, &gdbPi)) {
                        CloseHandle(hGdbWritePipe);
                        
                        // Wait for GDB to finish
                        WaitForSingleObject(gdbPi.hProcess, 30000); // 30 second timeout
                        
                        // Read GDB output
                        char gdbBuffer[4096];
                        DWORD gdbBytesRead;
                        
                        while (ReadFile(hGdbReadPipe, gdbBuffer, sizeof(gdbBuffer) - 1, &gdbBytesRead, NULL) && gdbBytesRead > 0) {
                            gdbBuffer[gdbBytesRead] = '\0';
                            
                            // Convert to wide string and display
                            int wideLen = MultiByteToWideChar(CP_UTF8, 0, gdbBuffer, -1, NULL, 0);
                            WCHAR* wideOutput = new WCHAR[wideLen];
                            MultiByteToWideChar(CP_UTF8, 0, gdbBuffer, -1, wideOutput, wideLen);
                            
                            // Display GDB output in terminal
                            if (hTerminal) {
                                int termLength = GetWindowTextLength(hTerminal);
                                SendMessage(hTerminal, EM_SETSEL, termLength, termLength);
                                SendMessage(hTerminal, EM_REPLACESEL, FALSE, (LPARAM)wideOutput);
                            }
                            
                            delete[] wideOutput;
                        }
                        
                        CloseHandle(hGdbReadPipe);
                        CloseHandle(gdbPi.hProcess);
                        CloseHandle(gdbPi.hThread);
                        
                        AppendToTerminal(L"--- End Debug Session ---");
                        AppendToTerminal(L"Debug session completed.");
                    } else {
                        CloseHandle(hGdbWritePipe);
                        CloseHandle(hGdbReadPipe);
                        AppendToTerminal(L"Error: Could not start GDB debugger.");
                        AppendToTerminal(L"Make sure GDB is installed with MinGW.");
                    }
                }
                
                // Clean up GDB script
                DeleteFile(gdbScriptFile);
            }
        } else {
            AppendToTerminal(L"Debug compilation failed!");
            
            // Show compiler errors
            if (!compilerOutput.empty()) {
                int wideLen = MultiByteToWideChar(CP_UTF8, 0, compilerOutput.c_str(), -1, NULL, 0);
                WCHAR* wideError = new WCHAR[wideLen];
                MultiByteToWideChar(CP_UTF8, 0, compilerOutput.c_str(), -1, wideError, wideLen);
                AppendToTerminal(wideError);
                delete[] wideError;
            }
        }
    } else {
        CloseHandle(hWritePipe);
        CloseHandle(hReadPipe);
        AppendToTerminal(L"Error: Could not start compiler for debugging.");
        AppendToTerminal(L"Make sure g++ is installed and in PATH.");
    }
    
    // Clean up temporary files
    DeleteFile(tempCppFile);
    DeleteFile(tempExeFile);
    
    delete[] codeBuffer;
}
