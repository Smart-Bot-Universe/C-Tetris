#define OLC_PGE_APPLICATION
#include "../vendor/olcPixelGameEngine/olcPixelGameEngine.h"

class Tetris : public olc::PixelGameEngine
{
	enum GameStatus
	{
		RUNNING, PAUSED, OVER
	};

	class Shape
	{
		public:
			const int WIDTH = 4, HEIGHT = 4;
			olc::Pixel coords[4][4];

			int xOffset = 3, yOffset = 0;
			olc::Pixel color = olc::WHITE;

			Shape()
			{
				olc::Pixel color = olc::Pixel(rand() % 155 + 100, rand() % 155 + 100, rand() % 155 + 10);

				// :)
				switch (rand() % 7)
				{
				case 0:
					coords[0][0] = olc::BLACK; coords[0][1] = olc::BLACK; coords[0][2] = olc::BLACK; coords[0][3] = olc::BLACK;
					coords[1][0] = olc::BLACK; coords[1][1] = color;      coords[1][2] = color;      coords[1][3] = olc::BLACK;
					coords[2][0] = olc::BLACK; coords[2][1] = color;      coords[2][2] = color;      coords[2][3] = olc::BLACK;
					coords[3][0] = olc::BLACK; coords[3][1] = olc::BLACK; coords[3][2] = olc::BLACK; coords[3][3] = olc::BLACK;
					break;
				case 1:
					coords[0][0] = olc::BLACK; coords[0][1] = olc::BLACK; coords[0][2] = olc::BLACK; coords[0][3] = olc::BLACK;
					coords[1][0] = color;      coords[1][1] = color;      coords[1][2] = olc::BLACK; coords[1][3] = olc::BLACK;
					coords[2][0] = olc::BLACK; coords[2][1] = color;      coords[2][2] = color;      coords[2][3] = olc::BLACK;
					coords[3][0] = olc::BLACK; coords[3][1] = olc::BLACK; coords[3][2] = olc::BLACK; coords[3][3] = olc::BLACK;
					break;
				case 2:
					coords[0][0] = olc::BLACK; coords[0][1] = olc::BLACK; coords[0][2] = olc::BLACK; coords[0][3] = olc::BLACK;
					coords[1][0] = olc::BLACK; coords[1][1] = color;      coords[1][2] = color;      coords[1][3] = olc::BLACK;
					coords[2][0] = color;      coords[2][1] = color;      coords[2][2] = olc::BLACK; coords[2][3] = olc::BLACK;
					coords[3][0] = olc::BLACK; coords[3][1] = olc::BLACK; coords[3][2] = olc::BLACK; coords[3][3] = olc::BLACK;
					break;
				case 3:
					coords[0][0] = olc::BLACK; coords[0][1] = olc::BLACK; coords[0][2] = olc::BLACK; coords[0][3] = olc::BLACK;
					coords[1][0] = color;      coords[1][1] = color;      coords[1][2] = color;      coords[1][3] = olc::BLACK;
					coords[2][0] = olc::BLACK; coords[2][1] = olc::BLACK; coords[2][2] = color;      coords[2][3] = olc::BLACK;
					coords[3][0] = olc::BLACK; coords[3][1] = olc::BLACK; coords[3][2] = olc::BLACK; coords[3][3] = olc::BLACK;
					break;
				case 4:
					coords[0][0] = olc::BLACK; coords[0][1] = olc::BLACK; coords[0][2] = olc::BLACK; coords[0][3] = olc::BLACK;
					coords[1][0] = color;      coords[1][1] = color;      coords[1][2] = color;      coords[1][3] = olc::BLACK;
					coords[2][0] = color;      coords[2][1] = olc::BLACK; coords[2][2] = olc::BLACK; coords[2][3] = olc::BLACK;
					coords[3][0] = olc::BLACK; coords[3][1] = olc::BLACK; coords[3][2] = olc::BLACK; coords[3][3] = olc::BLACK;
					break;
				case 5:
					coords[0][0] = olc::BLACK; coords[0][1] = olc::BLACK; coords[0][2] = olc::BLACK; coords[0][3] = olc::BLACK;
					coords[1][0] = color;      coords[1][1] = color;      coords[1][2] = color;      coords[1][3] = olc::BLACK;
					coords[2][0] = olc::BLACK; coords[2][1] = color;      coords[2][2] = olc::BLACK; coords[2][3] = olc::BLACK;
					coords[3][0] = olc::BLACK; coords[3][1] = olc::BLACK; coords[3][2] = olc::BLACK; coords[3][3] = olc::BLACK;
					break;
				case 6:
					coords[0][0] = olc::BLACK; coords[0][1] = olc::BLACK; coords[0][2] = olc::BLACK; coords[0][3] = olc::BLACK;
					coords[1][0] = color;      coords[1][1] = color;      coords[1][2] = color;      coords[1][3] = color;
					coords[2][0] = olc::BLACK; coords[2][1] = olc::BLACK; coords[2][2] = olc::BLACK; coords[2][3] = olc::BLACK;
					coords[3][0] = olc::BLACK; coords[3][1] = olc::BLACK; coords[3][2] = olc::BLACK; coords[3][3] = olc::BLACK;
					break;
				}
			}

