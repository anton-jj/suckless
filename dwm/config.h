/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
#include "movestack.c"
/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;    /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const unsigned int systrayiconsize = 16; /* systray icon size in px */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 20;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
#define ICONSIZE 16   /* icon size */
#define ICONSPACING 5 /* space between icon and title */
static const int horizpadbar        = 6;        /* horizontal padding for statusbar */
static const int vertpadbar         = 14;        /* vertical padding for statusbar */
static const int vertpad            = 8;       /* vertical padding of bar */
static const int sidepad            = 4;       /* horizontal padding of bar */
static const char *fonts[]          = { "HackNerdFont-Regular:size=14" };
static const char dmenufont[]       = "HackNerdFont-Regular:size=14";
static const char col_bg1[]      = "#0c0e0f";  
static const char col_fg[]       = "#edeff0";  
static const char col_fg1[]      = "#e4e6e7";
static const char col_bg2[]      = "#232526";  
static const char col_accent1[]  = "#709ad2";  
static const char col_accent2[]  = "#70b8ca";  

static const char *colors[][3]      = {
    /*               fg         bg         border   */
    [SchemeNorm] = { col_fg, col_bg1, col_accent2 },
    [SchemeSel]  = { col_accent2, col_bg2,  col_accent1  },
    [SchemeTitle]  = { col_fg1, col_bg2, col_accent2 },
};

static const char *tagsel[][2] = {
    /* fg         bg */
    { "#bc83e3","#232526" }, 
    { "#df5b61","#232526"},  
    { "#78b892","#232526"},  
    { "#de8f78","#232526"},  
    { "#6791c9","#232526"},  
    { "#bc83e3","#232526"},  
    { "#67afc1","#232526"},  
    { "#e4e6e7","#232526"},  
    { "#e79881","#232526"},  
};

