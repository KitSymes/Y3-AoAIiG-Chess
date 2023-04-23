#pragma once
#include "Chess\PieceColor.h"
#include "Chess\PieceType.h"
#include <vector>
#include <memory>
#include "Chess/Move.h"
#include "Playbook.h"

using namespace std;

class Piece;
class Board;
class GameStatus;
class Gameplay;
class Move;

struct PieceInPostion
{
	std::shared_ptr<Piece> piece;
	int col;
	int row;
};

typedef vector<PieceInPostion> vecPieces;

class ChessPlayer
{
public:
	static void		setupPlayers(ChessPlayer** playerWhite, ChessPlayer** playerBlack, Board* pBoard, GameStatus* pGameStatus, Gameplay* pGamePlay);
	ChessPlayer(Board* pBoard, GameStatus* pGameStatus, Gameplay* pGamePlay, PieceColor colour);

	void			setAI() { m_bAI = true; }
	bool			isAI() { return m_bAI; }
	unsigned int	getAllLivePieces(vecPieces& vpieces);
	unsigned int	getAllLivePieces(vecPieces& vpieces, Board* board);
	unsigned int	getAllLiveEnemyPieces(vecPieces& vpieces, Board* board);
	vector<std::shared_ptr<Move>>	getValidMovesForPiece(PieceInPostion pip);
	bool			chooseAIMove(std::shared_ptr<Move>* moveToMake);

	vector<PlaybookMove> m_gameMoves;
protected:
	PieceColor		getColour() { return m_colour; }

private:
	PieceColor	m_colour;
	Board*		m_pBoard;
	GameStatus* m_pGameStatus;
	Gameplay*	m_pGamePlay;
	bool		m_bAI;
	vector<Playbook> m_playbooks;
	int m_maxPlaybookMoveCount;

	int maximise(Board* board, GameStatus* status, std::shared_ptr<Move>* move, int depthLimit, int alpha, int beta);
	int minimise(Board* board, GameStatus* status, std::shared_ptr<Move>* move, int depthLimit, int alpha, int beta);
	int CalculateValue(Board* board, GameStatus* status);
	int CustomEvaluation(Board* board, GameStatus* status);
	int ShannonEvaluation(Board* board, GameStatus* status, PieceColor colour);
};