			inline void RotateLeft()
			{
				olc::Pixel rotatedCoords[4][4];

				// ((0 + 4 + 4 + 0) / 4) * 2 = 4
				// Since arrays start at index 0, we make it three
				int medianTimesTwo = 3;
				for (int x = 0; x < WIDTH; x++)
				{
					for (int y = 0; y < HEIGHT; y++)
					{
						// -0 -1 -2 -3
						rotatedCoords[-x + medianTimesTwo][y] = coords[y][x];
					}
				}
				// Currently don't know how else to change the reference
				// So I'll just manually transfer the values.
				for (int row = 0; row < WIDTH; row++)
				{
					for (int col = 0; col < HEIGHT; col++)
					{
						coords[row][col] = rotatedCoords[row][col];
					}
				}
				// Non of this because it will prob be faster 
				// to just override the previous coordinates.
			//	delete rotatedCoords;
			}

			inline void MoveLeft()
			{
				xOffset--;
			}

			inline void RotateRight()
			{
				olc::Pixel rotatedCoords[4][4];

				// ((0 + 4 + 4 + 0) / 4) * 2 = 4
				// Since arrays start at index 0, we make it three
				int medianTimesTwo = 3;
				for (int x = 0; x < WIDTH; x++)
				{
					for (int y = 0; y < HEIGHT; y++)
					{
						// -0 -1 -2 -3
						rotatedCoords[x][-y + medianTimesTwo] = coords[y][x];
					}
				}
				// Currently don't know how else to change the reference
				// So I'll just manually transfer the values.
				for (int row = 0; row < WIDTH; row++)
				{
					for (int col = 0; col < HEIGHT; col++)
					{
						coords[row][col] = rotatedCoords[row][col];
					}
				}
				// Non of this because it will prob be faster 
				// to just override the previous coordinates.
			//	delete rotatedCoords;
			}

			inline void MoveRight()
			{
				xOffset++;
			}

			inline void Fall()
			{
				yOffset++;
			}
			
			// Some if not of these coord methods are buggy. Should def be double checked.
			int GetMinX()
			{
				for (int col = 0; col < WIDTH; col++)
				{
					for (int row = 0; row < HEIGHT; row++)
					{
						if (coords[row][col] == color)
							return xOffset + col;
					}
				}
				return xOffset;
			}

			int GetMaxX()
			{
				for (int col = WIDTH-1; col >= 0; col--)
				{
					for (int row = 0; row < HEIGHT; row++)
					{
						if (coords[row][col] == color)
							return xOffset + col;
					}
				}
				return xOffset + WIDTH - 1;
			}

			int GetMinY()
			{
				for (int row = 0; row < HEIGHT; row++)
				{
					for (int col = 0; col < WIDTH; col++)
					{
						if (coords[row][col] == color)
							return yOffset + row;
					}
				}
				return yOffset;
			}

			// Prob buggy
			int GetMaxY()
			{
				for (int row = HEIGHT-1; row >= 0; row--)
				{
					for (int col = 0; col < WIDTH; col++)
					{
						if (coords[row][col] == color)
							return yOffset + row;
					}
				}
				return yOffset + HEIGHT - 1;
			}
	};

	public:
		olc::Pixel board[20][10];
		const int ROW_LENGTH = 20;
		const int COL_LENGTH = 10;
		const olc::Pixel BACKGROUND_COLOR = olc::BLUE;

		Shape *currentPiece;
		GameStatus currentStatus = GameStatus::RUNNING;
		int score = 0;

