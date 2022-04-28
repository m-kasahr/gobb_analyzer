//
// Copyright (C) 2022 Motoyuki Kasahara.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>
//

#ifndef GOBB_ANALYZER_ANALYSIS_DATA_FILE_HANDLER_HPP
#define GOBB_ANALYZER_ANALYSIS_DATA_FILE_HANDLER_H

#include <cstddef>
#include <filesystem>
#include <string>
#include "analyzer.hpp"

///
/// @file   analysis_data_file_handler.hpp
/// @brief  Define the class `AnalysisDataFileHandler`, implementation of `AnalysisDataIOHandler`.
///
namespace gobb_analyzer {

///
/// File handler for reading and writing analysis data from/to files.
///
/// The handler reads and writes files `gobb_analyzer_<generation>.dat` at the specified directory,
/// where `<generation>` is a generation number of the analysis data.
///
class AnalysisDataFileHandler: public AnalysisDataIOHandler {
public:
    ///
    /// Default constructor.
    ///
    /// It reads and writes analysis data from/to files at the current directory.
    ///
    AnalysisDataFileHandler();

    ///
    /// Constructor.
    ///
    /// @param   dir  a path to the directory.
    ///
    /// It reads and writes analysis data from/to a file at the given directory.
    /// The directory `dir` is created automatically if missing.
    ///
    AnalysisDataFileHandler(const std::string& dir);

    ///
    /// Copy constructor.
    ///
    /// @param   other  an instance to copy from.
    ///
    /// It constructs an instance with a copy of the contents of `other`.
    ///
    AnalysisDataFileHandler(const AnalysisDataFileHandler& other) = default;

    ///
    /// Move constructor.
    ///
    /// @param   other  an instance to move from.
    ///
    /// It constructs an instance with the contents of `other` using move semantics.
    ///
    AnalysisDataFileHandler(AnalysisDataFileHandler&& other) = default;

    ///
    /// Copy assignment operator.
    ///
    /// @param   other  an instance to copy from.
    /// @return  a reference of `*this`.
    ///
    /// It replaces the contents with a copy of `other`.
    ///
    AnalysisDataFileHandler& operator=(const AnalysisDataFileHandler& other) = default;

    ///
    /// Move assignment operator.
    ///
    /// @param   other  an instance to move from.
    /// @return  a reference of `*this`.
    ///
    /// It replaces the contents with those of `other` using move semantics.
    ///
    AnalysisDataFileHandler& operator=(AnalysisDataFileHandler&& other) = default;

    ///
    /// Destructor.
    ///
    virtual ~AnalysisDataFileHandler() = default;

    ///
    /// Store analysis data and its statistics to a file.
    ///
    /// @param   generation  a generation number.
    /// @param   stats       statistics data.
    /// @param   table       a table of analysis data.
    /// @param   tableSize   the number of elements in `table`.
    /// @return  true upon success.
    ///
    virtual bool store(Generation generation, const AnalysisStatistics& stats,
        const AnalysisData* table, std::size_t tableSize);

    ///
    /// Load data from a file.
    ///
    /// @param   generation  a generation number.
    /// @param   stats       statistics data.
    /// @param   table       a table of analysis data.
    /// @param   tableSize   the number of elements in `table`.
    /// @return  true upon success.
    ///
    /// Upon success, the loaded analysis data and its statistics are written to `table` and `stats`.
    ///
    virtual bool load(Generation generation, AnalysisStatistics& stats,
        AnalysisData* table, std::size_t tableSize) const;

    ///
    /// Finds the latest generation of stored data files.
    ///
    /// @return  the largest generation number.
    ///
    /// It returns -1 if no data file is found.
    ///
    virtual Generation find_latest() const;

    ///
    /// Load a data file of the latest generation.
    ///
    /// @param   stats       statistics data.
    /// @param   table       a table of analysis data.
    /// @param   tableSize   the number of elements in `table`.
    /// @return  the generation number of the loaded data.
    ///
    /// The function finds an analysis data file with the largest generation number and loads it.
    /// Upon success, the loaded analysis data and its statistics data are written to `table` and `stats` and
    /// it returns the generation number.
    /// Otherwise, it returns -1.
    ///
    virtual Generation load_latest(AnalysisStatistics& stats, AnalysisData* table, std::size_t tableSize) const;

    ///
    /// Remove a temporary file.
    ///
    virtual void clean();

private:
    ///
    /// Return an absolute path to the analysis data file with the specified generation number.
    ///
    /// @param   generation  a generation number.
    /// @return  an absolute path.
    ///
    std::filesystem::path file_path(Generation generation) const;

    ///
    /// Return an absolute path to the temporary file.
    ///
    /// @return  an absolute path.
    ///
    std::filesystem::path tmp_file_path() const;

    /// A path to the directory where analysis data files are stored.
    std::filesystem::path dirPath_;

    /// A path to the default directory where analysis data files are stored.
    static const std::string defaultDir_;

    /// A file prefix of analysis data files (filename only).
    static const std::string filePrefix_;

    /// A file suffix of analysis data files.
    static const std::string fileSuffix_;

    /// A name of the temporary file (filename only).
    static const std::string tmpFile_;

    /// Maximum I/O size in bytes.
    static constexpr std::size_t maxIoSize = 0x100'0000;
};

} // namespace gobb_analyzer

#endif // GOBB_ANALYZER_ANALYSIS_DATA_FILE_HANDLER_HPP
