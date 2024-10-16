#ifndef CONFIG_H
#define CONFIG_H

// String used to delimit block outputs in the status.
#define DELIMITER " "

// Maximum number of Unicode characters that a block can output.
#define MAX_BLOCK_OUTPUT_LENGTH 50

// Control whether blocks are clickable.
#define CLICKABLE_BLOCKS 1

// Control whether a leading delimiter should be prepended to the status.
#define LEADING_DELIMITER 0

// Control whether a trailing delimiter should be appended to the status.
#define TRAILING_DELIMITER 0

// Define blocks for the status feed as X(icon, cmd, interval, signal).
#define BLOCKS(X)               \
    X("", "~/.local/bin/statusbar/pkg_modul", 3600, 2)     \
    X("", "~/.local/bin/statusbar/cpu_modul", 20, 1)       \
    X("", "~/.local/bin/statusbar/mem_modul", 20, 3)     \
    X("", "~/.local/bin/statusbar/clock_modul", 60, 5)      \
    X("", "~/.local/bin/statusbar/volume_modul", 41, 7)         

#endif  // CONFIG_H
