#define OLC_PGE_APPLICATION
#include "../vendor/olcPixelGameEngine/olcPixelGameEngine.h"

#include <exception>

/*
	Problems:

		1: The piece can cement itself without actually falling, only needing something to be under it.
		2: Infinite new pieces and Infinite amount of cementing occurs when your cemented blocks reach the spawn location.
		3: The pieces can clip into other pieces by moving right or left until the piece has something below it.
		3-fix: Instead of checking if it would move off-screen, check if it will move into something other than the BACKGROUND_COLOR
				Similiar to the checking of if there's something below the piece to see if it finished falling but horizontially.

		4: Restructure cementing the pieces to not just cement where they are.
			They'll be cemented at the same x coordinate. But the y coordinate
			may change.

		5: Need more random pieces as currently the same seed is used for rand().
*/

const olc::Pixel BACKGROUND_COLOR = olc::BLUE;

olc::Pixel board[20][10];
const int ROW_LENGTH = 20;
const int COL_LENGTH = 10;
int score = 0;

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
			olc::Pixel color;

			Shape()
			{
				color = olc::Pixel(rand() % 155 + 100, rand() % 155 + 100, rand() % 155 + 10);

				// :)
				int value = rand() % 7;
				switch (value)
				{
				case 0:
					coords[0][0] = BACKGROUND_COLOR; coords[0][1] = BACKGROUND_COLOR; coords[0][2] = BACKGROUND_COLOR; coords[0][3] = BACKGROUND_COLOR;
					coords[1][0] = BACKGROUND_COLOR; coords[1][1] = color;            coords[1][2] = color;            coords[1][3] = BACKGROUND_COLOR;
					coords[2][0] = BACKGROUND_COLOR; coords[2][1] = color;            coords[2][2] = color;            coords[2][3] = BACKGROUND_COLOR;
					coords[3][0] = BACKGROUND_COLOR; coords[3][1] = BACKGROUND_COLOR; coords[3][2] = BACKGROUND_COLOR; coords[3][3] = BACKGROUND_COLOR;
					break;
				case 1:
					coords[0][0] = BACKGROUND_COLOR; coords[0][1] = BACKGROUND_COLOR; coords[0][2] = BACKGROUND_COLOR; coords[0][3] = BACKGROUND_COLOR;
					coords[1][0] = color;            coords[1][1] = color;            coords[1][2] = BACKGROUND_COLOR; coords[1][3] = BACKGROUND_COLOR;
					coords[2][0] = BACKGROUND_COLOR; coords[2][1] = color;            coords[2][2] = color;            coords[2][3] = BACKGROUND_COLOR;
					coords[3][0] = BACKGROUND_COLOR; coords[3][1] = BACKGROUND_COLOR; coords[3][2] = BACKGROUND_COLOR; coords[3][3] = BACKGROUND_COLOR;
					break;
				case 2:
					coords[0][0] = BACKGROUND_COLOR; coords[0][1] = BACKGROUND_COLOR; coords[0][2] = BACKGROUND_COLOR; coords[0][3] = BACKGROUND_COLOR;
					coords[1][0] = BACKGROUND_COLOR; coords[1][1] = color;            coords[1][2] = color;            coords[1][3] = BACKGROUND_COLOR;
					coords[2][0] = color;            coords[2][1] = color;            coords[2][2] = BACKGROUND_COLOR; coords[2][3] = BACKGROUND_COLOR;
					coords[3][0] = BACKGROUND_COLOR; coords[3][1] = BACKGROUND_COLOR; coords[3][2] = BACKGROUND_COLOR; coords[3][3] = BACKGROUND_COLOR;
					break;
				case 3:
					coords[0][0] = BACKGROUND_COLOR; coords[0][1] = BACKGROUND_COLOR; coords[0][2] = BACKGROUND_COLOR; coords[0][3] = BACKGROUND_COLOR;
					coords[1][0] = color;            coords[1][1] = color;            coords[1][2] = color;            coords[1][3] = BACKGROUND_COLOR;
					coords[2][0] = BACKGROUND_COLOR; coords[2][1] = BACKGROUND_COLOR; coords[2][2] = color;            coords[2][3] = BACKGROUND_COLOR;
					coords[3][0] = BACKGROUND_COLOR; coords[3][1] = BACKGROUND_COLOR; coords[3][2] = BACKGROUND_COLOR; coords[3][3] = BACKGROUND_COLOR;
					break;
				case 4:
					coords[0][0] = BACKGROUND_COLOR; coords[0][1] = BACKGROUND_COLOR; coords[0][2] = BACKGROUND_COLOR; coords[0][3] = BACKGROUND_COLOR;
					coords[1][0] = color;            coords[1][1] = color;            coords[1][2] = color;            coords[1][3] = BACKGROUND_COLOR;
					coords[2][0] = color;            coords[2][1] = BACKGROUND_COLOR; coords[2][2] = BACKGROUND_COLOR; coords[2][3] = BACKGROUND_COLOR;
					coords[3][0] = BACKGROUND_COLOR; coords[3][1] = BACKGROUND_COLOR; coords[3][2] = BACKGROUND_COLOR; coords[3][3] = BACKGROUND_COLOR;
					break;
				case 5:
					coords[0][0] = BACKGROUND_COLOR; coords[0][1] = BACKGROUND_COLOR; coords[0][2] = BACKGROUND_COLOR; coords[0][3] = BACKGROUND_COLOR;
					coords[1][0] = color;            coords[1][1] = color;            coords[1][2] = color;            coords[1][3] = BACKGROUND_COLOR;
					coords[2][0] = BACKGROUND_COLOR; coords[2][1] = color;            coords[2][2] = BACKGROUND_COLOR; coords[2][3] = BACKGROUND_COLOR;
					coords[3][0] = BACKGROUND_COLOR; coords[3][1] = BACKGROUND_COLOR; coords[3][2] = BACKGROUND_COLOR; coords[3][3] = BACKGROUND_COLOR;
					break;
				case 6:
					coords[0][0] = BACKGROUND_COLOR; coords[0][1] = BACKGROUND_COLOR; coords[0][2] = BACKGROUND_COLOR; coords[0][3] = BACKGROUND_COLOR;
					coords[1][0] = color;            coords[1][1] = color;            coords[1][2] = color;            coords[1][3] = color;
					coords[2][0] = BACKGROUND_COLOR; coords[2][1] = BACKGROUND_COLOR; coords[2][2] = BACKGROUND_COLOR; coords[2][3] = BACKGROUND_COLOR;
					coords[3][0] = BACKGROUND_COLOR; coords[3][1] = BACKGROUND_COLOR; coords[3][2] = BACKGROUND_COLOR; coords[3][3] = BACKGROUND_COLOR;
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
		GameStatus currentStatus = GameStatus::RUNNING;
		Shape *currentPiece;

		float delay_until_fall = 10001.5f;
		float timer_until_fall;

		float move_cd = 0.07f;
		float timer_move_left = move_cd;

		float fall_cd = 0.32f;
		float timer_fall_left = fall_cd;

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

		GameStatus GetGameStatus()
		{
		//	if (currentStatus == GameStatus::OVER)
		//		return currentStatus;

			if (GetKey(olc::Key::P).bPressed)
			{
				if (currentStatus == GameStatus::PAUSED)
					currentStatus = GameStatus::RUNNING;
				else
					currentStatus = GameStatus::PAUSED;
			}
			return currentStatus;
		}

		inline void GameRunning(float fElapsedTime)
		{
			timer_move_left -= fElapsedTime;
			timer_fall_left -= fElapsedTime;

			// Current Piece controls
			if (timer_move_left < 0 && GetKey(olc::Key::A).bHeld && CanMoveHorizontallyLeft(currentPiece))
			{
				currentPiece->MoveLeft();
				timer_move_left = move_cd;
			}
			else if (timer_move_left < 0 && GetKey(olc::Key::D).bHeld && CanMoveHorizontallyRight(currentPiece))
			{
				currentPiece->MoveRight();
				timer_move_left = move_cd;
			}

			if (GetKey(olc::Key::LEFT).bPressed)
			{
				currentPiece->RotateLeft();
				FixClipping(currentPiece);
			}
			else if (GetKey(olc::Key::RIGHT).bPressed)
			{
				currentPiece->RotateRight();
				FixClipping(currentPiece);
			}

			if (GetKey(olc::Key::SPACE).bHeld && timer_fall_left < 0)
			{
				currentPiece->Fall();
				timer_fall_left = fall_cd;
			}

			//Timer for natural piece falling
			timer_until_fall -= fElapsedTime;
			if (timer_until_fall < 0)
			{
				currentPiece->Fall();
				timer_until_fall = delay_until_fall;
			}

			if (FinishedFalling(*currentPiece))
			{
				CementPiece(currentPiece);
				// Only time this should be checked.
				CheckForFullLines();
				// The old piece is long gone
				// from the players control.
				NewPiece();
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
					if (pieceColor != BACKGROUND_COLOR)
						FillRect((currentPiece->xOffset + col) * 20, (currentPiece->yOffset + row) * 20, 20, 20, pieceColor);
				}
			}
			// Drawing the score last so that 
			// it will be on top of everything else.
			DrawString(5, 5, std::to_string(score));
		}

		inline void GamePaused()
		{
			// Pain. Don't know how to figure out font width and height
			// to just calculate the center of this text and center
			// it on the screen.
			DrawString(32, 180, "\t PAUSED\nPress p to unpause");
		}

		inline void GameOver()
		{
			DrawString(32, 180, "GAME OVER");
		}

		void NewPiece()
		{
			currentPiece = new Shape();
			timer_until_fall = delay_until_fall;
		}
	
		bool FinishedFalling(Shape piece)
		{
			for (int col = 0;col < piece.WIDTH; col++)
			{
				for (int row = piece.HEIGHT - 1; row >= 0; row--)
				{
					if (piece.coords[row][col] != BACKGROUND_COLOR)
					{
						if (board[piece.yOffset + row + 1][piece.xOffset + col] != BACKGROUND_COLOR)
							return true;
						break;
					}
				}
			}
			return false;
		//	return piece.GetMaxY() >= ROW_LENGTH - 1;
		}

		void CementPiece(Shape *currentPiece)
		{
			int xOffset = currentPiece->xOffset;
			int yOffset = currentPiece->yOffset;

			int minX = currentPiece->GetMinX();
			int maxX = currentPiece->GetMaxX();
			int maxY = currentPiece->GetMaxY();
			
			// Checks the bottom row of the tetris piece
			// of if it's overlapping anything in the board
			// other than the background color.
			LOOP:if (maxY <= 0)
			{
				currentStatus = GameStatus::OVER;
				// No need to cement the piece because
				// well it won't cement it either way.
				// Just saving the trouble of a possibility.
				return;
			}
			else
			{
				for (int x = minX; x < maxX; x++)
				{
					// If it overlaps something, restart with
					// the whole piece higher by one row.
					if (board[maxY][x] != BACKGROUND_COLOR && currentPiece->coords[maxY - yOffset][x - xOffset] != BACKGROUND_COLOR)
					{
						yOffset--;
						maxY--;
						goto LOOP;
					}
				}
			}

			// Cements the piece into the board
			for (int y = 0; y < currentPiece->HEIGHT; y++)
			{
				for (int x = (minX - xOffset); x <= (maxX - xOffset); x++)
				{
					olc::Pixel pieceColor = currentPiece->coords[y][x];
					if (pieceColor != BACKGROUND_COLOR)
						board[y + yOffset][x + xOffset] = pieceColor;
				}
			}
			delete currentPiece;
		}

		/*
			Checks each row if it's full.
			And if a full row is found, it
			clears that line.
		*/
		void CheckForFullLines()
		{
			for (int row = 0; row < ROW_LENGTH; row++)
			{
				if (IsFullLine(row))
				{
					RemoveLine(row);
				}
			}
		}

		/*
			Checks the specified row index
			if the whole row is filled with
			stuff.
		*/
		bool IsFullLine(byte row)
		{
			for (int col = 0; col < COL_LENGTH; col++)
			{
				if (board[row][col] == BACKGROUND_COLOR)
					return false;
			}
			return true;
		}

		/*
			Removes a full line specified by the row
			index and rewards the player by increasing
			the score.
		*/
		inline void RemoveLine(byte row)
		{
		//	for (int col = 0; col < COL_LENGTH; col++)
		//	{
		//		board[row][col] = BACKGROUND_COLOR;
		//	}
			// We don't need to actually remove the row
			// because it will be removed by the gravity
			// hehehaha
			ApplyGravity(row);
			score += 500;
		}

		bool IsGameOver()
		{

		}

		/*
			Called right after a single
			line is removed

			the value row being the row
			index of where the line
			was removed.
		*/
		inline void ApplyGravity(byte row)
		{
			for (int y = row; y > 0; y--)
			{
				for (int col = 0; col < COL_LENGTH; col++)
				{
					board[y][col] = board[y - 1][col];
				}
			}
		}

		/*
			Clears the board to the background color
			as the default value inside the array is 
			probably not the background color

			Also can be used when restarting the game
		*/
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

		
		/*
			Fixes the clipping of a piece of the piece
			being outside of the board.
		*/
		void FixClipping(Shape *currentPiece)
		{
			while (currentPiece->GetMinX() < 0)
				currentPiece->MoveRight();
			while (currentPiece->GetMaxX() >= COL_LENGTH)
				currentPiece->MoveLeft();
		}

		/*
			Checks if you can move left without colliding
			with the border or colliding with another
			tetris piece.
		*/
		bool CanMoveHorizontallyLeft(Shape *currentPiece)
		{
			if (currentPiece->GetMinX() == 0)
				return false;

			for (int row = 0; row < currentPiece->HEIGHT; row++)
			{
				for (int col = 0; col < currentPiece->WIDTH; col++)
				{
					if (currentPiece->coords[row][col] != BACKGROUND_COLOR)
					{
						if (board[row + currentPiece->yOffset][col + currentPiece->xOffset - 1] != BACKGROUND_COLOR)
						{
							return false;
						}
						break;
					}
				}
			}
			return true;
		}

		/*
			Checks if you can move right without colliding
			with the border or colliding with another
			tetris piece.
		*/
		bool CanMoveHorizontallyRight(Shape * currentPiece)
		{
			if (currentPiece->GetMaxX() == (COL_LENGTH - 1))
				return false;

			for (int row = 0; row < currentPiece->HEIGHT; row++)
			{
				for (int col = currentPiece->WIDTH - 1; col >= 0; col--)
				{
					if (currentPiece->coords[row][col] != BACKGROUND_COLOR)
					{
						if (board[row + currentPiece->yOffset][col + currentPiece->xOffset + 1] != BACKGROUND_COLOR)
						{
							return false;
						}
						break;
					}
				}
			}
			return true;
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