		float delay_until_fall = 0.4f;
		float time_until_fall;

		bool OnUserCreate() override
		{
			NewPiece();
			ClearBoard(0, 0, COL_LENGTH, ROW_LENGTH);
			return true;
		}

		bool OnUserUpdate(float fElapsedTime) override
		{
			switch (GetGameStatus())
			{
				case GameStatus::RUNNING:
					GameRunning(fElapsedTime); break;
				case GameStatus::PAUSED:
					GamePaused(); break;
				case GameStatus::OVER:
					GameOver(); break;
			}
			return true;
		}

		inline void GameRunning(float fElapsedTime)
		{
			// Current Piece controls
			if (GetKey(olc::Key::A).bPressed && currentPiece->GetMinX() != 0)
				currentPiece->MoveLeft();
			else if (GetKey(olc::Key::D).bPressed && currentPiece->GetMaxX() != COL_LENGTH)
				currentPiece->MoveRight();

			if (GetKey(olc::Key::LEFT).bPressed)
				currentPiece->RotateLeft();
			else if (GetKey(olc::Key::RIGHT).bPressed)
				currentPiece->RotateRight();

			// Checking if the piece fell down first so as to not create any weird bugs.
			if (FinishedFalling(*currentPiece))
			{
				CementPiece(currentPiece);
				NewPiece();
			}
			else
			{
				// Current Piece control to fall down 
				if (GetKey(olc::Key::SPACE).bHeld)
					currentPiece->Fall();

				//Timer for natural piece falling
				time_until_fall -= fElapsedTime;
				if (time_until_fall < 0)
				{
					currentPiece->Fall();
					time_until_fall = delay_until_fall;
				}
			}

			// Drawing the board
			for (int row = 0; row < ROW_LENGTH; row++)
			{
				for (int col = 0; col < COL_LENGTH; col++)
				{
					FillRect(col * 20, row * 20, 20, 20, board[row][col]);
				}
			}
			
			// Drawing the current piece
			for (int row = 0; row < currentPiece->HEIGHT; row++)
			{
				for (int col = 0; col < currentPiece->WIDTH; col++)
				{
					olc::Pixel pieceColor = currentPiece->coords[row][col];
				//	if (pieceColor != olc::BLACK)
						FillRect((currentPiece->xOffset + col) * 20, (currentPiece->yOffset + row) * 20, 20, 20, pieceColor);
				}
			}
		}

		inline void GamePaused()
		{

		}

		inline void GameOver()
		{

		}

		GameStatus GetGameStatus()
		{
			if (GetKey(olc::Key::P).bPressed)
			{
				if (currentStatus == GameStatus::PAUSED)
					currentStatus = GameStatus::RUNNING;
				else
					currentStatus = GameStatus::PAUSED;
			}
			return currentStatus;
		}

		void NewPiece()
		{
			currentPiece = new Shape();
			time_until_fall = delay_until_fall;
		}
	
		bool FinishedFalling(Shape piece)
		{
			return piece.GetMaxY() == ROW_LENGTH;
		}

		void CementPiece(Shape *piece)
		{
			for (int row = 0; row < piece->HEIGHT; row++)
			{
				for (int col = 0; col < piece->WIDTH; col++)
				{
					olc::Pixel pieceColor = piece->coords[row][col];
					if (pieceColor != olc::BLACK)
						board[piece->yOffset + row][piece->xOffset + col] = pieceColor;
				}
			}
			delete piece;
		}

		void checkForFullLines()
		{
			for (int row = 0; row < ROW_LENGTH; row++)
			{
				if (isFullLine(row))
				{

				}
			}
		}

		bool isFullLine(byte row)
		{

		}

		void removeLine(byte row)
		{
			for (int col = 0; col < COL_LENGTH; col++)
			{
				
			}
		}

		bool isGameOver()
		{

		}

		void gravity()
		{

		}

		void ClearBoard(int x, int y, int width, int height)
		{
			for (int row = y; row < y + height; row++)
			{
				for (int col = x; col < x + width; col++)
				{
					board[row][col] = BACKGROUND_COLOR;
				}
			}
		}
};

int main()
{
	Tetris tetris;
	if (tetris.Construct(20 * 10, 20 * 20, 2, 2, false, true, false))
	{
		tetris.Start();
	}
	return 0;
}