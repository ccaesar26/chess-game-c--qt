#pragma once

#include <string>

enum class EFormat
{
	Pgn
};

/**
 * @brief Interface for loading and saving chess game data to/from files.
 *
 * This interface defines methods to load and save chess game data from/to files in various formats.
 */
class IChessGameStorage
{
public:
    /**
     * @brief Virtual destructor for the interface.
     */
    virtual ~IChessGameStorage() = default;

    /**
     * @brief Loads chess game data from a file in the specified format.
     *
     * @param format The format of the file (e.g., PGN).
     * @param fileName The name of the file to load data from.
     * @return `true` if the loading was successful, otherwise `false`.
     */
    virtual bool LoadFromFile(EFormat format, const std::string& fileName) = 0;

    /**
     * @brief Saves chess game data to a file in the specified format.
     *
     * @param format The format in which to save the data (e.g., PGN).
     * @param fileName The name of the file to save data to.
     * @return `true` if the saving was successful, otherwise `false`.
     */
    virtual bool SaveFormat(EFormat format, const std::string& fileName) const = 0;

    /**
     * @brief Retrieves the data format as a string representation.
     *
     * @param format The format for which to retrieve the string representation.
     * @return The string representation of the specified format.
     */
    virtual std::string GetFormat(EFormat format) const = 0;
};

