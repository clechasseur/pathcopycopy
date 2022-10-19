// HotkeyHandler.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "PathCopyCopyHotkeyHandler.h"

#define MAX_LOADSTRING 100

typedef void (*_GetPathWithPluginW)(HWND p_hWnd, HINSTANCE /*p_hDllInstance*/, LPWSTR p_pCmdLine, int /*p_ShowCmd*/);

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
_GetPathWithPluginW GetPathWithPluginW;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    HMODULE hPcc = LoadLibrary(L"PCC64.dll");

    if (!hPcc) {
        MessageBox(NULL, L"Error: could not find PCC64.dll", L"ERROR", 0);
        return 1;
    }

    GetPathWithPluginW = (_GetPathWithPluginW)GetProcAddress(hPcc, "GetPathWithPluginW");


    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PATHCOPYCOPYHOTKEYHANDLER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PATHCOPYCOPYHOTKEYHANDLER));

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

    return (int)msg.wParam;
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

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PATHCOPYCOPYHOTKEYHANDLER));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_PATHCOPYCOPYHOTKEYHANDLER);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

    UpdateWindow(hWnd);

    return TRUE;
}

// code from https://devblogs.microsoft.com/oldnewthing/20040720-00/?p=38393
// for getting selected file/folder from explorer window

void SelectedPathToClipboard(HWND hWnd) {
    TCHAR g_szPath[MAX_PATH];
    TCHAR g_szItem[MAX_PATH];
    TCHAR g_formatString[] = L"{331A3B60-AF49-44f4-B30D-56ADFF6D25E8},%s\\%s";
    TCHAR g_combined[sizeof(g_formatString) + sizeof(g_szPath) + 1 + sizeof(g_szItem)];
    HWND hwndFind = GetForegroundWindow();
    g_szPath[0] = TEXT('\0');
    g_szItem[0] = TEXT('\0');
    IShellWindows* psw;
    CoInitialize(NULL);
    HRESULT result = CoCreateInstance(CLSID_ShellWindows, NULL, CLSCTX_ALL,
        IID_IShellWindows, (void**)&psw);
    if (SUCCEEDED(result)) {
        VARIANT v;
        V_VT(&v) = VT_I4;
        IDispatch* pdisp;
        BOOL fFound = FALSE;
        for (V_I4(&v) = 0; !fFound && psw->Item(v, &pdisp) == S_OK;
            V_I4(&v)++) {
            IWebBrowserApp* pwba;
            if (SUCCEEDED(pdisp->QueryInterface(IID_IWebBrowserApp, (void**)&pwba))) {
                HWND hwndWBA;
                if (SUCCEEDED(pwba->get_HWND((LONG_PTR*)&hwndWBA)) &&
                    hwndWBA == hwndFind) {
                    fFound = TRUE;
                    IServiceProvider* psp;
                    if (SUCCEEDED(pwba->QueryInterface(IID_IServiceProvider, (void**)&psp))) {
                        IShellBrowser* psb;
                        if (SUCCEEDED(psp->QueryService(SID_STopLevelBrowser,
                            IID_IShellBrowser, (void**)&psb))) {
                            IShellView* psv;
                            if (SUCCEEDED(psb->QueryActiveShellView(&psv))) {
                                IFolderView* pfv;
                                if (SUCCEEDED(psv->QueryInterface(IID_IFolderView,
                                    (void**)&pfv))) {
                                    IPersistFolder2* ppf2;
                                    if (SUCCEEDED(pfv->GetFolder(IID_IPersistFolder2,
                                        (void**)&ppf2))) {
                                        LPITEMIDLIST pidlFolder;
                                        if (SUCCEEDED(ppf2->GetCurFolder(&pidlFolder))) {
                                            if (!SHGetPathFromIDList(pidlFolder, g_szPath)) {
                                                lstrcpyn(g_szPath, TEXT("<not a directory>"), MAX_PATH);
                                            }
                                            int iFocus;
                                            if (SUCCEEDED(pfv->GetFocusedItem(&iFocus))) {
                                                LPITEMIDLIST pidlItem;
                                                if (SUCCEEDED(pfv->Item(iFocus, &pidlItem))) {
                                                    IShellFolder* psf;
                                                    if (SUCCEEDED(ppf2->QueryInterface(IID_IShellFolder,
                                                        (void**)&psf))) {
                                                        STRRET str;
                                                        if (SUCCEEDED(psf->GetDisplayNameOf(pidlItem,
                                                            SHGDN_INFOLDER | SHGDN_FORPARSING,
                                                            &str))) {
                                                            StrRetToBuf(&str, pidlItem, g_szItem, MAX_PATH);
                                                            StringCchPrintf(g_combined, sizeof(g_combined), g_formatString, g_szPath, g_szItem);
                                                            GetPathWithPluginW(NULL, NULL, g_combined, 0);
                                                        }
                                                        psf->Release();
                                                    }
                                                    CoTaskMemFree(pidlItem);
                                                }
                                            }
                                            CoTaskMemFree(pidlFolder);
                                        }
                                        ppf2->Release();
                                    }
                                    pfv->Release();
                                }
                                psv->Release();
                            }
                            psb->Release();
                        }
                        psp->Release();
                    }
                }
                pwba->Release();
            }
            pdisp->Release();
        }
        psw->Release();
    }
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
    case WM_CREATE:
        RegisterHotKey(hWnd, 1, MOD_ALT | MOD_CONTROL, 'C');
        return DefWindowProc(hWnd, message, wParam, lParam);
    case WM_HOTKEY:
        SelectedPathToClipboard(hWnd);
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
