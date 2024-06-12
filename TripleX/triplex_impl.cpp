#include <cassert>
#include <iostream>

#include "triplex_impl.hpp"

void TripleX::Difficulty::SetDifficulty(int difficulty)
{
    assert(difficulty >= MIN_DIFFICULTY && difficulty <= MAX_DIFFICULTY);

    mDifficulty = difficulty;
}

void TripleX::Difficulty::IncrementDifficulty()
{
    mDifficulty++;

    if (mDifficulty >= MAX_DIFFICULTY) mDifficulty = MAX_DIFFICULTY;
}

void TripleX::Difficulty::DecrementDifficulty()
{
    mDifficulty--;

    if (mDifficulty <= MIN_DIFFICULTY) mDifficulty = MIN_DIFFICULTY;
}

int const & TripleX::Difficulty::GetDifficulty()
{
    return mDifficulty;
}

void TripleX::Clues::UpdateSum()
{
    mClueSum = mClueA + mClueB + mClueC;
}

void TripleX::Clues::UpdateProduct()
{
    mClueProduct = mClueA * mClueB * mClueC;
}

void TripleX::Clues::SetNumbers(int numberA, int numberB, int numberC)
{
    mClueA = numberA;
    mClueB = numberB;
    mClueC = numberC;

    TripleX::Clues::UpdateSum();
    TripleX::Clues::UpdateProduct();
}

int const & TripleX::Clues::GetClueA()
{
    return mClueA;
}

int const & TripleX::Clues::GetClueB()
{
    return mClueB;
}

int const & TripleX::Clues::GetClueC()
{
    return mClueC;
}

int const & TripleX::Clues::GetClueSum()
{
    return mClueSum;
}

int const & TripleX::Clues::GetClueProduct()
{
    return mClueProduct;
}

void TripleX::Guesses::UpdateSum()
{
    mGuessSum = mGuessA + mGuessB + mGuessC;
}

void TripleX::Guesses::UpdateProduct()
{
    mGuessProduct = mGuessA * mGuessB * mGuessC;
}

void TripleX::Guesses::SetNumbers(int numberA, int numberB, int numberC)
{
    mGuessA = numberA;
    mGuessB = numberB;
    mGuessC = numberC;

    Guesses::UpdateSum();
    Guesses::UpdateProduct();
}

int const & TripleX::Guesses::GetGuessA()
{
    return mGuessA;
}

int const & TripleX::Guesses::GetGuessB()
{
    return mGuessB;
}

int const & TripleX::Guesses::GetGuessC()
{
    return mGuessC;
}

int const & TripleX::Guesses::GetGuessSum()
{
    return mGuessSum;
}

int const & TripleX::Guesses::GetGuessProduct()
{
    return mGuessProduct;
}

int TripleX::GetRandomRange(int min, int max)
{
    assert(min < max);

    static std::mt19937 engine{ std::random_device{}() };
    std::uniform_int_distribution dist{ min, max };

    return dist(engine);
}

void TripleX::PrintIntroduction(DifficultyPtr const & pDifficulty)
{
    std::wcout << L"You're a secret agent breaking into a level " << pDifficulty->GetDifficulty() << " secure server room..." << std::endl;
    std::wcout << L"You need to enter the correct codes to continue..." << std::endl;
    std::wcout << std::endl;
}

void TripleX::PrintCluesAndPrompt(CluesPtr const & pClues)
{
    std::wcout << L"\tThere are 3 numbers in the code" << std::endl;
    std::wcout << L"\tThe product of the numbers is " << pClues->GetClueProduct() << std::endl;
    std::wcout << L"\tThe sum of the numbers is " << pClues->GetClueSum() << std::endl;
    std::wcout << std::endl;
    std::wcout << L"Enter your guesses (separated by spaces, i.e. 1 2 3): ";
}

void TripleX::PrintGuessDebug(GuessesPtr const & pGuesses)
{
    std::wcout << L"DEBUG: You entered: " << pGuesses->GetGuessA() << " " << pGuesses->GetGuessB() << " " << pGuesses->GetGuessC() << std::endl;
    std::wcout << L"DEBUG: Your guess product is: " << pGuesses->GetGuessProduct() << std::endl;
    std::wcout << L"DEBUG: Your guess sum is: " << pGuesses->GetGuessSum() << std::endl;
}

void TripleX::PrintCluesDebug(CluesPtr const & pClues)
{
    std::wcout << L"DEBUG: Clue codes are: " << pClues->GetClueA() << " " << pClues->GetClueB() << " " << pClues->GetClueC() << std::endl;
}

bool TripleX::PlayLevel(DifficultyPtr const & pDifficulty, CluesPtr const & pClues, GuessesPtr const & pGuesses)
{
    TripleX::PrintIntroduction(pDifficulty);

    TripleX::PrintCluesDebug(pClues);

    TripleX::PrintCluesAndPrompt(pClues);

    // Declare variables and capture user input
    int GuessA, GuessB, GuessC;
    std::wcin >> GuessA >> GuessB >> GuessC;

    // If failbit is set, set all the inputs to 0 before passing to SetNumbers()
    if (!std::wcin)
    {
        GuessA = GuessB = GuessC = 0;
    }

    // Copy input to Guesses class
    pGuesses->SetNumbers(GuessA, GuessB, GuessC);

    TripleX::PrintGuessDebug(pGuesses);

    // Test if guess calculations are the same as code calculations
    if (pGuesses->GetGuessProduct() == pClues->GetClueProduct() && pGuesses->GetGuessSum() == pClues->GetClueSum())
    {
        std::wcout << L"You guessed correctly!" << std::endl;
        std::wcout << std::endl;

        return true;
    }
    else
    {
        std::wcout << L"These are the wrong numbers! :(" << std::endl;
        std::wcout << std::endl;

        return false;
    }    
}
