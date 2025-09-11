#pragma once
// Add this definition to assign a unique value to ID_FILE_OPENFILE
#define ID_FILE_OPENFILE                32780
#define ID_FILE_SAVEFILE                32781
#define ID_DOTCPLUS_RUNPROJECT          32787
#define ID_ADD_HEADERS                  32789
#define ID_COPILOT_AI                   32790
#define ID_C_NEWCHATWINDOW              32793
#define ID_GIT_STASH                    32795
#define ID_GIT_COMMIT                   32796
#define ID_GIT_OPENLOCALREPO            32797
#define ID_GIT_ADD                      32798
#define ID_ADD_ADDFILE                  32799
#define ID_ADD_ADDFOLDER                32800
#define ID_GIT_REMOTE                   32801
#define ID_REMOTE_PULL                  32802
#define ID_REMOTE_PUSH                  32803
#define ID_REMOTE_SYNC                  32804
#define ID_GIT_BRANCH                   32805
#define ID_BRANCH_NEW                   32806
#define ID_BRANCH_OPEN                  32807
#define ID_TERMINAL_MICROSOFTPOWERSHELL 32814
#define ID_TERMINAL_GITCLI              32810
#define ID_TERMINAL_COMMANDPROMPT       32812
#define ID_TERMINAL_CPLUSTERMINAL       32813
#define ID_GIT_OPENGITWINDOW            32816

// Unicode support function declarations
#include <string>
bool WriteUnicodeFileImpl(const std::wstring& filePath, const std::wstring& content);
std::wstring ReadUnicodeFileImpl(const std::wstring& filePath);
bool SetConsoleUnicodeImpl();

#include "resource.h"