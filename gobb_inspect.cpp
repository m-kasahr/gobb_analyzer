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

#include <cstring>
#include <exception>
#include <iostream>
#include <string>
#include "analysis_data_file_handler.hpp"
#include "gobb_inspect_processor.hpp"
#include "position_text_creator.hpp"
#include "string_to_uint.hpp"
#include "inspector.hpp"
#include "version.hpp"

#if defined(_WIN32)
#include <io.h>
#elif defined(HAVE_UNISTD_H)
extern "C" {
#include <unistd.h>
}
#endif

//
// Print the help message.
//
void print_help_message() {
    std::cout << "Usage: gobb_inspect [OPTION...] [POSITION-ID]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -c          print pieces in color on the terminal" << std::endl;
    std::cout << "  -C          do not print pieces in color on the terminal" << std::endl;
    std::cout << "  -d DIR      load an analysis data file in DIR (default: .)" << std::endl;
    std::cout << "  -g NUM      load analysis data file of the NUM'th generation" << std::endl;
    std::cout << "              (default: the latest generation stored)" << std::endl;
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
void print_hint(const char* argv0) {
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
    bool opt_c = false;
    bool opt_d = false;
    bool opt_g = false;
#if defined(_WIN32)
    opt_c = _isatty(1);
#elif defined(HAVE_UNISTD_H)
    opt_c = isatty(1);
#endif

    int optind = 1;
    while (optind < argc) {
        if (argv[optind][0] != '-' || argv[optind][1] == '\0') {
            break;
        }

        char ch = argv[optind][1];
        if (ch == '-' && argv[optind][2] == '\0') {
            optind++;
            break;
        } else if (ch == 'c') {
            opt_c = true;
            optind++;
        } else if (ch == 'C') {
            opt_c = false;
            optind++;
        } else if (ch == 'd') {
            opt_d = true;
            const char* optarg;
            if (argv[optind][2] == '\0') {
                if (optind + 1 >= argc) {
                    std::cerr << argv[0] << ": missing argument to option '-d'" << std::endl;
                    print_hint(argv[0]);
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
                    print_hint(argv[0]);
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
                print_hint(argv[0]);
                return 1;
            }
        } else if (std::strcmp(argv[optind], "--help") == 0) {
            print_help_message();
            return 0;
        } else if (std::strcmp(argv[optind], "--version") == 0) {
            print_version();
            return 0;
        } else {
            std::cerr << argv[0] << ": invalid option '-" << ch << "'" << std::endl;
            print_hint(argv[0]);
            return 1;
        }
    }

    if (optind + 1 < argc) {
        std::cerr << argv[0] << ": too many arguments" << std::endl;
        print_hint(argv[0]);
        return 1;
    }

    PositionId posId;
    if (optind + 1 == argc) {
        if (!string_to_uint(argv[optind], posId)) {
            std::cerr << argv[0] << ": invalid position '"  << argv[optind] << "'" << std::endl;
            print_hint(argv[0]);
            return 1;
        }
        optind++;
    } else {
        posId = InitialPositionId;
    }

    //
    // Creates a GobbInspectProcessor instance and starts interactive processing.
    //
    PositionAsciiCreator textCreator(opt_c);

    try {
        AnalysisDataFileHandler fileHandler;
        if (opt_d) {
            fileHandler = AnalysisDataFileHandler(dataDir);
        }

        Inspector inspector;
        if (opt_g) {
            if (!inspector.load(fileHandler, static_cast<Generation>(generation))) {
                std::cerr << "failed to load the analysis data file of the specified generation" << std::endl;
                return 1;
            }
        } else {
            if (inspector.load_latest(fileHandler) == InvalidGeneration) {
                std::cerr << "failed to load an analysis data file" << std::endl;
                return 1;
            }
        }

        GobbInspectProcessor processor(inspector, textCreator, posId);
        processor.do_main_loop();
    } catch (std::exception& err) {
        std::cerr << "an exception raised, " << err.what() <<  std::endl;
        return 1;
    }

    return 0;
}
