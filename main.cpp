#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <bitset>
#include <fstream>
#include <cstring>
#include <windows.h>
#include <shlobj.h>
#include "fs_std.hpp"
#include "filesystem.hpp"
#include <codecvt> 
#include <locale>

using namespace std;

// ������� ��� ����������� ������ � ����� ������
int copyTextToClipboard(const std::string& text) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring wideText = converter.from_bytes(text);

    if (!OpenClipboard(0)) {
        return 011; // ������ �������� ������ ������
    }

    if (!EmptyClipboard()) {
        CloseClipboard();
        return 010; // ������ ������� ������ ������
    }

    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, (wideText.size() + 1) * sizeof(wchar_t));
    if (!hMem) {
        CloseClipboard();
        return 001; // ������ ��������� ������
    }

    wchar_t* buffer = static_cast<wchar_t*>(GlobalLock(hMem));
    wcscpy_s(buffer, wideText.size() + 1, wideText.c_str());
    GlobalUnlock(hMem);

    if (!SetClipboardData(CF_UNICODETEXT, hMem)) {
        return 01; // ������ ��������� ������ � ����� ������
    }

    CloseClipboard();
    return 0;
}

// ������� ���������� ������
void encryption(string& txt, int& key) {
    for (int i = 0; i < txt.length(); i++) {
        txt[i] = char(int(txt[i] + key)); // ���������� �������� ������
    }
}

// ������� ������������ ����� � ������
void permutation_bits(string& txt) {
    for (int i = 0; i < txt.length() - 1; i++) {
        int ascii_value = static_cast<int>(txt[i]);
        int ascii_value_2 = static_cast<int>(txt[i + 1]);
        string binary_representation = bitset<8>(ascii_value).to_string();
        string binary_representation_2 = bitset<8>(ascii_value_2).to_string();

        for (int j = 4; j < 7; j++) {
            swap(binary_representation[j], binary_representation_2[j]);
        }

        unsigned long decimalValue = bitset<8>(binary_representation).to_ulong();
        unsigned long decimalValue2 = bitset<8>(binary_representation_2).to_ulong();
        txt[i] = static_cast<char>(decimalValue);
        txt[i + 1] = static_cast<char>(decimalValue2);
    }
}

// ������� ������������ ������
void decryption(string& txt, int& key) {
    for (int i = 0; i < txt.length(); i++) {
        txt[i] = char(int(txt[i] - key)); // ������������ �������� ������
    }
}

// ������� �������� ������������ ����� � ������
void permutation_bits_back(string& txt) {
    for (int i = txt.length() - 1; i > 0; i--) {
        int ascii_value = static_cast<int>(txt[i]);
        int ascii_value_2 = static_cast<int>(txt[i - 1]);
        string binary_representation = bitset<8>(ascii_value).to_string();
        string binary_representation_2 = bitset<8>(ascii_value_2).to_string();

        for (int j = 4; j < 7; j++) {
            swap(binary_representation[j], binary_representation_2[j]);
        }

        unsigned long decimalValue = bitset<8>(binary_representation).to_ulong();
        unsigned long decimalValue2 = bitset<8>(binary_representation_2).to_ulong();
        txt[i] = static_cast<char>(decimalValue);
        txt[i - 1] = static_cast<char>(decimalValue2);
    }
}

// �������� ����������������� �����
string create_user_key(int& key, int& size_key) {
    string str_key;
    int rand_num;

    for (size_t i = 0; i <= size_key; i++) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(65, 90);
        rand_num = dis(gen);

        str_key += char(rand_num);
        key += rand_num;
    }
    return str_key;
}

// ���� ����� �������������
void deployment_key(int& key) {
    string user_key;
    cout << ">> Enter key:";
    cin >> user_key;
    for (int i = 0; i < user_key.length() - 1; i++) {
        key += int(user_key[i]);
    }
}

// ������� ���� �� ������ ��������
void clear_path(string& pathf) {
    string clearW;
    for (int i = 0; i < pathf.length(); i++) {
        if (char(pathf[i]) != '"') {
            clearW = clearW + pathf[i];
            if (pathf[i] == '\\')
                clearW = clearW + '\\';
        }
    }
    pathf = clearW;
    cout << pathf << endl;
}

int main() {
    setlocale(LC_ALL, "rus");

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(10, 90);
    int size_key = dis(gen);

    int user_command, key;
    cout << "#1 ����������\n";
    cout << "#2 ������������\n";
    cout << "<< ������� ����� �������: ";
    cin >> user_command;

    system("cls");

    wchar_t path[MAX_PATH] = L"";

    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = path;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = L"All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) == TRUE) {
        wcout << L"<< ��������� ����: " << path << std::endl;

        ifstream read_file;
        ofstream writh_file;

        switch (user_command) {
        case 1: {
            string txt;
            read_file.open(path);
            if (!read_file.is_open()) {
                cout << "<< ������ ��� �������� �����...";
            }
            else {
                getline(read_file, txt, '\0');
            }
            read_file.close();
            string user_key = create_user_key(key, size_key);
            encryption(txt, key);
            permutation_bits(txt);
            writh_file.open(path);
            if (!writh_file.is_open()) {
                cout << "<< ������ ��� ������ � ����...";
            }
            else {
                writh_file << txt;
            }
            writh_file.close();
            copyTextToClipboard(user_key);
            std::cout << "<< ���� ������� ���������� � ����� ������!!!" << std::endl;
            cout << ">> ����:" << user_key;
        }
              break;
        case 2: {
            string txt;
            read_file.open(path);
            if (!read_file.is_open()) {
                cout << "<< ������ ��� �������� �����...";
            }
            else {
                getline(read_file, txt, '\0');
            }
            read_file.close();
            deployment_key(key);
            permutation_bits_back(txt);
            decryption(txt, key);
            writh_file.open(path);
            if (!writh_file.is_open()) {
                cout << "<< ������ ��� ������ � ����...";
            }
            else {
                writh_file << txt;
            }
            writh_file.close();
        }
              break;
        default:
            break;
        }
    }
    else {
        std::cout << "<< ���������� ������� ����!!!" << std::endl;
    }
    return 0;
}
