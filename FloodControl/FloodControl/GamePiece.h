#pragma once

#include "pch.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

namespace DX
{
	class GamePiece
	{
	public:
		vector<string> PieceTypes = {
			"Left,Right",
			"Top,Bottom",
			"Left,Top",
			"Top,Right",
			"Right,Bottom",
			"Bottom,Left",
			"Empty"
		};

		int const PieceHeight = 40;
		int const PieceWidth = 40;
		int const MaxPlayablePieceIndex = 5;
		int const EmptyPieceIndex = 6;

		GamePiece(string type, string suffix)
		{
			pieceType = type;
			pieceSuffix = suffix;
		}
		GamePiece(string type)
		{
			pieceType = type;
			pieceSuffix = "";
		}
		GamePiece()
		{
			pieceType = "Empty";
			pieceSuffix = "";
		}
		~GamePiece() { /**/ }

		GamePiece* InitPiece()
		{
			GamePiece* piece = new GamePiece("Empty");
			return piece;
		}

		string GetPieceType()
		{
			return pieceType;
		}
		string GetPieceSuffix()
		{
			return pieceSuffix;
		}

		void SetPiece(string type, string suffix)
		{
			pieceType = type;
			pieceSuffix = suffix;
		}
		void SetPiece(string type)
		{
			SetPiece(type, "");
		}

		void AddSuffix(string suffix)
		{
			if (!pieceSuffix.find(suffix))
			{
				pieceSuffix += suffix;
			}
		}
		void RemoveSuffix(string suffix)
		{
			pieceSuffix.replace(pieceSuffix.find(suffix), suffix.length(), "");
		}

		void RotatePiece(bool clockwise)
		{
			if (pieceType == "Left,Right")
			{
				pieceType = "Top,Bottom";
			}
			else if (pieceType == "Top,Bottom")
			{
				pieceType = "Left,Right";
			}
			else if (pieceType == "Left,Top")
			{
				if (clockwise)
					pieceType = "Top,Right";
				else
					pieceType = "Bottom,Left";
			}
			else if (pieceType == "Top,Right")
			{
				if (clockwise)
					pieceType = "Right,Bottom";
				else
					pieceType = "Left,Top";
			}
			else if (pieceType == "Right,Bottom")
			{
				if (clockwise)
					pieceType = "Bottom,Left";
				else
					pieceType = "Top,Right";
			}
			else if (pieceType == "Bottom,Left")
			{
				if (clockwise)
					pieceType = "Left,Top";
				else
					pieceType = "Right,Bottom";
			}
			else if (pieceType == "Empty")
			{
				// Do nothing? Probably only here to avoid the exception in the switch
			}
		}

		void GetOtherEnds(string startingEnd)
		{
			istringstream ss(startingEnd);
			string end;

			while (getline(ss, end, ','))
			{
				if (end != startingEnd)
				{
					opposites.push_back(end);
				}
			}
		}
		bool HasConnector(string direction)
		{
			return (pieceType.find(direction) != string::npos) ? true : false;
		}

		Rectangle GetSourceRect()
		{
			int x = textureOffsetX;
			int y = textureOffsetY;

			if (pieceSuffix.find('W') != string::npos)
			{
				x += PieceWidth + texturePaddingX;
				for (int i = 0; i < PieceTypes.size(); i++)
				{
					if (pieceType == PieceTypes[i])
					{
						y += i * (PieceHeight + texturePaddingY);
					}
				}
				return Rectangle(x, y, PieceWidth, PieceHeight);
			}
			throw ("You should never see this.");
		}
	private:
		string pieceType;
		string pieceSuffix;
		vector<string> opposites;

		int const textureOffsetX = 1;
		int const textureOffsetY = 1;
		int const texturePaddingX = 1;
		int const texturePaddingY = 1;
	};
}
