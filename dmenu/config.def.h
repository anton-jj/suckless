/* See LICENSE file for copyright and license details. */
/* Default settings; can be overriden by command line. */

static int topbar = 1;      /* -b  option; if 0, dmenu appears at bottom     */
static int centered = 1;    /* -c option; centers dmenu on screen */
static int min_width = 1000; /* minimum width when centered */
/* -fn option overrides fonts[0]; default X11 font or font set */
static char *fonts[] = {"HackNerdFont-Regular:size=20"};
static const char *prompt = NULL; /* -p  option; prompt to the left of */

static const char col_fg[]       = "#edeff0";  // Light foreground text
static const char col_bg[]       = "#0c0e0f";  // Dark background
static const char col_border[]   = "#6791c9";  // Soft blue border
static const char col_highlight[] = "#df5b61"; // Muted red highlight
static const char col_sel_bg[]   = "#e8646a";  // Red for selected background
static const unsigned int alpha = 200;         // Amount of opacity (0xff is opaque)

static const char *colors[SchemeLast][2] = {
    /*     fg         bg       */
    [SchemeNorm]          = { col_fg, col_bg },              
    [SchemeSel]           = { col_fg, col_bg },        
    [SchemeSelHighlight]  = { col_fg, col_highlight },            
    [SchemeNormHighlight] = { col_highlight, col_bg },     
    [SchemeOut]           = { col_fg, col_bg },            
    [SchemeOutHighlight]  = { col_highlight, col_bg },     
};

static const unsigned int alphas[SchemeLast][2] = {
	[SchemeNorm] = { OPAQUE, alpha },
	[SchemeSel] = { OPAQUE, alpha },
	[SchemeOut] = { OPAQUE, alpha },
};

/* -l and -g options; controls number of lines and columns in grid if > 0 */
static unsigned int lines = 3;
static unsigned int columns = 1;

/*
 * Characters not considered part of a word while deleting words
 * for example: " /?\"&[]"
 */
static const char worddelimiters[] = " | ";

/* Size of the window border */
static unsigned int border_width = 3;
