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

#include <fstream>
#include <string>
#include <system_error>
#include "analysis_data_file_handler.hpp"
#include "string_to_uint.hpp"

namespace gobb_analyzer {

AnalysisDataFileHandler::AnalysisDataFileHandler()
    : AnalysisDataFileHandler(defaultDir_) {
}

AnalysisDataFileHandler::AnalysisDataFileHandler(const std::string& dir)
    : dirPath_(std::filesystem::absolute(dir)) {
}

bool AnalysisDataFileHandler::store(Generation generation, const AnalysisStatistics& stats,
    const AnalysisData* table, std::size_t tableSize) {
    if (generation > MaxGeneration) {
        return false;
    }

    std::error_code errCode;
    if (!is_directory(dirPath_, errCode) &&
        !std::filesystem::create_directories(dirPath_, errCode)) {
        return false;
    }

    std::filesystem::path filePath(file_path(generation));
    std::filesystem::path tmpFilePath(tmp_file_path());

    std::ofstream ofs(tmpFilePath, std::ios::binary);
    ofs.write(reinterpret_cast<const char*>(&stats), sizeof(AnalysisStatistics));

    const char* p = reinterpret_cast<const char*>(table);
    std::size_t writtenSize = 0u;
    while (writtenSize + maxIoSize < tableSize) {
        ofs.write(p, maxIoSize);
        if (ofs.fail()) {
            ofs.close();
            clean();
            return false;
        }
        p += maxIoSize;
        writtenSize += maxIoSize;
    }
    if (writtenSize < tableSize) {
        ofs.write(p, tableSize - writtenSize);
        if (ofs.fail()) {
            ofs.close();
            clean();
            return false;
        }
    }

    ofs.close();
    if (ofs.fail()) {
        clean();
        return false;
    }

    std::filesystem::rename(tmpFilePath, filePath, errCode);
    if (static_cast<bool>(errCode)) {
        clean();
        return false;
    }

    return true;
}

bool AnalysisDataFileHandler::load(Generation generation, AnalysisStatistics& stats,
    AnalysisData* table, std::size_t tableSize) const {
    if (generation > MaxGeneration) {
        return false;
    }

    std::filesystem::path filePath(file_path(generation));
    std::ifstream ifs(filePath, std::ios::binary);
    ifs.read(reinterpret_cast<char*>(&stats), sizeof(AnalysisStatistics));

    char* p = reinterpret_cast<char*>(table);
    std::size_t readSize = 0u;
    while (readSize + maxIoSize < tableSize) {
        ifs.read(p, maxIoSize);
        if (ifs.fail()) {
            return false;
        }
        p += maxIoSize;
        readSize += maxIoSize;
    }
    if (readSize < tableSize) {
        ifs.read(p, tableSize - readSize);
        if (ifs.fail()) {
            return false;
        }
    }
    ifs.close();
    if (ifs.fail()) {
        return false;
    }

    return true;
}

Generation AnalysisDataFileHandler::find_latest() const {
    Generation latestGeneration = InvalidGeneration;

    std::error_code errCode;
    for (const std::filesystem::directory_entry& entry: std::filesystem::directory_iterator(dirPath_, errCode)) {
        if (!entry.is_regular_file(errCode)) {
            continue;
        }
        std::string filename = entry.path().filename().string();
        if (filename.size() <= filePrefix_.size() + fileSuffix_.size()) {
            continue;
        }

        // Using C++20, it could be written as:
        //   !filename.starts_with(filePrefix) || !filename.ends_with(fileSuffix)
        if (filename.substr(0, filePrefix_.size()) != filePrefix_ ||
            filename.substr(filename.size() - fileSuffix_.size(), fileSuffix_.size()) != fileSuffix_) {
            continue;
        }

        Generation generation;
        if (!string_to_uint(filename.substr(filePrefix_.size(),
                    filename.size() - filePrefix_.size() - fileSuffix_.size()), generation)) {
            continue;
        }
        if (generation > MaxGeneration) {
            continue;
        }

        if (latestGeneration == InvalidGeneration || generation > latestGeneration) {
            latestGeneration = generation;
        }
    }

    if (latestGeneration == InvalidGeneration) {
        return InvalidGeneration;
    }

    return latestGeneration;
}

Generation AnalysisDataFileHandler::load_latest(AnalysisStatistics& stats, AnalysisData* table,
    std::size_t tableSize) const {
    Generation latestGeneration = find_latest();
    if (latestGeneration == InvalidGeneration) {
        return InvalidGeneration;
    }
    if (load(latestGeneration, stats, table, tableSize)) {
        return latestGeneration;
    }
    return InvalidGeneration;
}

void AnalysisDataFileHandler::clean() {
    std::filesystem::path tmpFilePath(tmp_file_path());
    std::error_code errCode;
    std::filesystem::remove(tmpFilePath, errCode);
}

std::filesystem::path AnalysisDataFileHandler::file_path(Generation generation) const {
    return dirPath_ / (filePrefix_ + std::to_string(generation) + fileSuffix_);
}

std::filesystem::path AnalysisDataFileHandler::tmp_file_path() const {
    return dirPath_ / tmpFile_;
}

const std::string AnalysisDataFileHandler::filePrefix_("gobb_analyzer_");
const std::string AnalysisDataFileHandler::fileSuffix_(".dat");
const std::string AnalysisDataFileHandler::tmpFile_("gobb_analyer_tmp.dat");
const std::string AnalysisDataFileHandler::defaultDir_(".");
} // namespace gobb_analyzer
