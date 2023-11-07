#include "PGNBuilder.h"

#include "fstream";

PGNBuilder::PGNBuilder()
	:m_PGNString("")
{}

std::string PGNBuilder::GetPGNFormat() const
{
	return m_PGNString;
}

void PGNBuilder::AddMove(const std::string& move)
{
	m_PGNString = m_PGNString + move + " ";
}

bool PGNBuilder::SaveFormat(const std::string& fileName) const
{
	std::ofstream fileStream(fileName);

	if (!fileStream.is_open())
		return false;

	fileStream << m_PGNString;
	return true;
}

