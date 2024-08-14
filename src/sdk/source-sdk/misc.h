#pragma once

enum signonstate_t {
	signonstate_none        = 0,
	signonstate_challenge   = 1,
	signonstate_connected   = 2,
	signonstate_new         = 3,
	signonstate_prespawn    = 4,
	signonstate_spawn       = 5,
	signonstate_full        = 6,
	signonstate_changelevel = 7,
};

typedef unsigned long h_font;

enum paint_mode_t {
	paint_uipanels     = ( 1 << 0 ),
	paint_ingamepanels = ( 1 << 1 ),
};

enum font_flags_t {
	fontflag_none,
	fontflag_italic       = 0x001,
	fontflag_underline    = 0x002,
	fontflag_strikeout    = 0x004,
	fontflag_symbol       = 0x008,
	fontflag_antialias    = 0x010,
	fontflag_gaussianblur = 0x020,
	fontflag_rotary       = 0x040,
	fontflag_dropshadow   = 0x080,
	fontflag_additive     = 0x100,
	fontflag_outline      = 0x200,
	fontflag_custom       = 0x400,
	fontflag_bitmap       = 0x800,
};

enum button_code_t {
	button_code_invalid = -1,
	button_code_none    = 0,

	key_first = 0,

	key_none = key_first,
	key_0,
	key_1,
	key_2,
	key_3,
	key_4,
	key_5,
	key_6,
	key_7,
	key_8,
	key_9,
	key_a,
	key_b,
	key_c,
	key_d,
	key_e,
	key_f,
	key_g,
	key_h,
	key_i,
	key_j,
	key_k,
	key_l,
	key_m,
	key_n,
	key_o,
	key_p,
	key_q,
	key_r,
	key_s,
	key_t,
	key_u,
	key_v,
	key_w,
	key_x,
	key_y,
	key_z,
	key_pad_0,
	key_pad_1,
	key_pad_2,
	key_pad_3,
	key_pad_4,
	key_pad_5,
	key_pad_6,
	key_pad_7,
	key_pad_8,
	key_pad_9,
	key_pad_divide,
	key_pad_multiply,
	key_pad_minus,
	key_pad_plus,
	key_pad_enter,
	key_pad_decimal,
	key_lbracket,
	key_rbracket,
	key_semicolon,
	key_apostrophe,
	key_backquote,
	key_comma,
	key_period,
	key_slash,
	key_backslash,
	key_minus,
	key_equal,
	key_enter,
	key_space,
	key_backspace,
	key_tab,
	key_capslock,
	key_numlock,
	key_escape,
	key_scrolllock,
	key_insert,
	key_delete,
	key_home,
	key_end,
	key_pageup,
	key_pagedown,
	key_break,
	key_lshift,
	key_rshift,
	key_lalt,
	key_ralt,
	key_lcontrol,
	key_rcontrol,
	key_lwin,
	key_rwin,
	key_app,
	key_up,
	key_left,
	key_down,
	key_right,
	key_f1,
	key_f2,
	key_f3,
	key_f4,
	key_f5,
	key_f6,
	key_f7,
	key_f8,
	key_f9,
	key_f10,
	key_f11,
	key_f12,
	key_capslocktoggle,
	key_numlocktoggle,
	key_scrolllocktoggle,

	key_last  = key_scrolllocktoggle,
	key_count = key_last - key_first + 1,

	mouse_first = key_last + 1,

	mouse_left = mouse_first,
	mouse_right,
	mouse_middle,
	mouse_4,
	mouse_5,
	mouse_wheel_up,
	mouse_wheel_down,

	mouse_last  = mouse_wheel_down,
	mouse_count = mouse_last - mouse_first + 1,
};

#define fcvar_none                    0
#define fcvar_unregistered            ( 1 << 0 )
#define fcvar_developmentonly         ( 1 << 1 )
#define fcvar_gamedll                 ( 1 << 2 )
#define fcvar_clientdll               ( 1 << 3 )
#define fcvar_hidden                  ( 1 << 4 )
#define fcvar_protected               ( 1 << 5 )
#define fcvar_sponly                  ( 1 << 6 )
#define fcvar_archive                 ( 1 << 7 )
#define fcvar_notify                  ( 1 << 8 )
#define fcvar_userinfo                ( 1 << 9 )
#define fcvar_cheat                   ( 1 << 14 )
#define fcvar_printableonly           ( 1 << 10 )
#define fcvar_unlogged                ( 1 << 11 )
#define fcvar_never_as_string         ( 1 << 12 )
#define fcvar_replicated              ( 1 << 13 )
#define fcvar_demo                    ( 1 << 16 )
#define fcvar_dontrecord              ( 1 << 17 )
#define fcvar_reload_materials        ( 1 << 20 )
#define fcvar_reload_textures         ( 1 << 21 )
#define fcvar_not_connected           ( 1 << 22 )
#define fcvar_material_system_thread  ( 1 << 23 )
#define fcvar_archive_xbox            ( 1 << 24 )
#define fcvar_accessible_from_threads ( 1 << 25 )
#define fcvar_server_can_execute      ( 1 << 28 )
#define fcvar_server_cannot_query     ( 1 << 29 )
#define fcvar_clientcmd_can_execute   ( 1 << 30 )
