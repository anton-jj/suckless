/* See LICENSE file for copyright and license details. */
/* Default settings; can be overriden by command line. */

static int topbar = 1;      /* -b  option; if 0, dmenu appears at bottom     */
static int centered = 1;    /* -c option; centers dmenu on screen */
static int min_width = 1000; /* minimum width when centered */
/* -fn option overrides fonts[0]; default X11 font or font set */
static char *fonts[] = {"HackNerdFont-Regular:size=20"};
static const char *prompt = NULL; /* -p  option; prompt to the left of */
static const char col_fg[]       = "#000000";       // Svart text
static const char col_bg[]       = "#181f21";       // Mörk bakgrund
static const char col_border[]    = "#ffffff";      // Vit kant
static const char col_highlight[] = "#e06e6e";      // Röd markering
static const char col_sel_bg[]    = "#dadada";
static const unsigned int alpha = 200;         // Opacitet (0xff är ogenomskinlig)

static const char *colors[SchemeLast][2] = {
    /*     fg         bg       */
    [SchemeNorm]          = { col_fg, col_bg },              // Normalt schema
    [SchemeSel]           = { col_fg, col_border },         // Vald schema
    [SchemeSelHighlight]  = { col_fg, col_highlight },      // Vald markering schema
    [SchemeNormHighlight] = { col_highlight, col_bg },      // Normal markering schema
    [SchemeOut]           = { col_fg, col_bg },             // Utskrift schema
    [SchemeOutHighlight]  = { col_highlight, col_bg },      // Utskrift markering schema
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
