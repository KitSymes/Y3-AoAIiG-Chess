#include "ChessPlayer.h"
#include "Chess\GameStatus.h"
#include "Chess\Gameplay.h"
#include "Chess\Board.h"
#include "Chess\Piece.h"

using namespace std;



void ChessPlayer::setupPlayers(ChessPlayer** playerWhite, ChessPlayer** playerBlack, Board* pBoard, GameStatus* pGameStatus, Gameplay* pGamePlay)
{
	*playerBlack = new ChessPlayer(pBoard, pGameStatus, pGamePlay, PieceColor::BLACK);
	(*playerBlack)->setAI();

	*playerWhite = new ChessPlayer(pBoard, pGameStatus, pGamePlay, PieceColor::WHITE);
	(*playerWhite)->setAI();

}

ChessPlayer::ChessPlayer(Board* pBoard, GameStatus* pGameStatus, Gameplay* pGamePlay, PieceColor colour)
{
	m_colour = colour;
	m_pBoard = pBoard;
	m_pGameStatus = pGameStatus;
	m_pGamePlay = pGamePlay;
	m_bAI = false;
}

unsigned int ChessPlayer::getAllLivePieces(vecPieces& vpieces)
{
	vpieces.clear();

	PieceInPostion pip;

	unsigned int count = 0;
	for (int i = m_pBoard->MIN_ROW_INDEX; i < m_pBoard->MAX_ROW_INDEX; i++)
	{
		for (int j = m_pBoard->MIN_COL_INDEX; j < m_pBoard->MAX_COL_INDEX; j++)
		{
			std::shared_ptr<Piece> pPiece = m_pBoard->getSquare(i, j)->getOccupyingPiece();

			if (pPiece == nullptr)
				continue;
			if (pPiece->getColor() != m_colour)
				continue;

			count++;
			pip.piece = pPiece;
			pip.row = i;
			pip.col = j;
			vpieces.emplace_back(pip);
		}
	}

	return count;
}

unsigned int ChessPlayer::getAllLivePieces(vecPieces& vpieces, Board* board)
{
	vpieces.clear();

	PieceInPostion pip;

	unsigned int count = 0;
	for (int i = board->MIN_ROW_INDEX; i < board->MAX_ROW_INDEX; i++)
	{
		for (int j = board->MIN_COL_INDEX; j < board->MAX_COL_INDEX; j++)
		{
			std::shared_ptr<Piece> pPiece = board->getSquare(i, j)->getOccupyingPiece();

			if (pPiece == nullptr)
				continue;
			if (pPiece->getColor() != m_colour)
				continue;

			count++;
			pip.piece = pPiece;
			pip.row = i;
			pip.col = j;
			vpieces.emplace_back(pip);
		}
	}

	return count;
}

unsigned int ChessPlayer::getAllLiveEnemyPieces(vecPieces& vpieces, Board* board)
{
	vpieces.clear();

	PieceInPostion pip;

	unsigned int count = 0;
	for (int i = board->MIN_ROW_INDEX; i < board->MAX_ROW_INDEX; i++)
	{
		for (int j = board->MIN_COL_INDEX; j < board->MAX_COL_INDEX; j++)
		{
			std::shared_ptr<Piece> pPiece = board->getSquare(i, j)->getOccupyingPiece();

			if (pPiece == nullptr)
				continue;
			if (pPiece->getColor() == m_colour)
				continue;

			count++;
			pip.piece = pPiece;
			pip.row = i;
			pip.col = j;
			vpieces.emplace_back(pip);
		}
	}

	return count;
}

vector<std::shared_ptr<Move>> ChessPlayer::getValidMovesForPiece(PieceInPostion pip)
{
	return Gameplay::getValidMoves(m_pGameStatus, m_pBoard, pip.piece, pip.row, pip.col);
}

