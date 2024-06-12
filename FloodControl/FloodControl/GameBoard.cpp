#include "pch.h"
#include "GameBoard.h"

using namespace std;

using namespace DX;
using namespace DirectX;
using namespace DirectX::SimpleMath;

namespace DX
{
	GameBoard::GameBoard()
	{
		InitBoard();
		ClearBoard();
	}

	void GameBoard::InitBoard()
	{
		for (int x = 0; x < GameBoardHeight; ++x)
		{
			for (int y = 0; y < GameBoardWidth; ++y)
			{
				boardSquares[x][y].InitPiece();
			}
		}
	}

	void GameBoard::ClearBoard()
	{
		for (int x = 0; x < GameBoardHeight; ++x)
		{
			for (int y = 0; y < GameBoardWidth; ++y)
			{
				boardSquares[x][y].SetPiece("Empty");
			}
		}
	}

	GameBoard::~GameBoard()
	{
		for (int x = 0; x < GameBoardHeight; ++x)
		{
			for (int y = 0; y < GameBoardWidth; ++y)
			{
				delete boardSquares[x][y];
			}
		}
	}
}