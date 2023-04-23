#pragma once
#ifndef PLAYBOOK_H
#define PLAYBOOK_H
#include "Chess/PieceType.h"
#include <vector>
#include <string>

struct PlaybookMove
{
	PieceType pieceType;
	int column;
	int row;

	bool operator== (const PlaybookMove& rhs)
	{
		return pieceType == rhs.pieceType &&
			column == rhs.column &&
			row == rhs.row;
	}

	bool operator!= (const PlaybookMove& rhs)
	{
		return !(*this == rhs);
	}
};

struct Playbook
{
	std::string name;
	std::vector<PlaybookMove> moves;
};
#endif