// chooseAIMove
// in this method - for an AI chess player - choose a move to make. This is called once per play. 
bool ChessPlayer::chooseAIMove(std::shared_ptr<Move>* moveToMake)
{
	vecPieces vPieces;
	unsigned int piecesAvailable = getAllLivePieces(vPieces);

	// BAD AI !! - for the first piece which can move, choose the first available move
	/*bool moveAvailable = false;
	int randomPiece;
	while (!moveAvailable)
	{
		randomPiece = rand() % vPieces.size(); // choose a random chess piece
		vector<std::shared_ptr<Move>> moves = getValidMovesForPiece(vPieces[randomPiece]); // get all the valid moves for this piece if any)
		if (moves.size() > 0) // if there is a valid move exit this loop - we have a candidate 
			moveAvailable = true;
	}

	// get all moves for the random piece chosen (yes there is some duplication here...)
	vector<std::shared_ptr<Move>> moves = getValidMovesForPiece(vPieces[randomPiece]);
	if (moves.size() > 0)
	{
		int field = moves.size();
		int randomMove = rand() % field; // for all the possible moves for that piece, choose a random one
		*moveToMake = moves[randomMove]; // store it in 'moveToMake' and return
		return true;
	}*/

	int value = maximise(m_pBoard, m_pGameStatus, moveToMake, 2);
	if (moveToMake != nullptr)
		return true;
	/*int predictedMoves = 0;
	std::shared_ptr<Move> best;
	for (PieceInPostion p : vPieces)
	{
		Board* newBoard = m_pBoard->hardCopy();
		auto moves = getValidMovesForPiece(p);
		for (auto m : moves)
		{
			int newMovesAvailable = 0;

			vecPieces vPieces;
			unsigned int piecesAvailable = getAllLivePieces(vPieces);
		}
	}*/

	return false; // if there are no moves to make return false
}

int ChessPlayer::maximise(Board* board, GameStatus* status, std::shared_ptr<Move>* move, int depthLimit)
{
	int max = -9999;

	if (Gameplay::isCheckMateState(status, board, m_colour))
		return max;

	if (depthLimit <= 0)
		return CalculateValue(board, status);

	vecPieces vPieces;
	unsigned int piecesAvailable = getAllLivePieces(vPieces, board);

	for (auto p : vPieces)
	{
		auto moves = getValidMovesForPiece(p);
		for (auto m : moves)
		{
			Board* newBoard = board->hardCopy();
			GameStatus* newStatus = new GameStatus(*status);
			Gameplay::move(newStatus, newBoard, m);
			std::shared_ptr<Move> nextMove;
			int value = minimise(newBoard, newStatus, &nextMove, depthLimit - 1);

			if (value > max)
			{
				max = value;
				*move = m;
			}

			delete newStatus;
			delete newBoard;
		}
	}

	return max;
}

int ChessPlayer::minimise(Board* board, GameStatus* status, std::shared_ptr<Move>* move, int depthLimit)
{
	int min = 9999;

	if (Gameplay::isCheckMateState(status, board, m_colour == PieceColor::WHITE ? PieceColor::BLACK : PieceColor::WHITE))
		return min;

	if (depthLimit <= 0)
		return CalculateValue(board, status);

	vecPieces vPieces;
	unsigned int piecesAvailable = getAllLiveEnemyPieces(vPieces, board);

	for (auto p : vPieces)
	{
		auto moves = getValidMovesForPiece(p);
		for (auto m : moves)
		{
			Board* newBoard = board->hardCopy();
			GameStatus* newStatus = new GameStatus(*status);
			Gameplay::move(newStatus, newBoard, m);
			std::shared_ptr<Move> nextMove;
			int value = maximise(newBoard, newStatus, &nextMove, depthLimit - 1);

			if (value < min)
			{
				min = value;
				*move = m;
			}

			delete newStatus;
			delete newBoard;
		}
	}

	return min;
}

int ChessPlayer::CalculateValue(Board* board, GameStatus* status)
{
	return 0;
}