// C+ IDE.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "C+ IDE.h"
#include <commdlg.h>
#include <string.h>
#include <string>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND hEdit = NULL; // Add this at the top of your file
HWND hLineNumbers = NULL; // Line numbers control

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void                UpdateLineNumbers();

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
                // Handle making a GUI project
                break;
            case ID_MAKE_MAKE32778:
                // Handle making a CLI project
                break;
            case ID__OPEN:
                // Handle opening a .cplus project (.csplusproj)
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
                10, 10, 50, 600, // Position and size for line numbers
                hWnd, (HMENU)2, hInst, NULL);
                
            // Create main text editor
            hEdit = CreateWindowEx(
                WS_EX_CLIENTEDGE, L"EDIT", L"",
                WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
                65, 10, 735, 600, // Position and size for editor (offset by line numbers width)
                hWnd, (HMENU)1, hInst, NULL);
                
            // Set a monospace font for both controls
            HFONT hFont = CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY, FIXED_PITCH | FF_MODERN, L"Consolas");
            SendMessage(hEdit, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hLineNumbers, WM_SETFONT, (WPARAM)hFont, TRUE);
            
            UpdateLineNumbers();
        }
        break;
    case WM_SIZE:
        {
            if (hLineNumbers && hEdit)
            {
                int width = LOWORD(lParam);
                int height = HIWORD(lParam);
                
                // Resize line numbers control
                MoveWindow(hLineNumbers, 10, 10, 50, height - 20, TRUE);
                
                // Resize editor control (offset by line numbers width)
                MoveWindow(hEdit, 65, 10, width - 75, height - 20, TRUE);
                
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
