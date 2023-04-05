#include "ChessPlayer.h"
#include "Chess\GameStatus.h"
#include "Chess\Gameplay.h"
#include "Chess\Board.h"
#include "Chess\Piece.h"
#include <chrono>
#include <iostream>

using namespace std;



void ChessPlayer::setupPlayers(ChessPlayer** playerWhite, ChessPlayer** playerBlack, Board* pBoard, GameStatus* pGameStatus, Gameplay* pGamePlay)
{
	*playerBlack = new ChessPlayer(pBoard, pGameStatus, pGamePlay, PieceColor::BLACK);
	//(*playerBlack)->setAI();

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

	int alpha = -100000;
	int beta = 100000;

	/*long long total = 0; int count = 0;

	for (int i = 0; i < 10; i++)
	{
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		int value = maximise(m_pBoard, m_pGameStatus, moveToMake, 3, alpha, beta);
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

		std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;
		count++;
		total += std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
	}
	std::cout << "Average: " << (total / count) << std::endl;*/
	int value = maximise(m_pBoard, m_pGameStatus, moveToMake, 3, alpha, beta);
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

	//return false; // if there are no moves to make return false
}

int ChessPlayer::maximise(Board* board, GameStatus* status, std::shared_ptr<Move>* move, int depthLimit, int alpha, int beta)
{
	int max = -99999;

	if (Gameplay::isCheckMateState(status, board, m_colour))
		return max;

	if (depthLimit <= 0)
		return CalculateValue(board, status);

	vecPieces vPieces;
	unsigned int piecesAvailable = getAllLivePieces(vPieces, board);

	for (auto p : vPieces)
	{
		auto moves = Gameplay::getValidMoves(status, board, p.piece, p.row, p.col);
		for (auto m : moves)
		{
			Board* newBoard = board->hardCopy();
			GameStatus* newStatus = new GameStatus(*status);
			Gameplay::move(newStatus, newBoard, m);
			std::shared_ptr<Move> nextMove;
			int value = minimise(newBoard, newStatus, &nextMove, depthLimit - 1, alpha, beta);

			if (value > max)
			{
				max = value;
				*move = m;
			}

			delete newStatus;
			delete newBoard;

			if (max > alpha)
				alpha = max;

			if (max >= beta)
				return max;
		}
	}

	return max;
}

int ChessPlayer::minimise(Board* board, GameStatus* status, std::shared_ptr<Move>* move, int depthLimit, int alpha, int beta)
{
	int min = 99999;

	if (Gameplay::isCheckMateState(status, board, m_colour == PieceColor::WHITE ? PieceColor::BLACK : PieceColor::WHITE))
		return min;

	if (depthLimit <= 0)
		return CalculateValue(board, status);

	vecPieces vPieces;
	unsigned int piecesAvailable = getAllLiveEnemyPieces(vPieces, board);

	for (auto p : vPieces)
	{
		auto moves = Gameplay::getValidMoves(status, board, p.piece, p.row, p.col);
		for (auto m : moves)
		{
			Board* newBoard = board->hardCopy();
			GameStatus* newStatus = new GameStatus(*status);
			Gameplay::move(newStatus, newBoard, m);
			std::shared_ptr<Move> nextMove;
			int value = maximise(newBoard, newStatus, &nextMove, depthLimit - 1, alpha, beta);

			if (value < min)
			{
				min = value;
				*move = m;
			}

			delete newStatus;
			delete newBoard;

			if (min < beta)
				beta = min;

			if (min <= alpha)
				return min;
		}
	}

	return min;
}

int ChessPlayer::CalculateValue(Board* board, GameStatus* status)
{
	int score = 0;
	vecPieces myPieces;
	unsigned int pieceCount = getAllLivePieces(myPieces, board);

	for (auto piece : myPieces)
	{
		score += (int)piece.piece->getType();
		if (2 == piece.row || piece.row == 5)
			score += 2;
		if (2 == piece.col || piece.col == 5)
			score += 2;
		if (3 == piece.row || piece.row == 4)
			score += 5;
		if (3 == piece.col || piece.col == 4)
			score += 5;
		auto moves = Gameplay::getValidMoves(status, board, piece.piece, piece.row, piece.col);
		for (auto move : moves)
		{
			score += 0.5f;
			switch (move->getType())
			{
			case MoveType::CAPTURE:
			case MoveType::EN_PASSANT:
				score += 5;
				break;
			case MoveType::CASTLING:
				score += 2;
				break;
			default:
				break;
			}
		}
		//score += Gameplay::getValidMoves(status, board, piece.piece, piece.row, piece.col).size();
	}

	vecPieces enemyPieces;
	unsigned int enemyPieceCount = getAllLiveEnemyPieces(enemyPieces, board);
	for (auto piece : enemyPieces)
	{
		score -= (int)piece.piece->getType();
		//score -= Gameplay::getValidMoves(status, board, piece.piece, piece.row, piece.col).size();
	}

	return score;
}
