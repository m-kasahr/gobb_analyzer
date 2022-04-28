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

#include <cstdint>
#include <cstdlib>
#include <exception>
#include <string>
#include <iostream>
#include <cstring>
#include "analysis_cout_logger.hpp"
#include "analysis_data_file_handler.hpp"
#include "analyzer.hpp"
#include "string_to_uint.hpp"
#include "version.hpp"

using namespace gobb_analyzer;

//
// Print the help messages.
//
void print_help_message() {
    std::cout << "Usage: gobb_analyze [OPTION...]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -d DIR      store analysis data files in DIR (default: .)" << std::endl;
    std::cout << "  -g NUM      resume analysis the NUM'th generation" << std::endl;
    std::cout << "              (default: the latest generation stored)" << std::endl;
    std::cout << "  -i          start analysis initially" << std::endl;
    std::cout << "  -s          store analysis data to a file every generation" << std::endl;
    std::cout << "  --help      print this help, then exit" << std::endl;
    std::cout << "  --version   print version information, then exit" << std::endl;
}

//
// Print the version information.
//
void print_version() {
    std::cout << "Gobb Analyzer version " << GOBB_ANALYZER_VERSION << std::endl;
}

//
// Print "try 'gobb_analyze --help' ..." message.
//
void print_try_help_message(const char* argv0) {
    std::cout << "Try '" << argv0 << " --help' for more information." << std::endl;
}

//
// Main.
//
int main(int argc, char* argv[]) {
    //
    // Parses command line arguments.
    //
    std::string dataDir;
    unsigned long generation = 0u;
    bool opt_d = false;
    bool opt_g = false;
    bool opt_i = false;
    bool opt_s = false;

    int optind = 1;
    while (optind < argc) {
        if (argv[optind][0] != '-' || argv[optind][1] == '\0') {
            break;
        }

        char ch = argv[optind][1];
        if (ch == '-' && argv[optind][2] == '\0') {
            optind++;
            break;
        } else if (ch == 'd') {
            opt_d = true;
            const char* optarg;
            if (argv[optind][2] == '\0') {
                if (optind + 1 >= argc) {
                    std::cerr << argv[0] << ": missing argument to option '-d'" << std::endl;
                    print_try_help_message(argv[0]);
                    return 1;
                }
                optarg = argv[optind + 1];
                optind += 2;
            } else {
                optarg = argv[optind] + 2;
                optind++;
            }
            dataDir = std::string(optarg);
        } else if (ch == 'g') {
            const char* optarg;
            opt_g = true;
            if (argv[optind][2] == '\0') {
                if (optind + 1 >= argc) {
                    std::cerr << argv[0] << ": missing argument to option '-g'" << std::endl;
                    print_try_help_message(argv[0]);
                    return 1;
                }
                optarg = argv[optind + 1];
                optind += 2;
            } else {
                optarg = argv[optind] + 2;
                optind++;
            }
            if (!string_to_uint(optarg, generation) || generation > MaxGeneration) {
                std::cerr << argv[0] << ": invalid generation '" << optarg << "'" << std::endl;
                print_try_help_message(argv[0]);
                return 1;
            }
        } else if (ch == 'i') {
            opt_i = true;
            optind++;
        } else if (ch == 's') {
            opt_s = true;
            optind++;
        } else if (std::strcmp(argv[optind], "--help") == 0) {
            print_help_message();
            return 0;
        } else if (std::strcmp(argv[optind], "--version") == 0) {
            print_version();
            return 0;
        } else {
            std::cerr << argv[0] << ": invalid option '-" << ch << "'" << std::endl;
            print_try_help_message(argv[0]);
            return 1;
        }
    }

    if (optind < argc) {
        std::cerr << argv[0] << ": too many arguments" << std::endl;
        print_try_help_message(argv[0]);
        return 1;
    }
    if (opt_g && opt_i) {
        std::cerr << argv[0] << ": '-g' and '-i' options are conflicted" << std::endl;
        print_try_help_message(argv[0]);
    }

    //
    // Creates an Anlyzer instance and do analysis.
    //
    AnalysisDataIOMode ioMode;
    if (opt_s) {
        ioMode = AnalysisDataIOMode::StoreEveryGenerations;
    } else {
        ioMode = AnalysisDataIOMode::StoreFinalGeneration;
    }

    try {
        AnalysisCoutLogger logger;
        Analyzer analyzer(logger);
        AnalysisDataFileHandler fileHandler;
        if (opt_d) {
            fileHandler = AnalysisDataFileHandler(dataDir);
        }

        if (opt_i) {
            if (!analyzer.start(fileHandler, ioMode)) {
                return 1;
            }
        } else if (opt_g) {
            if (!analyzer.resume(fileHandler, ioMode, static_cast<Generation>(generation))) {
                return 1;
            }
        } else {
            if (!analyzer.resume(fileHandler, ioMode)) {
                return 1;
            }
        }
    } catch (std::exception& err) {
        std::cerr << "an exception raised, " << err.what() <<  std::endl;
        return 1;
    }

    return 0;
}
