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

	/*
	Playbook templatePB;
	templatePB.name = "";
	templatePB.moves.push_back(PlaybookMove{ PieceType::PAWN, 0, 0 });
	m_playbooks.push_back(templatePB);
	*/

	// Pawn to E4

	Playbook spanishOpening;
	spanishOpening.name = "Ruy Lopez Opening";
	spanishOpening.moves.push_back(PlaybookMove{ PieceType::PAWN, 4, 3 });
	spanishOpening.moves.push_back(PlaybookMove{ PieceType::PAWN, 4, 4 });
	spanishOpening.moves.push_back(PlaybookMove{ PieceType::KNIGHT, 5, 2 });
	spanishOpening.moves.push_back(PlaybookMove{ PieceType::KNIGHT, 2, 5 });
	spanishOpening.moves.push_back(PlaybookMove{ PieceType::BISHOP, 1, 4 });
	m_playbooks.push_back(spanishOpening);

	Playbook sicilianDefense;
	sicilianDefense.name = "Sicilian Defense";
	sicilianDefense.moves.push_back(PlaybookMove{ PieceType::PAWN, 4, 3 });
	sicilianDefense.moves.push_back(PlaybookMove{ PieceType::PAWN, 2, 4 });
	m_playbooks.push_back(sicilianDefense);

	Playbook frenchDefense;
	frenchDefense.name = "French Defense";
	frenchDefense.moves.push_back(PlaybookMove{ PieceType::PAWN, 4, 3 });
	frenchDefense.moves.push_back(PlaybookMove{ PieceType::PAWN, 4, 5 });
	m_playbooks.push_back(frenchDefense);

	Playbook caroKannDefense;
	caroKannDefense.name = "Caro-Kann Defense";
	caroKannDefense.moves.push_back(PlaybookMove{ PieceType::PAWN, 4, 3 });
	caroKannDefense.moves.push_back(PlaybookMove{ PieceType::PAWN, 3, 5 });
	m_playbooks.push_back(caroKannDefense);

	Playbook italianGame;
	italianGame.name = "Italian Game";
	italianGame.moves.push_back(PlaybookMove{ PieceType::PAWN, 4, 3 });
	italianGame.moves.push_back(PlaybookMove{ PieceType::KNIGHT, 5, 2 });
	italianGame.moves.push_back(PlaybookMove{ PieceType::KNIGHT, 2, 5 });
	italianGame.moves.push_back(PlaybookMove{ PieceType::BISHOP, 2, 3 });
	m_playbooks.push_back(italianGame);

	Playbook scandinavianDefense;
	scandinavianDefense.name = "ScandinavianDefense";
	scandinavianDefense.moves.push_back(PlaybookMove{ PieceType::PAWN, 4, 3 });
	scandinavianDefense.moves.push_back(PlaybookMove{ PieceType::PAWN, 3, 4 });
	m_playbooks.push_back(scandinavianDefense);

	Playbook pircDefense;
	pircDefense.name = "Pirc Defense";
	pircDefense.moves.push_back(PlaybookMove{ PieceType::PAWN, 4, 3 });
	pircDefense.moves.push_back(PlaybookMove{ PieceType::PAWN, 3, 5 });
	pircDefense.moves.push_back(PlaybookMove{ PieceType::PAWN, 3, 3 });
	pircDefense.moves.push_back(PlaybookMove{ PieceType::KNIGHT, 5, 5 });
	m_playbooks.push_back(pircDefense);

	Playbook AlekhinesDefense;
	AlekhinesDefense.name = "Alekhine's Defense";
	AlekhinesDefense.moves.push_back(PlaybookMove{ PieceType::PAWN, 4, 3 });
	AlekhinesDefense.moves.push_back(PlaybookMove{ PieceType::KNIGHT, 5, 5 });
	m_playbooks.push_back(AlekhinesDefense);

	Playbook kingsGambit;
	kingsGambit.name = "King's Gambit";
	kingsGambit.moves.push_back(PlaybookMove{ PieceType::PAWN, 4, 3 });
	kingsGambit.moves.push_back(PlaybookMove{ PieceType::PAWN, 4, 4 });
	kingsGambit.moves.push_back(PlaybookMove{ PieceType::PAWN, 5, 3 });
	m_playbooks.push_back(kingsGambit);

	Playbook scotchGame;
	scotchGame.name = "Scotch Game";
	scotchGame.moves.push_back(PlaybookMove{ PieceType::PAWN, 4, 3 });
	scotchGame.moves.push_back(PlaybookMove{ PieceType::PAWN, 4, 4 });
	scotchGame.moves.push_back(PlaybookMove{ PieceType::KNIGHT, 5, 2 });
	scotchGame.moves.push_back(PlaybookMove{ PieceType::KNIGHT, 2, 5 });
	scotchGame.moves.push_back(PlaybookMove{ PieceType::PAWN, 3, 3 });
	m_playbooks.push_back(scotchGame);

	Playbook viennaGame;
	viennaGame.name = "Vienna Game";
	viennaGame.moves.push_back(PlaybookMove{ PieceType::PAWN, 4, 3 });
	viennaGame.moves.push_back(PlaybookMove{ PieceType::PAWN, 4, 4 });
	viennaGame.moves.push_back(PlaybookMove{ PieceType::KNIGHT, 2, 2 });
	m_playbooks.push_back(viennaGame);

	// Pawn to D4

	Playbook queensGambit;
	queensGambit.name = "Queen's Gambit";
	queensGambit.moves.push_back(PlaybookMove{ PieceType::PAWN, 3, 3 });
	queensGambit.moves.push_back(PlaybookMove{ PieceType::PAWN, 3, 4 });
	queensGambit.moves.push_back(PlaybookMove{ PieceType::PAWN, 2, 3 });
	m_playbooks.push_back(queensGambit);

	Playbook slavDefense;
	slavDefense.name = "Slav Defense";
	slavDefense.moves.push_back(PlaybookMove{ PieceType::PAWN, 3, 3 });
	slavDefense.moves.push_back(PlaybookMove{ PieceType::PAWN, 3, 4 });
	slavDefense.moves.push_back(PlaybookMove{ PieceType::PAWN, 2, 3 });
	slavDefense.moves.push_back(PlaybookMove{ PieceType::PAWN, 2, 5 });
	m_playbooks.push_back(slavDefense);

	Playbook kingsIndianDefense;
	kingsIndianDefense.name = "King's Indian Defense";
	kingsIndianDefense.moves.push_back(PlaybookMove{ PieceType::PAWN, 3, 3 });
	kingsIndianDefense.moves.push_back(PlaybookMove{ PieceType::KNIGHT, 5, 5 });
	kingsIndianDefense.moves.push_back(PlaybookMove{ PieceType::PAWN, 2, 3 });
	kingsIndianDefense.moves.push_back(PlaybookMove{ PieceType::PAWN, 6, 5 });
	m_playbooks.push_back(kingsIndianDefense);

	Playbook nimzoIndianDefense;
	nimzoIndianDefense.name = "Nimzo-Indian Defense";
	nimzoIndianDefense.moves.push_back(PlaybookMove{ PieceType::PAWN, 3, 3 });
	nimzoIndianDefense.moves.push_back(PlaybookMove{ PieceType::KNIGHT, 5, 5 });
	nimzoIndianDefense.moves.push_back(PlaybookMove{ PieceType::PAWN, 2, 3 });
	nimzoIndianDefense.moves.push_back(PlaybookMove{ PieceType::PAWN, 4, 5 });
	nimzoIndianDefense.moves.push_back(PlaybookMove{ PieceType::KNIGHT, 2, 2 });
	nimzoIndianDefense.moves.push_back(PlaybookMove{ PieceType::BISHOP, 1, 3 });
	m_playbooks.push_back(nimzoIndianDefense);

	Playbook queensIndianDefense;
	queensIndianDefense.name = "Queen's Indian Defense";
	queensIndianDefense.moves.push_back(PlaybookMove{ PieceType::PAWN, 3, 3 });
	queensIndianDefense.moves.push_back(PlaybookMove{ PieceType::KNIGHT, 5, 5 });
	queensIndianDefense.moves.push_back(PlaybookMove{ PieceType::PAWN, 2, 3 });
	queensIndianDefense.moves.push_back(PlaybookMove{ PieceType::PAWN, 4, 5 });
	queensIndianDefense.moves.push_back(PlaybookMove{ PieceType::KNIGHT, 5, 2 });
	queensIndianDefense.moves.push_back(PlaybookMove{ PieceType::PAWN, 1, 5 });
	m_playbooks.push_back(queensIndianDefense);

	Playbook catalanOpening;
	catalanOpening.name = "Catalan Opening";
	catalanOpening.moves.push_back(PlaybookMove{ PieceType::PAWN, 3, 3 });
	catalanOpening.moves.push_back(PlaybookMove{ PieceType::KNIGHT, 5, 5 });
	catalanOpening.moves.push_back(PlaybookMove{ PieceType::PAWN, 2, 3 });
	catalanOpening.moves.push_back(PlaybookMove{ PieceType::PAWN, 4, 5 });
	catalanOpening.moves.push_back(PlaybookMove{ PieceType::PAWN, 6, 2 });
	m_playbooks.push_back(catalanOpening);

	Playbook bogoIndianDefense;
	bogoIndianDefense.name = "Bogo-Indian Defense";
	bogoIndianDefense.moves.push_back(PlaybookMove{ PieceType::PAWN, 3, 3 });
	bogoIndianDefense.moves.push_back(PlaybookMove{ PieceType::KNIGHT, 5, 5 });
	bogoIndianDefense.moves.push_back(PlaybookMove{ PieceType::PAWN, 2, 3 });
	bogoIndianDefense.moves.push_back(PlaybookMove{ PieceType::PAWN, 4, 5 });
	bogoIndianDefense.moves.push_back(PlaybookMove{ PieceType::KNIGHT, 5, 2 });
	bogoIndianDefense.moves.push_back(PlaybookMove{ PieceType::BISHOP, 1, 3 });
	m_playbooks.push_back(bogoIndianDefense);

	Playbook grunfeldDefense;
	grunfeldDefense.name = "Grunfeld Defense";
	grunfeldDefense.moves.push_back(PlaybookMove{ PieceType::PAWN, 3, 3 });
	grunfeldDefense.moves.push_back(PlaybookMove{ PieceType::KNIGHT, 5, 5 });
	grunfeldDefense.moves.push_back(PlaybookMove{ PieceType::PAWN, 2, 3 });
	grunfeldDefense.moves.push_back(PlaybookMove{ PieceType::PAWN, 6, 5 });
	grunfeldDefense.moves.push_back(PlaybookMove{ PieceType::KNIGHT, 2, 2 });
	grunfeldDefense.moves.push_back(PlaybookMove{ PieceType::PAWN, 3, 4 });
	m_playbooks.push_back(grunfeldDefense);

	Playbook dutchDefense;
	dutchDefense.name = "Dutch Defense";
	dutchDefense.moves.push_back(PlaybookMove{ PieceType::PAWN, 3, 3 });
	dutchDefense.moves.push_back(PlaybookMove{ PieceType::PAWN, 5, 4 });
	m_playbooks.push_back(dutchDefense);

	Playbook trompowskyAttack;
	trompowskyAttack.name = "Trompowsky Attack";
	trompowskyAttack.moves.push_back(PlaybookMove{ PieceType::PAWN, 3, 3 });
	trompowskyAttack.moves.push_back(PlaybookMove{ PieceType::KNIGHT, 5, 5 });
	trompowskyAttack.moves.push_back(PlaybookMove{ PieceType::BISHOP, 6, 4 });
	m_playbooks.push_back(trompowskyAttack);

	Playbook benkoGambit;
	benkoGambit.name = "Benko Gambit";
	benkoGambit.moves.push_back(PlaybookMove{ PieceType::PAWN, 3, 3 });
	benkoGambit.moves.push_back(PlaybookMove{ PieceType::KNIGHT, 5, 5 });
	benkoGambit.moves.push_back(PlaybookMove{ PieceType::PAWN, 2, 3 });
	benkoGambit.moves.push_back(PlaybookMove{ PieceType::PAWN, 2, 4 });
	benkoGambit.moves.push_back(PlaybookMove{ PieceType::PAWN, 3, 4 });
	benkoGambit.moves.push_back(PlaybookMove{ PieceType::PAWN, 1, 4 });
	m_playbooks.push_back(benkoGambit);

	m_maxPlaybookMoveCount = 0;
	for (Playbook playbook : m_playbooks)
	{
		if (playbook.moves.size() > m_maxPlaybookMoveCount)
			m_maxPlaybookMoveCount = playbook.moves.size();
	}
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
	int alpha = -100000;
	int beta = 100000;

	if (m_gameMoves.size() < m_maxPlaybookMoveCount)
	{
		vector<Playbook> books;

		for (Playbook playbook : m_playbooks)
		{
			if (playbook.moves.size() <= m_gameMoves.size())
				continue;

			bool matches = true;
			for (int i = 0; i < m_gameMoves.size(); i++)
				if (m_gameMoves[i] != playbook.moves[i])
				{
					matches = false;
					break;
				}

			if (matches)
			{
				books.push_back(playbook);
			}
		}

		if (books.size() > 0)
		{
			// TODO
			cout << "Playbooks Available: " << books.size() << endl;
			vecPieces vPieces;
			unsigned int piecesAvailable = getAllLivePieces(vPieces, m_pBoard);

			Playbook chosen = books[rand() % books.size()];
			PlaybookMove nextMove = chosen.moves[m_gameMoves.size()];
			cout << "Playing " << chosen.name << endl;
			for (PieceInPostion piece : vPieces)
			{
				if (piece.piece->getType() != nextMove.pieceType)
					continue;
				vector<shared_ptr<Move>> moves = Gameplay::getValidMoves(m_pGameStatus, m_pBoard, piece.piece, piece.row, piece.col);
				for (shared_ptr<Move> move : moves)
				{
					int moveToRow = move->getDestinationPosition().first;
					int moveToCol = move->getDestinationPosition().second;

					if (moveToRow != nextMove.row || moveToCol != nextMove.column)
						continue;

					*moveToMake = move;
					return true;
				}
			}
			cout << "Failed??" << endl;
		}
	}

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	int value = maximise(m_pBoard, m_pGameStatus, moveToMake, 3, alpha, beta);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time taken = " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]" << std::endl;

	if (value > -99999)
		return true;

	// Opponent can checkmate next turn, so all moves were regarded as awful, therefore pick a random move.
	bool canMove = false;

	vecPieces vPieces;
	unsigned int piecesAvailable = getAllLivePieces(vPieces, m_pBoard);
	for (PieceInPostion piece : vPieces)
	{
		std::vector<std::shared_ptr<Move>> moves = Gameplay::getValidMoves(m_pGameStatus, m_pBoard, piece.piece, piece.row, piece.col);
		if (moves.size() <= 0)
			continue;

		*moveToMake = moves[0];
		canMove = true;
	}

	return canMove;
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
	return ShannonEvaluation(board, status, m_colour) - ShannonEvaluation(board, status, m_colour == PieceColor::WHITE ? PieceColor::BLACK : PieceColor::WHITE);
}

