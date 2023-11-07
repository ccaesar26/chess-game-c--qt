#include "PGNReader.h"

#include <sstream>
#include <fstream>
#include <regex>

PGNReader::PGNReader()
{
}

bool PGNReader::LoadFromFile(const std::string& fileName)
{
	std::ifstream fileStream(fileName);

	if (!fileStream.is_open())
		return false;

	std::stringstream buffer;
	buffer << fileStream.rdbuf();
	fileStream.close();

	return LoadFromString(buffer.str());
}

bool PGNReader::LoadFromString(const std::string& str)
{
	std::string move;

	for (char c : str)
	{
		if (c != ' ')
			move += c;
		else
		{
			move = std::regex_replace(move, std::regex("(\\b\\d+\\.\\s*)|\\s+|[+#x*]|1/2-1/2"), "");
			if (!move.empty())
				m_moves.push_back(move);
			move.clear();
		}
	}

	return true;
}

const StringMoveList& PGNReader::GetMoves() const
{
	return m_moves;
}
