/* See LICENSE file for copyright and license details. */

/* Constants */
#define TERMINAL  "st"
#define TERMCLASS "St"
#define BROWSER   "librewolf-bin"
#define BROWSER2  "firefox-bin"

/* appearance */
static unsigned int borderpx  = 5;        /* border pixel of windows */
static unsigned int snap      = 32;       /* snap pixel */
static unsigned int gappih    = 20;       /* horiz inner gap between windows */
static unsigned int gappiv    = 10;       /* vert inner gap between windows */
static unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static unsigned int gappov    = 30;       /* vert outer gap between windows and screen edge */
static int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
static char *fonts[]          = { "Iosevka Comfy:pixelsize=16", "NotoColorEmoji:pixelsize=12:antialias=true:autohint=true"  };
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

typedef struct {
    const char *name;
    const void *cmd;
} Sp;
const char *spcmd1[] = {TERMINAL, "-n", "spterm", "-g", "120x34", NULL };
const char *spcmd2[] = {TERMINAL, "-n", "spcalc", "-g", "50x20", "-e", "bc", "-lq", NULL };
const char *spcmd3[] = {TERMINAL, "-n", "spnote", "-g", "60x20", "-e", "vim", "~/note.md", NULL };
static Sp scratchpads[] = {
    /* name          cmd  */
    {"spterm",      spcmd1},
    {"spcalc",      spcmd2},
    {"spnote",      spcmd3},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
    /* xprop(1):
     *  WM_CLASS(STRING) = instance, class
     *  WM_NAME(STRING) = title
    */
    /* class    instance      title          tags mask    isfloating   isterminal  noswallow  monitor */
    { "Gimp",     NULL,       NULL,             1 << 8,       0,           0,         0,        -1 },
    { "mpv",      NULL,       NULL,             1 << 7,       0,           0,         0,         1 },
    { TERMCLASS,  NULL,       NULL,             0,            0,           1,         0,        -1 },
    { NULL,       NULL,       "Event Tester",   0,            0,           0,         1,        -1 },
    { TERMCLASS,      "floatterm", NULL,                0,       1,           1,         0,        -1 },
    { TERMCLASS,      "bg",        NULL,                1 << 7,       0,           1,         0,        -1 },
    { TERMCLASS,      "spterm",    NULL,                SPTAG(0),     1,           1,         0,        -1 },
    { TERMCLASS,      "spcalc",    NULL,                SPTAG(1),     1,           1,         0,        -1 },
    { TERMCLASS,      "spnote",    NULL,                SPTAG(2),     1,           1,         0,        -1 },
};