int ChessPlayer::CustomEvaluation(Board* board, GameStatus* status)
{
	int score = 0;
	vecPieces myPieces;
	// pieceCount is unused, but needed to get all the live pieces
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

int ChessPlayer::ShannonEvaluation(Board* board, GameStatus* status, PieceColor colour)
{
	vecPieces myPieces;
	// pieceCount is unused, but needed to get all the live pieces
	if (colour == m_colour)
		unsigned int pieceCount = getAllLivePieces(myPieces, board);
	else
		unsigned int pieceCount = getAllLiveEnemyPieces(myPieces, board);

	int kings = 0;
	int queens = 0;
	int rooks = 0;
	int bishops = 0;
	int knights = 0;
	int pawns = 0;

	int doubledPawns = 0;
	int backwardPawns = 0;
	int isolatedPawns = 0;

	int mobility = 0;

	for (auto piece : myPieces)
	{
		switch (piece.piece->getType())
		{
		case PieceType::KING:
			kings++;
			break;
		case PieceType::QUEEN:
			queens++;
			break;
		case PieceType::ROOK:
			rooks++;
			break;
		case PieceType::BISHOP:
			bishops++;
			break;
		case PieceType::KNIGHT:
			knights++;
			break;
		case PieceType::PAWN:
			pawns++;
			// Check if this pawn is doubled (has other same-coloured pawns in its column/file)
			for (int i = 0; i < 8; i++)
			{
				if (i == piece.row)
					continue;

				Square* s = board->getSquare(i, piece.col);
				if (!s->hasOccupyingPiece())
					continue;

				shared_ptr<Piece> p = s->getOccupyingPiece();
				if (p->getType() == PieceType::PAWN && p->getColor() == piece.piece->getColor())
				{
					doubledPawns++;
					break;
				}
			}

			// Check if this pawn is backward (is not being defended by other pawns)
			bool backward = true;
			if (colour == PieceColor::WHITE)
			{
				// Check down left
				if (piece.col > 0)
				{
					Square* s = board->getSquare(piece.row - 1, piece.col - 1);
					if (s->hasOccupyingPiece())
					{
						shared_ptr<Piece> p = s->getOccupyingPiece();
						if (p->getType() == PieceType::PAWN && p->getColor() == piece.piece->getColor())
							backward = false;
					}
				}
				// Check down right
				if (piece.col < 7)
				{
					Square* s = board->getSquare(piece.row - 1, piece.col + 1);
					if (s->hasOccupyingPiece())
					{
						shared_ptr<Piece> p = s->getOccupyingPiece();
						if (p->getType() == PieceType::PAWN && p->getColor() == piece.piece->getColor())
							backward = false;
					}
				}
			}
			else
			{
				// Check up left
				if (piece.col > 0)
				{
					Square* s = board->getSquare(piece.row + 1, piece.col - 1);
					if (s->hasOccupyingPiece())
					{
						shared_ptr<Piece> p = s->getOccupyingPiece();
						if (p->getType() == PieceType::PAWN && p->getColor() == piece.piece->getColor())
							backward = false;
					}
				}
				// Check up right
				if (piece.col < 7)
				{
					Square* s = board->getSquare(piece.row + 1, piece.col + 1);
					if (s->hasOccupyingPiece())
					{
						shared_ptr<Piece> p = s->getOccupyingPiece();
						if (p->getType() == PieceType::PAWN && p->getColor() == piece.piece->getColor())
							backward = false;
					}
				}
			}
			if (backward)
				backwardPawns++;

			// Check if this pawn is isolated (has no pawns in adjacent columns/files)
			bool isolated = true;
			for (int colOffset = -1; colOffset < 2; colOffset++)
			{
				//if (!isolated)
					//break;

				if (colOffset == 0 || piece.col + colOffset < 0 || piece.col + colOffset > 8)
					continue;

				for (int row = 0; row < 8; row++)
				{
					if (piece.col + colOffset > 7 || piece.col + colOffset < 0)
						continue;

					Square* s = board->getSquare(row, piece.col + colOffset);
					if (!s->hasOccupyingPiece())
						continue;

					shared_ptr<Piece> p = s->getOccupyingPiece();
					if (p->getType() == PieceType::PAWN && p->getColor() == piece.piece->getColor())
					{
						isolated = false;
						break;
					}
				}
			}
			if (isolated)
				isolatedPawns++;
			break;
		}
		/*if (2 == piece.row || piece.row == 5)
			score += 2;
		if (2 == piece.col || piece.col == 5)
			score += 2;
		if (3 == piece.row || piece.row == 4)
			score += 5;
		if (3 == piece.col || piece.col == 4)
			score += 5;*/
		auto moves = Gameplay::getValidMoves(status, board, piece.piece, piece.row, piece.col);
		mobility += moves.size();
		/*for (auto move : moves)
		{
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
		}*/
		//score += Gameplay::getValidMoves(status, board, piece.piece, piece.row, piece.col).size();
	}

	return 200 * kings + 9 * queens + 5 * rooks + 3 * (bishops + knights) + pawns - 0.5f * (doubledPawns + backwardPawns + isolatedPawns) + 0.1f * mobility;
	//return ((int) PieceType::KING) * kings + 9 * queens + 5 * rooks + 3 * (bishops + knights) + pawns - 0.5f * (doubledPawns + backwardPawns + isolatedPawns) + 0.1f * mobility;
}
