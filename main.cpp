#ifndef UNICODE
#define UNICODE
#endif
#include <windows.h>
#include <string>
#include <commctrl.h>
#pragma comment(lib, "comctl32.lib")
#define id_buttontest 1
#define input1 4
#define search 5
#define info_window 6
#define add 7
#define del 8
// id for the stuff at the selected window
#define nametext 100
#define pricetext 101
#define quantitytext 102
#define typetext 103
#define curpricetext 104
#define infotext 105
#define idtext 106
// id for the text at the selected window
#define deftext1 200
#define deftext2 201
#define deftext3 202
#define deftext4 203
#define deftext5 204
#define deftext6 205
#define deftext7 206
// id for the text at the add new item window
#define deftext1_add 210
#define deftext2_add 211
#define deftext3_add 212
#define deftext4_add 213
#define deftext5_add 214
#define deftext6_add 215
// id for the input at the add new item window
#define inputname 400
#define inputprice 401
#define inputtype 402
#define inputquan 403
#define addnewcase 404
#define inputinfo 405
#define inputid 406
HWND hNameText;
HWND hPriceText;
HWND hQuantityText;
HWND hTypeText;
HWND hdelbut;
HWND haddbut;
HWND hcurpricetext;
HWND hinfo;
HWND hid;
extern "C"
{
#include "sqlite3.h"
}
int curid = 2;

HWND hList;
// to do ,
// make the rows and for each stocking stuff and display the count and make a button to view more info of that thing

void addrow(HWND hList, const wchar_t *name, int quantity, int price, const wchar_t *type, const wchar_t *infoo, int id)
{
    LVITEMW lvi = {0};
    lvi.mask = LVIF_TEXT;
    lvi.iItem = ListView_GetItemCount(hList);
    lvi.iSubItem = 0;
    lvi.pszText = (LPWSTR)name;
    int curprice = quantity * price;
    // Insert the actual row and get its index
    int index = ListView_InsertItem(hList, &lvi);
    // first column
    std::wstring qtyid = std::to_wstring(id);
    ListView_SetItemText(hList, index, 0, (LPWSTR)qtyid.c_str());
    // second column
    ListView_SetItemText(hList, index, 1, (LPWSTR)name);
    // third column
    std::wstring priceStr = std::to_wstring(price);
    ListView_SetItemText(hList, index, 2, (LPWSTR)priceStr.c_str());
    // fourth column
    std::wstring qtyStr = std::to_wstring(quantity);
    ListView_SetItemText(hList, index, 3, (LPWSTR)qtyStr.c_str());
    // fifth column
    ListView_SetItemText(hList, index, 4, (LPWSTR)type);
    // sixth column
    std::wstring curpriceStr = std::to_wstring(curprice);
    ListView_SetItemText(hList, index, 5, (LPWSTR)curpriceStr.c_str());
    // seventh column
    ListView_SetItemText(hList, index, 6, (LPWSTR)infoo);
}

void InitDatabase(HWND hList)
{
    sqlite3 *db;
    if (sqlite3_open16(L"stocks.db", &db) == SQLITE_OK)
    {
        // const char *sql = "CREATE TABLE IF NOT EXISTS test (""id INTEGER PRIMARY KEY AUTOINCREMENT, " "count INTEGER default 0);";
        /*
                char *errMsg = 0;
                if (sqlite3_exec(db, sql, NULL, NULL, &errMsg) != SQLITE_OK)
                {
                    MessageBoxA(NULL, errMsg, "SQL error", MB_OK);
                    sqlite3_free(errMsg);
                }
        */

        sqlite3_stmt *stmt;
        const wchar_t *command = L"SELECT * FROM test;";
        if (sqlite3_prepare16_v2(db, command, -1, &stmt, NULL) == SQLITE_OK)
        {

            while (sqlite3_step(stmt) == SQLITE_ROW)
            {
                int quantity = sqlite3_column_int(stmt, 3);
                int price = sqlite3_column_int(stmt, 2);

                const wchar_t *rawName = (const wchar_t *)sqlite3_column_text16(stmt, 1);
                const wchar_t *rawType = (const wchar_t *)sqlite3_column_text16(stmt, 4);
                const wchar_t *rawInfo = (const wchar_t *)sqlite3_column_text16(stmt, 5);
                int curprice = quantity * price;
                // wchar_t is larger than char at 16 bit (opposed to 8 bit) also L"" is for wchar (wide char)
                addrow(hList,
                       rawName ? rawName : L"Unknown",
                       quantity,
                       price,
                       rawType ? rawType : L"N/A",
                       rawInfo ? rawInfo : L"N/A",
                       sqlite3_column_int(stmt, 0));
            }
        }
        sqlite3_finalize(stmt);
        sqlite3_close(db);
    }
}

