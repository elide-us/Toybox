#pragma once

#include "pch.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

namespace DX
{
	class GameBoard
	{
	public:
		static const int GameBoardWidth = 8;
		static const int GameBoardHeight = 10;

		GameBoard();
		~GameBoard();

		void ClearBoard();
		void InitBoard();

		//std::map<std::string, DX::FallingPiece> fallingPieces;
		//std::map<std::string, DX::RotatingPiece> rotatingPieces;
		//std::map<std::string, DX::FadingPiece> fadingPieces;

	private:
		GamePiece boardSquares[GameBoardHeight][GameBoardWidth];
		vector<Vector2> waterTracker;
	};
}

