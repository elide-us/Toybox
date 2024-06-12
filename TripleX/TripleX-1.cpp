#include <iostream>
#include <random>

// Prints level detail and objective of the game
void PrintIntroduction(const int difficulty)
{
    std::cout << "You're a secret agent breaking into a level " << difficulty << " secure server room..." << std::endl;
    std::cout << "You need to enter the correct codes to continue..." << std::endl;
    std::cout << std::endl;
}

// For debug purposes, prints the codes that are used to create the clues
void PrintCodesDebug(const int codeA, const int codeB, const int codeC)
{
    std::cout << "\t****DEBUG****\tCodes are:\t" << codeA << " " << codeB << " " << codeC << std::endl;
}

// Prints the clue numbers and a prompt to input guesses
void PrintCluesAndPrompt(const int product, const int sum)
{
    std::cout << "\tThere are 3 numbers in the code" << std::endl;
    std::cout << "\tThe product of the numbers is " << product << std::endl;
    std::cout << "\tThe sum of the numbers is " << sum << std::endl;
    std::cout << std::endl;
    std::cout << "Enter your three guesses (separated by spaces, i.e. 1 2 3): ";
}

// For debug purposes, prints the values derived from input
void PrintGuessDebug(const int guessA, const int guessB, const int guessC, const int guessProduct, const int guessSum)
{
    std::cout << "****DEBUG****\tInput:\t\t" << guessA << " " << guessB << " " << guessC << std::endl;
    std::cout << "****DEBUG****\tInput Product:\t" << guessProduct << std::endl;
    std::cout << "****DEBUG****\tInput Sum:\t" << guessSum << std::endl;
}

// Clear error code and buffer
void ClearInput()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Generate a random number between min and max inclusive
int GetRandomRange(int min, int max)
{
	static std::mt19937 engine{ std::random_device{}() };
	std::uniform_int_distribution dist{ min, max };

	return dist(engine);
}

// Main logic that returns success or failure for solving the puzzle
bool PlayGame(const int difficulty)
{
    PrintIntroduction(difficulty);

    // Declare three read-only values for the "secret code"
    const int CodeA = GetRandomRange(2, difficulty * 2);
    const int CodeB = GetRandomRange(2, difficulty * 2);
    const int CodeC = GetRandomRange(2, difficulty * 2);

    PrintCodesDebug(CodeA, CodeB, CodeC);

    // Calculate the sum and product for the "secret code" clues
    const int CodeProduct = CodeA * CodeB * CodeC;
    const int CodeSum = CodeA + CodeB + CodeC;

    PrintCluesAndPrompt(CodeProduct, CodeSum);

    // Declare variables and capture user input
    int GuessA, GuessB, GuessC;
    std::cin >> GuessA >> GuessB >> GuessC;

    // Perform result calculations
    const int GuessProduct = GuessA * GuessB * GuessC;
    const int GuessSum = GuessA + GuessB + GuessC;

    // PrintGuessDebug(GuessA, GuessB, GuessC, GuessProduct, GuessSum);

    // Test if guess calculations are the same as code calculations
    if (GuessProduct == CodeProduct && GuessSum == CodeSum)
    {
        std::cout << "You guessed correctly! Get ready for the next level..." << std::endl << std::endl;
        return true;
    }
    else
    {
        std::cout << "These are the wrong numbers! Try again!" << std::endl << std::endl;
        return false;
    }    
}

int main()
{
    //Game configuration settings
    const int MaxDifficulty = 6;
    int LevelDifficulty = 2;

    // Play the game until you've beat all the levels
    while (LevelDifficulty <= MaxDifficulty)
    {
        bool bLevelComplete = PlayGame(LevelDifficulty);

        ClearInput();

        // If you guess correctly, go up a level
        if (bLevelComplete)
        {
            LevelDifficulty++;
        }
    }

    // You've won, let the player know before exiting
    std::cout << "You hacked the server room! You win!" << std::endl;

    return 0;
}
