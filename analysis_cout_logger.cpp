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

#include <iostream>
#include "analysis_cout_logger.hpp"

namespace gobb_analyzer {

AnalysisCoutLogger::AnalysisCoutLogger() noexcept {
}

AnalysisCoutLogger::~AnalysisCoutLogger() {
}

void AnalysisCoutLogger::info() {
    std::cout << std::endl;
}

void AnalysisCoutLogger::info(const std::string& message) {
    std::cout << message << std::endl;
}

void AnalysisCoutLogger::notice() {
    std::cerr << std::endl;
}

void AnalysisCoutLogger::notice(const std::string& message) {
    std::cerr << message << std::endl;
}

void AnalysisCoutLogger::warn() {
    std::cerr << std::endl;
}

void AnalysisCoutLogger::warn(const std::string& message) {
    std::cerr << "warning: " << message << std::endl;
}

void AnalysisCoutLogger::error() {
    std::cerr << std::endl;
}

void AnalysisCoutLogger::error(const std::string& message) {
    std::cerr << "error: " << message << std::endl;
}

} // namespace gobb_analyzer
