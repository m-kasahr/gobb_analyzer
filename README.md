# gobb_analyzer

`gobb_analyzer` is a tool for analyzing Gobblet Gobblers, which is a board game with perfect
information played between two players.  For all positions, the tool analyzes which player
wins (or draw), assuming that both players play perfectly.  It is written in C++17 and
runs on POSIX based systems and Windows.

`gobb_analyzer` provides two programs with command line interface, `gobb_analyze` and
`gobb_inspect`.  `gobb_analyze` analyzes all positions of Gobblet Gobblers and writes
the result to a file.  `gobb_inspect` reads the file and shows the analysis data as
text messages.

`gobb_analyzer` is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation, either
version 3 of the License, or (at your option) any later version.

## Requirements

The following software packages are required for building:

* C++17 compiler
* Python (version 3.6 or later)
* CMake (version 3.12 or later)
* Make (for POSIX based systems) or MSBuild (for Windows with VC++)

At run time, `gobb_analyze` and `gobb_inspect` each consume about 3GB of memory.

## Build gobb_analyzer

Create a working directory and change the current directory to there.

    mkdir build
    cd build

Build the programs.  It is highly recommended to choose `Release` as the build type,
because they run much faster.

    cmake -DCMAKE_BUILD_TYPE=Release ..

If you would also like to build a test program, add `-DENABLE_TESTING=ON` to the arguments.

    cmake -DCMAKE_BUILD_TYPE=Release -DENABLE_TESTING=ON ..

Run `make` (on POSIX based systems)

    make

or `msbuild` (on Windows x64 with VC++)

    msbuild gobb_analyzer.sln /p:Configuration=Release /p:Platform=x64

## Generate analysis data with gobb_analyze

Run `gobb_analyze` to create an analysis data file.  On POSIX based systems, execute

    ./gobb_analyze

On Windows, execute

    .\Release\gobb_analyze.exe

It may take several dozens of minutes.  It took 30 minutes on PC with Intel Core i5-4590
and 16GB memory, running Linux on WSL (Windows subsystem for Linux) of Windows 10.

When the analysis is completed, `gobb_analyze` stores the analysis data to a file named
`gobb_analyzer_16.dat` (16 is a generation number of the analysis data) at
the current directory.  Its size is 3GB.

For more details about `gobb_analyze`, refer to the document `gobb_analyze.1.md`.

## Inspect the analysis data with gobb_inspect

Run `gobb_inspect` to inspect the data file `gobb_analyzer_16.dat` at the current
directory.  On POSIX based systems, execute

    ./gobb_inspect

On Windows, execute

    .\Release\gobb_inspect.exe

`gobb_inspect` loads the entire file with 3GB, so that it may take for a while.
When loading the file is completed, the following text will be displayed.  it is
recommended to use a terminal with a minimum width of 100, and a height of 48.

    position = 0, remainingTurns = 13, Won
    +-------+-------+-------+
    |       |       |       |
    |       |       |       |
    |       |       |       |
    +-------+-------+-------+
    |       |       |       |
    |       |       |       |
    |       |       |       |
    +-------+-------+-------+
    |       |       |       |
    |       |       |       |
    |       |       |       |
    +-------+-------+-------+
    (the player having the turn: "==")

    possible moves:
       0| Small , Out    -> NW    , position = 2881474013, remainingTurns = 14, Won
       1| Small , Out    -> N     , position = 2881474050, remainingTurns = 14, Won
       2| Small , Out    -> NE    , position = 2881474087, remainingTurns = 14, Won
       3| Small , Out    -> W     , position = 2881474124, remainingTurns = 14, Won
       4| Small , Out    -> Center, position = 2881474161, remainingTurns = 14, Won
       5| Small , Out    -> E     , position = 2881474198, remainingTurns = 14, Won
       6| Small , Out    -> SW    , position = 2881474235, remainingTurns = 14, Won
       7| Small , Out    -> S     , position = 2881474272, remainingTurns = 14, Won
       8| Small , Out    -> SE    , position = 2881474309, remainingTurns = 14, Won
       9| Medium, Out    -> NW    , position = 2881539425, remainingTurns = 13, Lost
      10| Medium, Out    -> N     , position = 2881592076, remainingTurns = 11, Lost
      11| Medium, Out    -> NE    , position = 2881644727, remainingTurns = 13, Lost
      12| Medium, Out    -> W     , position = 2881697378, remainingTurns = 11, Lost
      13| Medium, Out    -> Center, position = 2881750029, remainingTurns = 11, Lost
      14| Medium, Out    -> E     , position = 2881802680, remainingTurns = 11, Lost
      15| Medium, Out    -> SW    , position = 2881855331, remainingTurns = 13, Lost
      16| Medium, Out    -> S     , position = 2881907982, remainingTurns = 11, Lost
      17| Medium, Out    -> SE    , position = 2881960633, remainingTurns = 13, Lost
      18| Large , Out    -> NW    , position = 2974620701, remainingTurns = 12, Won [best]
      19| Large , Out    -> N     , position = 3049543074, remainingTurns = 14, Won
      20| Large , Out    -> NE    , position = 3124465447, remainingTurns = 12, Won [best]
      21| Large , Out    -> W     , position = 3199387820, remainingTurns = 14, Won
      22| Large , Out    -> Center, position = 3274310193, remainingTurns = 12, Won [best]
      23| Large , Out    -> E     , position = 3349232566, remainingTurns = 14, Won
      24| Large , Out    -> SW    , position = 3424154939, remainingTurns = 12, Won [best]
      25| Large , Out    -> S     , position = 3499077312, remainingTurns = 14, Won
      26| Large , Out    -> SE    , position = 3573999685, remainingTurns = 12, Won [best]
    gobb_inspect>

The text messages show the initial position of the game, the possible moves and
a prompt `gobb_inspect>`.

The program interactively accepts input from standard in and prints the result messages
to standard out.  Type `? Enter` to see help messages.  For details about `gobb_inspect`,
refer to the document `gobb_inspect.1.md`.

`gobb_inspect` displays colored pieces using escape sequences.  On Windows, Command Prompt
and PowerShell support escape sequences, but they are disabled by default.  If you would
like to make Command Prompt and PowerShell recognize escape sequences, set the registory

    HKEY_CURRENT_USER\Console\VirtualTerminalLevel

to 1 (REG_DWORD) and launch a terminal program.