/* tagging */
static const char *tags[]         = { "一", "二", "三", "四", "五", "六", "七", "八","九" };
static const char *tagsalt[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const int momentaryalttags = 0; /* 1 means alttags will show only when key is held down*/

static const unsigned int ulinepad	= 5;	/* horizontal padding between the underline and tag */
static const unsigned int ulinestroke	= 4;	/* thickness / height of the underline */
static const unsigned int ulinevoffset	= 2;	/* how far above the bottom of the bar the line should appear */
static const int ulineall 		= 0;	/* 1 to show underline on all tags, 0 for just the active ones */


static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",    NULL,     NULL,           0,         1,          0,           0,        -1 },
	{ "Firefox", NULL,     NULL,           1 << 8,    0,          0,          -1,        -1 },
	{ "st",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "H[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", 
 // "-m", dmenumon,
 // "-fn",
 // dmenufont, 
 // "-nb", 
 // col_gray1,
 // "-nf",
 // col_gray3,
 // "-sb", col_cyan,
 // "-sf",
 // col_gray4,
  NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *printcmd[] = {"flameshot", "gui", NULL};
static const char *fmcmd[]    = {"pcmanfm", NULL};
static const char *browcmd[]  = { "firefox", NULL};
/*wallpapers*/

static const char *variety_prev[]  = { "sh", "-c", "variety -p && dunstify 'Variety' 'Previous wallpaper'", NULL };
static const char *variety_fav[]   = { "sh", "-c", "variety -f && dunstify 'Variety' 'Added to Favorites'", NULL };
static const char *variety_next[]  = { "sh", "-c", "variety -n && dunstify 'Variety' 'Next wallpaper'", NULL };
static const char *variety_trash[] = { "sh", "-c", "variety -t && dunstify 'Variety' 'Wallpaper trashed'", NULL };

/* volume commands */
static const char *volupcmd[] =   { "sh", "-c", "pactl set-sink-volume @DEFAULT_SINK@ +1% && kill -41 $(pidof dwmblocks)", NULL };
static const char *voldowncmd[] = { "sh", "-c", "pactl set-sink-volume @DEFAULT_SINK@ -1% && kill -41 $(pidof dwmblocks)", NULL};
static const char *volmutecmd[] = {"sh", "-c", "pactl set-sink-mute @DEFAULT_SINK@ toggle && kill -41 $(pidof dwmblocks)", NULL};
static const Key keys[] = {

	/* modifier                     key        function        argument */
  /*commands */
  {MODKEY,                       XK_p,       spawn,        {.v = dmenucmd } },
  {MODKEY|ShiftMask,             XK_Return,  spawn,        {.v = termcmd } },
  {MODKEY|ShiftMask,             XK_p,       spawn,        {.v = printcmd}},
  {MODKEY|ShiftMask,             XK_f,       spawn,        {.v = fmcmd}},
  {MODKEY,                       XK_b,       spawn,        {.v = browcmd } },
  /*wallpaper*/
  { Mod1Mask,                   XK_p,        spawn,        {.v = variety_prev } },  
  { Mod1Mask,                   XK_f,        spawn,        {.v = variety_fav } },  
  { Mod1Mask,                   XK_n,        spawn,        {.v = variety_next } },  
  { Mod1Mask,                   XK_t,        spawn,        {.v = variety_trash } }, 
    /*volume*/ 
  {0, XF86XK_AudioRaiseVolume,               spawn,        {.v = volupcmd}},
  {0, XF86XK_AudioLowerVolume,               spawn,        {.v = voldowncmd}},
  {0, XF86XK_AudioMute,                      spawn,        {.v = volmutecmd}},
  /*tags*/
  TAGKEYS(                      XK_1,                      0)
  TAGKEYS(                      XK_2,                      1)
  TAGKEYS(                      XK_3,                      2)
  TAGKEYS(                      XK_4,                      3)
  TAGKEYS(                      XK_5,                      4)
  TAGKEYS(                      XK_6,                      5)
  TAGKEYS(                      XK_7,                      6)
  TAGKEYS(                      XK_8,                      7)
  TAGKEYS(                      XK_9,                      8)
  /*gaps  */
  {MODKEY,                      XK_plus,    incrgaps,        {.i = +3}},
  {MODKEY,                      XK_minus, incrgaps,          {.i = -3}},
  {MODKEY|ShiftMask,            XK_BackSpace, togglegaps,    {0}},
  {MODKEY,                      XK_Tab,       defaultgaps,   {0}},
	{MODKEY,                      XK_n,      togglealttag,     {0} },
   /*layouts*/
  { MODKEY,                     XK_t,      setlayout,      {.v = &layouts[0]} },
  { MODKEY,                     XK_e,      setlayout,      {.v = &layouts[1]} },
  { MODKEY,                     XK_w,      setlayout,      {.v = &layouts[13]} },
  { MODKEY|ShiftMask,           XK_w,      setlayout,      {.v = &layouts[5]} },
  { MODKEY,                     XK_x,      setlayout,      {.v = &layouts[6]} },
  { MODKEY|ShiftMask,           XK_x,      setlayout,      {.v = &layouts[3]} },
  { MODKEY,                     XK_z,      setlayout,      {.v = &layouts[11]} },
  { MODKEY|ShiftMask,           XK_z,      setlayout,      {.v = &layouts[7]} },
  { MODKEY,                     XK_space,  setlayout,      {0} },
  { MODKEY|ShiftMask,           XK_space,  togglefloating, {0} },
  { MODKEY|ControlMask,		      XK_comma,  cyclelayout,    {.i = -1 } },
  { MODKEY|ControlMask,         XK_period, cyclelayout,    {.i = +1 } },
  { MODKEY,                     XK_f,      togglefullscr,  {0} },

	{ MODKEY|ControlMask,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                     XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                     XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                     XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,           XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,           XK_k,      movestack,      {.i = -1 } },
	{ MODKEY,                     XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                     XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                     XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,           XK_h,      setcfact,       {.f = +0.25} },
	{ MODKEY|ShiftMask,           XK_l,      setcfact,       {.f = -0.25} },
	{ MODKEY|ShiftMask,           XK_o,      setcfact,       {.f =  0.00} },
	{ MODKEY,                     XK_Return, zoom,           {0} },
	{ MODKEY,                     XK_Tab,    view,           {0} },
	{ MODKEY,                     XK_q,      killclient,     {0} },
	{ MODKEY,                     XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,           XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                     XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                     XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,           XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,           XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                     XK_Escape, quit,           {0} },
	{ MODKEY|ControlMask,         XK_r,      quit,           {1} }, 
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },

 // Klicka på statustexten och skicka signaler
  { ClkStatusText, 0, Button1, sigstatusbar, {.i = 1} }, // Signal 2 för pkg_modul
  { ClkStatusText, 0, Button2, sigstatusbar, {.i = 2} }, // Signal 6 för cpu_modul
  { ClkStatusText, 0, Button3, sigstatusbar, {.i = 3} }, // Signal 3 för mem_modul
  { ClkStatusText, 0, Button4, sigstatusbar, {.i = 4} }, // Signal 4 för cpu_temp_modul
  { ClkStatusText, 0, Button2, sigstatusbar, {.i = 5} }, // Signal 5 för clock_modul
  { ClkStatusText|ShiftMask, 0, Button3, sigstatusbar, {.i = 6} }, // Signal 5 för clock_modul
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

