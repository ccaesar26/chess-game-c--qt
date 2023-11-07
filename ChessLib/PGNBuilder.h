#pragma once

#include <string>

class PGNBuilder
{

public:

	PGNBuilder();

	std::string GetPGNFormat() const;

	void AddMove(const std::string& move);

	bool SaveFormat(const std::string& fileName) const;

private:

	std::string m_PGNString;
};

