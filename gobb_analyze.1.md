# NAME

gobb_analyze - analyze positions of Gobblet Gobblers

# SYNOPSIS

gobb_analyze [OPTION]...

# DESCRIPTION

`gobb_analyze` performs retrograde analysis of Gobblet Gobblers, which is a board game with perfect information
played between two players.
For all positions, the tool analyzes which player wins (or draw), assuming that both players play perfectly.
It analyzes all positions of Gobblet Gobblers, and writes the result to a file.

`gobb_analyze` may take several dozens of minutes to perform the analysis.
When the analysis is completed, it stores the analysis data to a file named `gobb_analyzer_16.dat`
(16 is a generation number of the analysis data) at the current directory.
Its size is 3GB.
The command also consumes about 3GB of memory at run time.

When `gobb_analyze` is launched, it first searches the current directory for a data file.
If found, it loads a file with the largest generation number, and resumes the analysis.

# OPTIONS

-d DIR
: Read and write the analyis data files at DIR instead of the current directory.

-g GENERATION
: Specify a generation number of the data file.
: `gobb_analyze` loads `gobb_analyzer_<GENERATION>.dat` and resumes the analysis.
: If also `-d` option is given, `gobb_analyze` loads the file at the specified directory.
: Otherwise it loads the file at the current directory.
: The option cannot be specfied with `-i`.

-i
: Start analysis initially, even if a data file exists.
: The option cannot be specified with `-g`.

-s
: Store analysis data to a file every generation.

--help
: Show help messages, then exit.

--version
: Show the version, then exit.

# SEE ALSO

`gobb_inspect(1)`
