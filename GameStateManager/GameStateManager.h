#pragma once

namespace Voxelmancy
{
	enum GameState
	{
		Initializing, // Load initial resources for the application (game menu) --> Initialized
		Initialized, // State to handle transition from loading resources to rendering the game menu
		AppLoading, // Build the application's game menu UI --> AppLoaded
		AppLoaded, // State to handle transition from building the game menu to running it
		AppRunning, // Main game menu, connect, create, save/load, configure audio/video/controls, game settings --> GameLoading or AppUnloading
		GameLoading, // Load game resources for the game, free application resources (game menu) --> GameLoaded
		GameLoaded, // State to handle transition from loading game resources to running it
		GameRunning, // Game is handling user input and running simulation
		GameUnloading, // Game is freeing game resources
		GameUnloaded, // State to hanle transition from game back to app --> AppLoading or AppUnloading
		AppUnloading // Game is freeing all resources
	};

	class GameStateManager
	{
	public:
		GameStateManager() : m_gameState(Initializing), m_nextGameState(Initialized)
		{
			//OMG: That's sweet as shit right there ^
		}
		~GameStateManager()
		{
			// Dispose any resources allocated on the heap by this class
		}

		void Update(float elapsedTime)
		{
			m_lastTime = m_currentTime;
			m_currentTime = elapsedTime;
		}

		GameState GetGameState()
		{
			return m_gameState;
		}

		// Transition state, must provide a new next state
		void SetNextGameState(GameState state)
		{
			if (state != m_nextGameState)
			{
				m_gameState = m_nextGameState;
			}
			m_nextGameState = state;
		}

	private:
		float					m_currentTime;
		float					m_lastTime;

		GameState				m_gameState;
		GameState				m_nextGameState;
	};
}
