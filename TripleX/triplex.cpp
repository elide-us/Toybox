#include <iostream>
#include <memory>

#include "triplex_impl.hpp"

int wmain(int argc, wchar_t argv[])
{
    // Instantiate data classes
    TripleX::DifficultyPtr pDifficulty = std::make_unique<TripleX::Difficulty>();
    TripleX::CluesPtr pClues = std::make_unique<TripleX::Clues>();
    TripleX::GuessesPtr pGuesses = std::make_unique<TripleX::Guesses>();
    
    while (true)
    {
        bool bLevelSuccess = PlayLevel(pDifficulty, pClues, pGuesses);

        // Clear error and buffer
        std::wcin.clear();
        std::wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (bLevelSuccess)
        {
            pDifficulty->IncrementDifficulty();
        }
        else
        {
            pDifficulty->DecrementDifficulty();
        }
    }

    return 0;
}
