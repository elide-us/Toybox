#include <iostream>
#include <string>
#include <sstream>
#include <random>
#include <vector>
#include <cstdio>

int GetNextRandomRange(int min, int max)
{
    static std::mt19937 engine{ std::random_device{}() };
    std::uniform_int_distribution<int> distribution(min, max);

    return distribution(engine);
}

enum class HackResult
{
    Success,
    Failure
};

HackResult PlayGameAtDifficulty(const int difficulty, const int length)
{
    auto _difficulty = 2L;
    if (difficulty > 2)
        _difficulty = difficulty;

    auto _length = 3L;
    if (length > 3)
        _length = length;

    auto _pinSum = 0L;
    auto _pinProduct = 1L;

    std::wcout << L"CHEAT! Numbers:";
    for (auto i = 0L; i < _length; ++i)
    {
        auto _num = GetNextRandomRange(_difficulty, _difficulty * 2);
        std::wcout << L" " << _num;
        _pinSum += _num;
        _pinProduct *= _num;
    }
    std::wcout << std::endl;

    std::wcout << L"You're a secret agent breaking into LEVEL " << _difficulty << L" server room!" << std::endl;
    std::wcout << L"Your SuperHacker 2000 tells you the following information..." << std::endl;
    std::wcout << std::endl;
    std::wcout << L"\tThere are " << _length << L" numbers in the code" << std::endl;
    std::wcout << L"\tThe product of the numbers is " << _pinProduct << std::endl;
    std::wcout << L"\tThe sum of the numbers is " << _pinSum << std::endl;
    std::wcout << std::endl;
    std::wcout << L"Enter the numbers followed by x" << std::endl;
    std::wcout << L"~nixadm> ";

    std::wstring _pinGuess;
    std::getline(std::wcin, _pinGuess);

    std::vector<std::wstring> codes;
    std::wstring code;

    std::wistringstream wiss{ _pinGuess.c_str() };
    while (std::getline(wiss, code, L' '))
    {
        if (code.compare(L"x") == 0)
            break;
        codes.push_back(code);
    }

    auto _guessSum = 0L;
    auto _guessProduct = 1L;

    for (auto code : codes)
    {
        auto _l = wcstol(code.c_str(), nullptr, 10);
        _guessSum += _l;
        _guessProduct *= _l;
    }

    if (_guessSum == _pinSum && _guessProduct == _pinProduct)
        return HackResult::Success;
    else
        return HackResult::Failure;
}

int wmain(int argc, wchar_t argv[])
{
    auto level = 2L;
    auto maxLevel = 10L;
    auto length = 3L;

    while (level <= maxLevel)
    {
        auto result = PlayGameAtDifficulty(level, length);
        std::wcout << std::endl;
        
        if (result == HackResult::Success)
        {
            std::wcout << L"Success! You have hacked a code!" << std::endl;
            std::wcout << L"The system has allowed you access to the next level..." << std::endl;
            std::wcout << std::endl;

            level++;
        }
        else
        {
            std::wcout << L"Failure! You have failed to hack the code!" << std::endl;
            std::wcout << L"The system has returned you to the previous level... Try again!" << std::endl;
            std::wcout << std::endl;
            
            level--;
            if (level < 2) level = 2;
        }
    }

    std::wcout << L"You've gained access to the server room, you're a master hacker!" << std::endl;

    return 0;
}