void clearlist(HWND hList)
{
    ListView_DeleteAllItems(hList);
}

/*************************************************************************************************************************************************************************
**************************************************************************************************************************************************************************
**************************************************************************************************************************************************************************
                table name test:
                id integer unique
                name text
                price integer
                quantity integer
                type text
                info text
**************************************************************************************************************************************************************************
**************************************************************************************************************************************************************************
**************************************************************************************************************************************************************************/

void updatelist(HWND hList, const wchar_t *name)
{
    sqlite3 *db;

    if (sqlite3_open16(L"stocks.db", &db) == SQLITE_OK)
    {
        sqlite3_stmt *stmt;

        const wchar_t *command =
            L"SELECT * FROM test WHERE name LIKE ? ;";
        std::wstring pattern = L"%";
        pattern += name;
        pattern += L"%";
        if (sqlite3_prepare16_v2(db, command, -1, &stmt, NULL) == SQLITE_OK)
        {
            sqlite3_bind_text16(
                stmt,
                1,
                pattern.c_str(),
                -1,
                SQLITE_TRANSIENT);

            while (sqlite3_step(stmt) == SQLITE_ROW)
            {
                int quantity = sqlite3_column_int(stmt, 3);
                int price = sqlite3_column_int(stmt, 2);

                const wchar_t *rawName =
                    (const wchar_t *)sqlite3_column_text16(stmt, 1);

                const wchar_t *rawType =
                    (const wchar_t *)sqlite3_column_text16(stmt, 4);

                const wchar_t *rawInfo =
                    (const wchar_t *)sqlite3_column_text16(stmt, 5);
                addrow(
                    hList,
                    rawName ? rawName : L"Unknown",
                    quantity,
                    price,
                    rawType ? rawType : L"N/A",
                    rawInfo ? rawInfo : L"N/A",
                    sqlite3_column_int(stmt, 0));
            }
        }
        sqlite3_finalize(stmt);
        sqlite3_close(db);
    }
}
bool searchdatabase(const std::wstring &symbol)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    bool found = false;

    if (sqlite3_open16(L"stocks.db", &db) != SQLITE_OK)
        return false;

    const wchar_t *sql = L"SELECT count FROM test WHERE id = 1;";

    if (sqlite3_prepare16_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            int count = sqlite3_column_int(stmt, 0);

            std::wstring result = L"Count: " + std::to_wstring(count);
            found = true;
        }
        else
        {
            found = false;
            std::wstring result = L"NOT FOUND";
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return found;
}

void searching(HWND hlist)
{
    wchar_t buffer[256];
    GetDlgItemTextW(GetActiveWindow(), input1, buffer, 256); // get the text from the input field and put it in buffer

    clearlist(hlist);
    updatelist(hlist, buffer);
}