/* layout(s) */
static float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
static const Layout layouts[] = {
    /* symbol     arrange function */
    { "[]=",    tile },             /* Default: Master on left, slaves on right */
    { "TTT",    bstack },       /* Master on top, slaves on bottom */

    { "[@]",    spiral },       /* Fibonacci spiral */
    { "[\\]",   dwindle },      /* Decreasing in size right and leftward */

    { "[D]",    deck },             /* Master on left, slaves in monocle-like mode on right */
    { "[M]",    monocle },      /* All windows on top of eachother */

    { "|M|",    centeredmaster },       /* Master in middle, slaves on sides */
    { ">M>",    centeredfloatingmaster },   /* Same but master floats */

    { "><>",    NULL },             /* no layout function means floating behavior */
    { NULL,         NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
    { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
    { MOD,  XK_j,   ACTION##stack,  {.i = INC(+1) } }, \
    { MOD,  XK_k,   ACTION##stack,  {.i = INC(-1) } }, \
    { MOD,  XK_v,   ACTION##stack,  {.i = 0 } }, \
    /* { MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \ */
    /* { MOD, XK_a,     ACTION##stack, {.i = 1 } }, \ */
    /* { MOD, XK_z,     ACTION##stack, {.i = 2 } }, \ */
    /* { MOD, XK_x,     ACTION##stack, {.i = -1 } }, */

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *termcmd[]  = { TERMINAL, NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
    { "background",     STRING,     &normbordercolor },
    { "selbackground",      STRING,     &selbordercolor },
    { "background",         STRING,     &normbgcolor },
    { "foreground",         STRING,     &normfgcolor },
    { "selforeground",      STRING,     &selfgcolor },
    { "selbackground",      STRING,     &selbgcolor },
    { "borderpx",           INTEGER, &borderpx },
    { "snap",               INTEGER, &snap },
    { "showbar",            INTEGER, &showbar },
    { "topbar",             INTEGER, &topbar },
    { "nmaster",            INTEGER, &nmaster },
    { "resizehints",        INTEGER, &resizehints },
    { "mfact",              FLOAT,  &mfact },
    { "gappih",             INTEGER, &gappih },
    { "gappiv",             INTEGER, &gappiv },
    { "gappoh",             INTEGER, &gappoh },
    { "gappov",             INTEGER, &gappov },
    { "swallowfloating",    INTEGER, &swallowfloating },
    { "smartgaps",          INTEGER, &smartgaps },
};

#include <X11/XF86keysym.h>
#include "shiftview.c"

static const Key keys[] = {
    /* modifier                     key        function        argument */
    STACKKEYS(MODKEY,                          focus)
    STACKKEYS(MODKEY|ShiftMask,                push)
    { MODKEY,               XK_grave,   view,       {0} },
    TAGKEYS(                XK_1,       0)
    TAGKEYS(                XK_2,       1)
    TAGKEYS(                XK_3,       2)
    TAGKEYS(                XK_4,       3)
    TAGKEYS(                XK_5,       4)
    TAGKEYS(                XK_6,       5)
    TAGKEYS(                XK_7,       6)
    TAGKEYS(                XK_8,       7)
    TAGKEYS(                XK_9,       8)
    { MODKEY,               XK_0,       view,       {.ui = ~0 } },
    { MODKEY|ShiftMask,     XK_0,       tag,        {.ui = ~0 } },
    { MODKEY,               XK_Tab,     focusstack, {.i = +1 } },
    { MODKEY,               XK_q,       killclient, {0} },
    { MODKEY|ShiftMask,     XK_q,       quit,       {0} },

    // monitors
    { MODKEY|ShiftMask,       XK_h,    focusmon,   {.i = -1 } },
    { MODKEY|ControlMask,     XK_h,    tagmon,     {.i = -1 } },
    { MODKEY|ShiftMask,       XK_l,    focusmon,   {.i = +1 } },
    { MODKEY|ControlMask,     XK_l,    tagmon,     {.i = +1 } },

    { MODKEY,               XK_w,       spawn,      {.v = (const char*[]){ BROWSER,  NULL } } },
    { MODKEY|ShiftMask,     XK_w,       spawn,      {.v = (const char*[]){ BROWSER2, NULL } } },
    { MODKEY,               XK_t,       setlayout,  {.v = &layouts[0]} }, /* tile */
    { MODKEY|ShiftMask,     XK_t,       setlayout,  {.v = &layouts[1]} }, /* bstack */
    { MODKEY,               XK_y,       setlayout,  {.v = &layouts[2]} }, /* spiral */
    { MODKEY|ShiftMask,     XK_y,       setlayout,  {.v = &layouts[3]} }, /* dwindle */
    { MODKEY,               XK_u,       setlayout,  {.v = &layouts[4]} }, /* deck */
    { MODKEY|ShiftMask,     XK_u,       setlayout,  {.v = &layouts[5]} }, /* monocle */
    { MODKEY,               XK_i,       setlayout,  {.v = &layouts[6]} }, /* centeredmaster */
    { MODKEY|ShiftMask,     XK_i,       setlayout,  {.v = &layouts[7]} }, /* centeredfloatingmaster */
    { MODKEY,               XK_o,       incnmaster,     {.i = +1 } },
    { MODKEY|ShiftMask,     XK_o,       incnmaster,     {.i = -1 } },
    { MODKEY,               XK_equal,   resetlayout,    {0}},
    { MODKEY,               XK_a,       togglegaps,     {0} },
    { MODKEY|ShiftMask,     XK_a,       defaultgaps,    {0} },
    { MODKEY,               XK_s,       togglesticky,   {0} },
    { MODKEY,               XK_d,       spawn,          SHCMD("dmenu_run -p 'Run: '") },
    /* { MODKEY,               XK_e,       spawn,      SHCMD("$HOME/.local/bin/scripts/emacs/emacs-daemon.sh start") }, */
    /* { MODKEY|ShiftMask,     XK_e,       spawn,      SHCMD("$HOME/.local/bin/scripts/emacs/emacs-daemon.sh kill") }, */
    { MODKEY,               XK_n,       spawn,      SHCMD("emacsclient -c") },
    { MODKEY,               XK_e,       spawn,      SHCMD("emacs") },
    { MODKEY|ShiftMask,     XK_d,       spawn,      SHCMD("$HOME/.local/bin/scripts/dired_selector") },
    { MODKEY|ShiftMask,     XK_n,       spawn,      SHCMD("$HOME/.local/bin/scripts/emacs-launcher") },
    { MODKEY,               XK_m,       spawn,      SHCMD("emacsclient -c -e '(emms)'") },
    { MODKEY|ShiftMask,     XK_m,       spawn,      SHCMD("$HOME/.local/bin/scripts/wallpaper.sh") },
    { MODKEY|ShiftMask,     XK_x,       spawn,      SHCMD("$HOME/.local/bin/scripts/sysact") },
    { MODKEY|ShiftMask,     XK_b,       spawn,      SHCMD("boomer") },
    { MODKEY,               XK_f,       togglefullscr,  {0} },
    { MODKEY,               XK_h,       setmfact,   {.f = -0.05} },
    /* J and K are automatically bound above in STACKEYS */
    { MODKEY,               XK_l,           setmfact,   {.f = +0.05} },
    { MODKEY,               XK_semicolon,   shiftview,  { .i = 1 } },
    { MODKEY|ShiftMask,     XK_semicolon,   shifttag,   { .i = 1 } },
    { MODKEY,               XK_apostrophe,  togglescratch,  {.ui = 1} },
    { MODKEY,               XK_Return,      spawn,      {.v = termcmd } },
    { MODKEY|ShiftMask,     XK_Return,      togglescratch,  {.ui = 0} },
    { MODKEY,               XK_BackSpace,   togglescratch,  {.ui = 2} },
    { MODKEY,               XK_z,           incrgaps,   {.i = +3 } },
    { MODKEY|ShiftMask,     XK_z,           incrgaps,   {.i = -3 } },
    /* V is automatically bound above in STACKKEYS */
    { MODKEY,               XK_b,               togglebar,  {0} },

    { MODKEY,               XK_p,       spawn,  SHCMD(TERMINAL " -e alsamixer; kill -44 $(pidof dwmblocks)") },
    { MODKEY,               XK_space,   zoom,       {0} },
    { MODKEY|ShiftMask,     XK_space,   togglefloating, {0} },

    { 0,                XK_Print,   spawn,      SHCMD("maim $HOME/Pictures/screenshots/pic-full-$(date '+%y%m%d-%H%M-%S').png") },
    { ShiftMask,        XK_Print,   spawn,      SHCMD("$HOME/.local/bin/scripts/maimpick") },

    { 0, XF86XK_AudioMute,          spawn,      SHCMD("pactl set-sink-mute @DEFAULT_SINK@ toggle; kill -44 $(pidof dwmblocks)") },
    { 0, XF86XK_AudioRaiseVolume,   spawn,      SHCMD("pactl set-sink-volume @DEFAULT_SINK@ +10%; kill -44 $(pidof dwmblocks)") },
    { 0, XF86XK_AudioLowerVolume,   spawn,      SHCMD("pactl set-sink-volume @DEFAULT_SINK@ -10%; kill -44 $(pidof dwmblocks)") },
    { 0, XF86XK_AudioMicMute,       spawn,      SHCMD("pactl set-source-mute @DEFAULT_SOURCE@ toggle") },

    { 0, XF86XK_AudioPrev,      spawn,      {.v = (const char*[]){ "playerctl", "previous", NULL } } },
    { 0, XF86XK_AudioNext,      spawn,      {.v = (const char*[]){ "playerctl", "next", NULL } } },
    { 0, XF86XK_AudioPause,     spawn,      {.v = (const char*[]){ "playerctl", "pause", NULL } } },
    { 0, XF86XK_AudioPlay,      spawn,      {.v = (const char*[]){ "playerctl", "play", NULL } } },

    { 0, XF86XK_MonBrightnessUp,    spawn,      {.v = (const char*[]){ "xbacklight", "-inc", "5", NULL } } },
    { 0, XF86XK_MonBrightnessDown,  spawn,      {.v = (const char*[]){ "xbacklight", "-dec", "5", NULL } } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
    /* click                event mask      button          function        argument */
#ifndef __OpenBSD__
    { ClkWinTitle,          0,              Button2,        zoom,           {0} },
    { ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
    { ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
    { ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
    { ClkStatusText,        0,              Button4,        sigdwmblocks,   {.i = 4} },
    { ClkStatusText,        0,              Button5,        sigdwmblocks,   {.i = 5} },
    { ClkStatusText,        ShiftMask,      Button1,        sigdwmblocks,   {.i = 6} },
#endif
    // { ClkStatusText,        ShiftMask,      Button3,        spawn,          SHCMD(TERMINAL " -e nvim ~/.local/src/dwmblocks/config.h") },
    { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
    { ClkClientWin,         MODKEY,         Button2,        defaultgaps,    {0} },
    { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
    { ClkClientWin,         MODKEY,         Button4,        incrgaps,   {.i = +1} },
    { ClkClientWin,         MODKEY,         Button5,        incrgaps,   {.i = -1} },
    { ClkTagBar,            0,              Button1,        view,           {0} },
    { ClkTagBar,            0,              Button3,        toggleview,     {0} },
    { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
    { ClkTagBar,            0,              Button4,        shiftview,  {.i = -1} },
    { ClkTagBar,            0,              Button5,        shiftview,  {.i = 1} },
    { ClkRootWin,           0,              Button2,        togglebar,  {0} },
};
