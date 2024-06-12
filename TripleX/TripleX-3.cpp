#include <tuple>
#include <cassert>
#include <random>
#include <iostream>
#include <locale>

#ifdef _DEBUG
constexpr bool DEBUG_PRINT_ANSWERS = true;
#else
constexpr bool DEBUG_PRINT_ANSWERS = false;
#endif

// Initial game settings
constexpr int STARTING_LEVEL = 2;
constexpr int MAX_LEVEL = 9;
constexpr int MAX_LIVES = 6;

static_assert(STARTING_LEVEL >= 2 && STARTING_LEVEL <= MAX_LEVEL,
	"The starting level must be >= 2 and <= to the max level");


constexpr auto VariadicSum = [](auto ...args) { return (args + ...); };
constexpr auto VariadicProduct = [](auto ...args) { return (args * ...); };

constexpr auto lxPrinter = [](auto ...args) { ((std::wcout << ' ' << args), ...) << std::endl; };

int GetRandomRange(int min, int max)
{
	assert(min < max);

	static std::mt19937 engine{ std::random_device{}() };
	std::uniform_int_distribution dist{ min, max };

	return dist(engine);
}

void PrintIntro()
{
	std::wcout << "Welcome to the XXX program, agent. Your mission, should you choose to accept:" << std::endl;
	std::wcout << "Hack your way to the top level of EvilCorp's server building!" << std::endl;
}

void ClearInputStream()
{
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
};

using Clues = std::tuple<int, int>;
Clues GetClues(int difficulty)
{
	const auto GenerateCode = [difficulty]() { return GetRandomRange(difficulty, difficulty * 2 - 1); };

	const auto codes = std::make_tuple(GenerateCode(), GenerateCode(), GenerateCode());

	const auto Calculate = [codes](auto lambda) { return std::apply(lambda, codes); };

	return { Calculate(VariadicSum), Calculate(VariadicProduct) };
}

using Guesses = std::tuple<int, int, int>;
Guesses GetGuesses()
{
	int a, b, c;

	while (!(std::wcin >> a >> b >> c)) // Not totally error proof.  123\r 1 2 3 == 123, 1, 2
	{
		std::wcout << "Please enter three numbers..." << std::endl;
		std::wcout << "~nixadm> ";

		ClearInputStream();
	}

	ClearInputStream();

	return { a, b, c };
}

bool IsLevelWon(int difficulty)
{
	// GetTargets returns a tuple of two ints. [target_sum, target_product] will be the two variables that contain the returned int values.
	const auto [clues_sum, clues_product] = GetClues(difficulty);

	std::wcout << std::endl;
	std::wcout << "You are attempting to hack the entry code for the level " << difficulty << " server room!" << std::endl;
	std::wcout << "Your SuperHacker 2000 tells you the following information..." << std::endl;
	std::wcout << std::endl;
	std::wcout << "\tThere are 3 numbers in the code" << std::endl;
	std::wcout << "\tThe product of the numbers is " << clues_product << std::endl;
	std::wcout << "\tThe sum of the numbers is " << clues_sum << std::endl;
	std::wcout << std::endl;
	std::wcout << "Enter the code numbers:" << std::endl;
	std::wcout << "~nixadm> ";

	const auto guesses = GetGuesses();

	const auto Calculate = [guesses](auto lambda) { return std::apply(lambda, guesses); };

	return Calculate(VariadicSum) == clues_sum && Calculate(VariadicProduct) == clues_product;
}

void PlayGame()
{
	auto lives = MAX_LIVES;
	
	for (auto level = STARTING_LEVEL; level <= MAX_LEVEL && lives > 0;)
	{
		if (IsLevelWon(level))
		{
			std::wcout << std::endl;
			std::wcout << "Success! You have hacked the code for level " << level++ << "!" << std::endl;

			if (level != MAX_LEVEL + 1)
			{
				std::wcout << "The system has allowed you access to the next level..." << std::endl;
			}
		}
		else
		{
			std::wcout << std::endl;
			std::wcout << "Failure! You have failed to hack the code!" << std::endl;

			if (level == 2 && lives > 1)
			{
				std::wcout << "The system will allow you to try again!" << std::endl;
			}
			else if (lives > 1)
			{
				std::wcout << "The system has returned you to level " << --level << "... Try again!" << std::endl;
			}

			std::wcout << "You have " << --lives << " failures remaining before security is alerted!" << std::endl;
		}
	}

	if (lives > 0)
	{
		std::wcout << "You have breeched the EvilCorp primary data center! You beat the game!" << std::endl;
	}
	else
	{
		std::wcout << "You have been captured by security! Better luck next time!" << std::endl;
	}
}

bool PlayAgain()
{
	std::wcout << std::endl;
	std::wcout << "Do you want to play a game (y/n)? ";
	
	wchar_t answer;
	while (!(std::wcin >> answer) || towlower(answer) != 'y' && towlower(answer) != 'n')
	{
		std::wcout << "How about a nice game of chess? Please enter only \'y\' or \'n\': ";

		ClearInputStream();
	}

	ClearInputStream();
	
	return towlower(answer) == 'y';
}

int wmain(int argc, wchar_t argv[])
{
#pragma warning(push)
#pragma warning(disable : 26444) // Ignore es.84 for locale setting
	std::locale::global(std::locale{ "en_US.utf-8" });
#pragma warning(pop)

	PrintIntro();

	do {
		PlayGame();
	} while (PlayAgain());
	
	return 0;
}
