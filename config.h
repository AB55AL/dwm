/* See LICENSE file for copyright and license details. */

/* appearance */
#include <X11/X.h>
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int gappx     = 5;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int usealtbar          = 1;        /* 1 means use non-dwm status bar */
static const char *altbarclass      = "Polybar"; /* Alternate bar class name */
static const char *altbarcmd        = "$HOME/.config/polybar/start-polybar.sh 2> /dev/null &"; /* Alternate bar launch command */
static const char *fonts[]          = { "SourceCodePro:size=8", "monospace:size=10" };
static const char dmenufont[]       = "SourceCodePro:size=8";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  "#FFFFFF"  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",         NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",      NULL,       NULL,       1 << 8,       0,           -1 },
	{ "steam_app_0",  NULL,       NULL,       1 << 4,       1,            0 },
	{ "steam_app",    NULL,       NULL,       1 << 4,       1,            0 },
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
static const int attachbelow = 1;    /* 1 means attach after the currently active window */

static const Layout layouts[LAYOUTS_LENGTH] = {
	/* first entry is default */
	/* symbol     arrange function */
	{ "Half",      half_tile },
	{ "Max",       monocle   },
	{ "Tile",      tile		   },    
	// { "Float",      NULL },    /* no layout function means floating behavior */
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

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]           = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *rofi_run[]           = { "rofi", "-show", "run", NULL };
static const char *rofi_switch_window[] = { "rofi", "-show", "window", NULL };
static const char *disable_redshift[]   = { "redshift", "-m", "randr:crtc=0", "-x", NULL };
static const char *enable_redshift[]    = { "redshift", "-m", "randr:crtc=0", "-PO", "5000", NULL };


static const char *termcmd[]  = { "st", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_z,      spawn,          {.v = rofi_run } },
	{ MODKEY,                       XK_t,      spawn_term_once,{.v = termcmd} },
	{ MODKEY|ShiftMask,             XK_t,      spawn          ,{.v = termcmd} },
	{ MODKEY,                       XK_i,      spawn,          {.v = rofi_switch_window } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_l,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_h,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_j,      setmfact,       {.f = -0.05} }, // Change windows size
	{ MODKEY,                       XK_k,      setmfact,       {.f = +0.05} }, // Change windows size
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_space,  setlayout,      {0} }, // Cycle layouts
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } }, // View all windows
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } }, // Put focused window on all tags
	{ MODKEY|ControlMask,           XK_l,      focusmon,       {.i  = +1 } }, // Focus next monitor
	{ MODKEY|ControlMask,           XK_h,      focusmon,       {.i  = -1 } },
	{ MODKEY,                       XK_o,      tagmon,         {.i  = -1 } }, // Move window to next monitor
	{ MODKEY,												XK_m,      togglefullscr,  {0} },
	{ MODKEY,												XK_a,      focusmaster,    {0} },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	{ MODKEY|ShiftMask,             XK_r,      quit,           {0} }, // quit or reload if running through startdwm.sh
	// { MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	// { MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	// { MODKEY,                       XK_f,      setlayout,      {.v = &layouts[0]} },
	// { MODKEY,                       XK_m,      setlayout,      {.v = &layouts[1]} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	// { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	// { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	// { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
};
