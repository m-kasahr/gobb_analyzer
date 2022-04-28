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

#ifndef GOBB_ANALYZER_ANALYSIS_COUT_LOGGER_HPP
#define GOBB_ANALYZER_ANALYSIS_COUT_LOGGER_HPP

#include "analyzer.hpp"

///
/// @file   analysis_cout_logger.hpp
/// @brief  Define the class `AnalysisCoutLogger`, implementation of `AnalysisLogger`.
///
namespace gobb_analyzer {

///
/// Print messages of the analysis processing to standard out and standard error.
///
/// info() puts messages to standard out.
/// notice(), warn() and error() put messages to standard error.
///
class AnalysisCoutLogger: public AnalysisLogger {
public:
    ///
    /// Default Constructor.
    ///
    AnalysisCoutLogger() noexcept;

    AnalysisCoutLogger(const AnalysisCoutLogger& other) = delete;
    AnalysisCoutLogger(AnalysisCoutLogger&& other) = delete;
    AnalysisCoutLogger& operator=(const AnalysisCoutLogger& other) = delete;
    AnalysisCoutLogger& operator=(AnalysisCoutLogger&& other) = delete;

    ///
    /// Destructor.
    ///
    virtual ~AnalysisCoutLogger();

    ///
    /// Put an empty line to standard out.
    ///
    virtual void info();

    ///
    /// Put the informational message to standard out.
    ///
    /// @param   message  a message.
    ///
    virtual void info(const std::string& message);

    ///
    /// Put an empty line to standard error.
    ///
    virtual void notice();

    ///
    /// Put the notification message to standard error
    ///
    /// @param   message  a message.
    ///
    virtual void notice(const std::string& message);

    ///
    /// Put an empty line to standard error.
    ///
    virtual void warn();

    ///
    /// Put the warning message preceded by the `warning: ` tag to standard error,
    ///
    /// @param   message  a message.
    ///
    virtual void warn(const std::string& message);

    ///
    /// Put an empty line to standard error.
    ///
    virtual void error();

    ///
    /// Put the error message preceded by the `error: ` tag to standard error,
    ///
    /// @param   message  a message.
    ///
    virtual void error(const std::string& message);
};

} // namespace gobb_analyzer

#endif // GOBB_ANALYZER_ANALYSIS_COUT_LOGGER_HPP