void findquan(wchar_t *quan, wchar_t *cur)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;

    if (sqlite3_open16(L"stocks.db", &db) != SQLITE_OK)
        return;

    const wchar_t *sql = L"SELECT quantity , price FROM test WHERE id = ?;";

    if (sqlite3_prepare16_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, curid);
        /*
                if (sqlite3_step(stmt) == SQLITE_ROW)
                {
                    int quantity = sqlite3_column_int(stmt, 0);
                    std::wstring qtyStr = std::to_wstring(quantity);
                    wcscpy(quan, qtyStr.c_str());
                } */
        int rc = sqlite3_step(stmt);

        if (rc == SQLITE_ROW)
        {
            int quantity = sqlite3_column_int(stmt, 0);
            int price = sqlite3_column_int(stmt, 1);
            int curprice = quantity * price;
            std::wstring qtyStr = std::to_wstring(quantity);
            std::wstring curpriceStr = std::to_wstring(curprice);
            wcscpy(quan, qtyStr.c_str());
            wcscpy(cur, curpriceStr.c_str());
        }
        else
        {
            wchar_t buf[100];
            swprintf(buf, 100, L"step returned %d", rc);
            MessageBoxW(NULL, buf, L"Debug", MB_OK);
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void updateselect()
{
    wchar_t quan[256];
    wchar_t cur[256];
    findquan(quan, cur);
    SetWindowTextW(hQuantityText, quan);
    SetWindowTextW(hcurpricetext, cur);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = {};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.
    InitCommonControls(); // activate the library

    HWND hwnd = CreateWindowEx(
        0,                   // Optional window styles
        CLASS_NAME,          // Window class
        L"stocking (?)",     // Window text
        WS_OVERLAPPEDWINDOW, // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,      // Parent window
        NULL,      // Menu
        hInstance, // Instance handle
        NULL       // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop.

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

void findid(const wchar_t *name)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;

    if (sqlite3_open16(L"stocks.db", &db) != SQLITE_OK)
        return;

    const wchar_t *sql = L"SELECT id FROM test WHERE name = ?;";

    if (sqlite3_prepare16_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_text16(
            stmt,
            1,
            name,
            -1,
            SQLITE_TRANSIENT);

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            curid = sqlite3_column_int(stmt, 0);
            // do something with the id
        }
        else
        {
            // not found
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void adding(HWND hList)
{
    if (curid == -1)
        return;

    sqlite3 *db;
    sqlite3_stmt *stmt;

    if (sqlite3_open16(L"stocks.db", &db) != SQLITE_OK)
        return;

    const wchar_t *sql = L"UPDATE test SET quantity = quantity + 1 WHERE id = ?;";

    if (sqlite3_prepare16_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, curid);

        if (sqlite3_step(stmt) == SQLITE_DONE)
        {
            wchar_t buffer[256];
            GetDlgItemTextW(GetActiveWindow(), input1, buffer, 256);
            if (buffer[0] != L'\0')
            {
                searching(hList);
            }
            else
            {
                clearlist(hList);
                InitDatabase(hList); // refresh the list
            }
        }
        else
        {
            // update failed
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void addnew(HWND hList)
{
    wchar_t name[256];
    GetDlgItemTextW(GetActiveWindow(), inputname, name, 256);
    wchar_t idbuffer[256];
    GetDlgItemTextW(GetActiveWindow(), inputid, idbuffer, 256);
    int id = _wtoi(idbuffer);
    wchar_t type[256];
    GetDlgItemTextW(GetActiveWindow(), inputtype, type, 256);
    wchar_t priceBuffer[256];
    GetDlgItemTextW(GetActiveWindow(), inputprice, priceBuffer, 256);
    int price = _wtoi(priceBuffer);
    wchar_t quanBuffer[256];
    GetDlgItemTextW(GetActiveWindow(), inputquan, quanBuffer, 256);
    int quantity = _wtoi(quanBuffer);
    wchar_t infoadd[256];
    GetDlgItemTextW(GetActiveWindow(), inputinfo, infoadd, 256);

    sqlite3 *db;
    sqlite3_stmt *stmt;
    if (sqlite3_open16(L"stocks.db", &db) != SQLITE_OK)
        return;
    const wchar_t *sql = L"INSERT INTO test (id , name, price, quantity, type, info) VALUES (? , ?, ?, ?, ?, ?);";
    if (sqlite3_prepare16_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, id);
        sqlite3_bind_text16(stmt, 2, name, -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 3, price);
        sqlite3_bind_int(stmt, 4, quantity);
        sqlite3_bind_text16(stmt, 5, type, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text16(stmt, 6, infoadd, -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmt) == SQLITE_DONE)
        {
            wchar_t buffer[256];
            GetDlgItemTextW(GetActiveWindow(), input1, buffer, 256);
            if (buffer[0] != L'\0')
            {
                searching(hList);
            }
            else
            {
                clearlist(hList);
                InitDatabase(hList); // refresh the list
            }
        }
        else
        {
            // failure
        }
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void deleting(HWND hList)
{
    if (curid == -1)
        return;

    sqlite3 *db;
    sqlite3_stmt *stmt;

    if (sqlite3_open16(L"stocks.db", &db) != SQLITE_OK)
        return;

    const wchar_t *sql = L"UPDATE test SET quantity = quantity - 1 WHERE id = ? AND quantity > 0;";

    if (sqlite3_prepare16_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, curid);

        if (sqlite3_step(stmt) == SQLITE_DONE)
        {
            wchar_t buffer[256];
            GetDlgItemTextW(GetActiveWindow(), input1, buffer, 256);
            if (buffer[0] != L'\0')
            {
                searching(hList);
            }
            else
            {
                clearlist(hList);
                InitDatabase(hList); // refresh the list
            }
        }
        else
        {
            // update failed
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // All painting occurs here, between BeginPaint and EndPaint.

        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        EndPaint(hwnd, &ps);
    }
        return 0;

    case WM_CREATE:
    {

        CreateWindowW(
            L"EDIT",
            L"",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
            50, 10, 200, 25,
            hwnd,
            (HMENU)input1,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);

        // create info window

        hList = CreateWindowW(
            WC_LISTVIEW,
            L"",
            WS_VISIBLE | WS_CHILD | LVS_REPORT | WS_BORDER,
            40, 40, 800, 600,
            hwnd,
            (HMENU)info_window,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);

        ListView_SetExtendedListViewStyle(hList, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

        // define columns
        LVCOLUMNW lvc = {0};
        lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

        // col 1
        lvc.iSubItem = 0;
        lvc.pszText = (LPWSTR)L"id";
        lvc.cx = 150; // Width in pixels
        ListView_InsertColumn(hList, 0, &lvc);

        // col 2
        lvc.iSubItem = 1;
        lvc.pszText = (LPWSTR)L"name";
        lvc.cx = 150; // Width in pixels
        ListView_InsertColumn(hList, 1, &lvc);

        // col 2
        lvc.iSubItem = 2;
        lvc.pszText = (LPWSTR)L"price";
        lvc.cx = 100;
        ListView_InsertColumn(hList, 2, &lvc);

        // col 3
        lvc.iSubItem = 3;
        lvc.pszText = (LPWSTR)L"quantity";
        lvc.cx = 100;
        ListView_InsertColumn(hList, 3, &lvc);

        // col 4
        lvc.iSubItem = 4;
        lvc.pszText = (LPWSTR)L"type";
        lvc.cx = 100;
        ListView_InsertColumn(hList, 4, &lvc);

        // col5
        lvc.iSubItem = 5;
        lvc.pszText = (LPWSTR)L"currentprice";
        lvc.cx = 100;
        ListView_InsertColumn(hList, 5, &lvc);

        // col6
        lvc.iSubItem = 6;
        lvc.pszText = (LPWSTR)L"info";
        lvc.cx = 100;
        ListView_InsertColumn(hList, 6, &lvc);

        //
        //
        //
        // database stuff
        InitDatabase(hList);
        CreateWindowW(
            L"BUTTON", // type of stuff
            L"Submit", // the label
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            260, 10, 80, 25,
            hwnd,
            (HMENU)search, // case
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);

        //
        //
        // the selected window
        CreateWindowW(
            L"STATIC",
            L"id : ",
            WS_CHILD | WS_VISIBLE,
            900, 30, 100, 35,
            hwnd,
            (HMENU)deftext7,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);
        CreateWindowW(
            L"STATIC",
            L"name: ",
            WS_CHILD | WS_VISIBLE,
            900, 70, 100, 35,
            hwnd,
            (HMENU)deftext1,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);
        CreateWindowW(
            L"STATIC",
            L"price: ",
            WS_CHILD | WS_VISIBLE,
            900, 110, 100, 35,
            hwnd,
            (HMENU)deftext2,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);
        CreateWindowW(
            L"STATIC",
            L"quantity: ",
            WS_CHILD | WS_VISIBLE,
            900, 150, 100, 35,
            hwnd,
            (HMENU)deftext3,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);
        CreateWindowW(
            L"STATIC",
            L"type: ",
            WS_CHILD | WS_VISIBLE,
            900, 190, 100, 35,
            hwnd,
            (HMENU)deftext4,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);
        CreateWindowW(
            L"STATIC",
            L"current price: ",
            WS_CHILD | WS_VISIBLE,
            900, 230, 100, 35,
            hwnd,
            (HMENU)deftext5,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);
        CreateWindowW(
            L"STATIC",
            L"info: ",
            WS_CHILD | WS_VISIBLE,
            900, 270, 200, 35,
            hwnd,
            (HMENU)deftext6,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);
        hid = CreateWindowW(
            L"STATIC",
            L" ",
            WS_CHILD | WS_VISIBLE,
            990, 30, 100, 35,
            hwnd,
            (HMENU)idtext,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);
        hNameText = CreateWindowW(
            L"STATIC",
            L" ",
            WS_CHILD | WS_VISIBLE,
            990, 70, 100, 35,
            hwnd,
            (HMENU)nametext,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);
        hPriceText = CreateWindowW(
            L"STATIC",
            L" ",
            WS_CHILD | WS_VISIBLE,
            990, 110, 100, 35,
            hwnd,
            (HMENU)pricetext,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);
        hQuantityText = CreateWindowW(
            L"STATIC",
            L" ",
            WS_CHILD | WS_VISIBLE,
            990, 150, 100, 35,
            hwnd,
            (HMENU)quantitytext,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);
        hTypeText = CreateWindowW(
            L"STATIC",
            L" ",
            WS_CHILD | WS_VISIBLE,
            990, 190, 100, 35,
            hwnd,
            (HMENU)typetext,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);
        hcurpricetext = CreateWindowW(
            L"STATIC",
            L" ",
            WS_CHILD | WS_VISIBLE,
            990, 230, 100, 35,
            hwnd,
            (HMENU)curpricetext,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);
        hinfo = CreateWindowW(
            L"STATIC",
            L" ",
            WS_CHILD | WS_VISIBLE,
            990, 270, 200, 35,
            hwnd,
            (HMENU)infotext,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);
        haddbut = CreateWindowW(
            L"BUTTON", // type of stuff
            L"add",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            930, 310, 75, 25,
            hwnd,
            (HMENU)add, // case
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);
        hdelbut = CreateWindowW(
            L"BUTTON", // type of stuff
            L"delete",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            1010, 310, 75, 25,
            hwnd,
            (HMENU)del, // case
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);
        //
        //
        //
        // the add new item window
        CreateWindowW(
            L"STATIC",
            L"id: ",
            WS_CHILD | WS_VISIBLE,
            900, 360, 70, 25,
            hwnd,
            (HMENU)deftext6_add,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);
        CreateWindowW(
            L"STATIC",
            L"name: ",
            WS_CHILD | WS_VISIBLE,
            900, 390, 70, 25,
            hwnd,
            (HMENU)deftext1_add,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);
        CreateWindowW(
            L"STATIC",
            L"price: ",
            WS_CHILD | WS_VISIBLE,
            900, 420, 70, 25,
            hwnd,
            (HMENU)deftext2_add,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);
        CreateWindowW(
            L"STATIC",
            L"quantity: ",
            WS_CHILD | WS_VISIBLE,
            900, 450, 70, 25,
            hwnd,
            (HMENU)deftext3_add,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);
        CreateWindowW(
            L"STATIC",
            L"type: ",
            WS_CHILD | WS_VISIBLE,
            900, 480, 70, 25,
            hwnd,
            (HMENU)deftext4_add,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);
        CreateWindowW(
            L"STATIC",
            L"info: ",
            WS_CHILD | WS_VISIBLE,
            900, 510, 70, 25,
            hwnd,
            (HMENU)deftext5_add,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);
        CreateWindowW(
            L"EDIT",
            L"",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
            960, 360, 200, 25,
            hwnd,
            (HMENU)inputid,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);
        CreateWindowW(
            L"EDIT",
            L"",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
            960, 390, 200, 25,
            hwnd,
            (HMENU)inputname,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);
        CreateWindowW(
            L"EDIT",
            L"",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
            960, 420, 200, 25,
            hwnd,
            (HMENU)inputprice,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);
        CreateWindowW(
            L"EDIT",
            L"",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
            960, 450, 200, 25,
            hwnd,
            (HMENU)inputquan,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);
        CreateWindowW(
            L"EDIT",
            L"",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
            960, 480, 200, 25,
            hwnd,
            (HMENU)inputtype,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);
        CreateWindowW(
            L"EDIT",
            L"",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
            960, 510, 200, 25,
            hwnd,
            (HMENU)inputinfo,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);
        CreateWindowW(
            L"BUTTON", // type of stuff
            L"add new",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            960, 540, 75, 25,
            hwnd,
            (HMENU)addnewcase, // case
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);

        /*
        CreateWindowW(
            L"BUTTON",                                             // Predefined class
            L"test",                                               // Button text
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // Styles
            50,                                                    // x position
            50,                                                    // y position
            100,                                                   // width
            30,                                                    // height
            hwnd,                                                  // Parent window
            (HMENU)id_buttontest,                                  // Control ID
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL);
        */
    }
    case WM_COMMAND:
    {
        // Check which control sent the notification
        switch (LOWORD(wParam))
        {
        case search:
        {
            searching(hList);
            break;
        }
        case add:
        {
            adding(hList);
            updateselect();
            break;
        }
        case del:
        {
            deleting(hList);
            updateselect();
            break;
        }
        case addnewcase:
        {
            addnew(hList);
            break;
        }
        }
        return 0;
    }
    case WM_NOTIFY:
    {
        LPNMHDR lpnmh = (LPNMHDR)lParam;

        // Check if the message is coming from our list and if its a double-click
        // the -> acts like a . , like lpnmh.idFrom is like calling a class lpnmh for the variable idFrom (i guess)
        if (lpnmh->idFrom == info_window && lpnmh->code == NM_DBLCLK)
        {

            LPNMLISTVIEW pnmv = (LPNMLISTVIEW)lParam;
            int rowIdx = pnmv->iItem; // the row since iSubItem is the column and iItem is the row

            if (rowIdx != -1)
            {
                wchar_t nameBuffer[256];
                ListView_GetItemText(lpnmh->hwndFrom, rowIdx, 1, nameBuffer, 256); // get item from lpnmh from row rowidx at the column 1 (second column) and put it in nameBuffer
                wchar_t typeBuffer[256];
                ListView_GetItemText(lpnmh->hwndFrom, rowIdx, 4, typeBuffer, 256);
                wchar_t priceBuffer[256];
                ListView_GetItemText(lpnmh->hwndFrom, rowIdx, 2, priceBuffer, 256);
                wchar_t quantityBuffer[256];
                ListView_GetItemText(lpnmh->hwndFrom, rowIdx, 3, quantityBuffer, 256);
                wchar_t idBuffer[256];
                ListView_GetItemText(lpnmh->hwndFrom, rowIdx, 0, idBuffer, 256);
                curid = _wtoi(idBuffer);
                wchar_t infoBuffer[256];
                ListView_GetItemText(lpnmh->hwndFrom, rowIdx, 6, infoBuffer, 256);
                int curprice = _wtoi(priceBuffer) * _wtoi(quantityBuffer);
                wchar_t curpriceBuffer[256];
                _itow_s(curprice, curpriceBuffer, 256, 10);
                SetWindowTextW(hNameText, nameBuffer);
                SetWindowTextW(hTypeText, typeBuffer);
                SetWindowTextW(hPriceText, priceBuffer);
                SetWindowTextW(hQuantityText, quantityBuffer);
                SetWindowTextW(hid, idBuffer);
                SetWindowTextW(hinfo, infoBuffer);
                SetWindowTextW(hcurpricetext, curpriceBuffer);
                // make the new window for the selected item
            }
        }
        break;
    }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
