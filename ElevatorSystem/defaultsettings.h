#pragma once

#define WM_MOUSEENTER                       0x2A2

// States
#define STATE_VISIBLE                       0x00000001
#define STATE_ENABLED                       0x00000002
#define STATE_HOT                           0x00000004
#define STATE_FOCUSED                       0x00000008
#define STATE_PRESSED                       0x00000010
#define STATE_CAPTURED                      0x00000020
#define STATE_READONLY                      0x00000040
#define STATE_PASSWORD                      0x00000080
#define STATE_CURSOR                        0x00000100
#define STATE_MASK                          0x00000200
#define STATE_SETFOCUS                      0x00000400
#define STATE_NOID                          0x00000800
#define STATE_HOTTRACK                      0x00001000
#define STATE_NOACTIVATE                    0x00002000

#define UITYPE_BUTTON_NORMAL                0
#define UITYPE_BUTTON_RECTANGLE             UITYPE_BUTTON_NORMAL
#define UITYPE_BUTTON_CIRCLE                1

#define UIFIND_VISIBLE                      0x00000001                
#define UIFIND_ENABLED                      0x00000002
#define UIFIND_HITTEST                      0x00000004
#define UIFIND_ME_FIRST                     0x00000008
#define UIFIND_TOP_FIRST                    0x00000010

// Controls
#define UIDFT_CONTROL_STATE                 ( STATE_VISIBLE | STATE_ENABLED )
#define UIDFT_CONTROL_NAME                  _T("Gui Object")
#define UIDFT_CONTROL_ID                    0
#define UIDFT_CONTROL_DIRECT_ID             0
#define UIDFT_CONTROL_TRANSPARENT           false
#define UIDFT_CONTROL_COLOR                 Color::WhiteSmoke
#define UIDFT_CONTROL_HITTEST               HTCLIENT

// Buttons
#define UIDFT_BUTTON_TYPE                   UITYPE_BUTTON_NORMAL
#define UIDFT_BUTTON_COLOR                  Color(236, 236, 240)
#define UIDFT_BUTTON_NORMAL_FONT_COLOR      Color::Black
#define UIDFT_BUTTON_HOT_FONT_COLOR         Color::Black
#define UIDFT_BUTTON_FOCUSED_FONT_COLOR     Color::Black
#define UIDFT_BUTTON_PRESSED_FONT_COLOR     Color::White
#define UIDFT_BUTTON_NORMAL_BK_COLOR        Color(236, 236, 240)
#define UIDFT_BUTTON_HOT_BK_COLOR           Color(201, 222, 245)
#define UIDFT_BUTTON_FOCUSED_BK_COLOR       Color(201, 222, 245)
#define UIDFT_BUTTON_PRESSED_BK_COLOR       Color(0, 122, 204)
#define UIDFT_BUTTON_NORMAL_FRAME_COLOR     Color(199, 201, 216)
#define UIDFT_BUTTON_HOT_FRAME_COLOR        Color(204, 206, 219)
#define UIDFT_BUTTON_FOCUSED_FRAME_COLOR    Color(204, 206, 219)
#define UIDFT_BUTTON_PRESSED_FRAME_COLOR    Color(28, 151, 234)
#define UIDFT_BUTTON_FRAME_WIDTH            1.0f

// Dialog
#define UIDFT_DIALOG_FRAME_COLOR            Color(0, 122, 204)
#define UIDFT_DIALOG_FRAME_WIDTH            1.0f

// ×ÖÌå
#define UIDFT_FONT_FAMILY                   _T("ËÎÌå")
#define UIDFT_FONT_SIZE                     12
#define UIDFT_FONT_COLOR                    Color::Black
#define UIDFT_FONT_UNIT                     UnitPoint
#define UIDFT_FONT_STYLE                    FontStyleRegular
#define UIDFT_FONT_FORMAT                   0
#define UIDFT_FONT_H_ALIGN                  StringAlignmentCenter
#define UIDFT_FONT_V_ALIGN                  StringAlignmentCenter
#define UIDFT_FONT_TRIMMING                 StringTrimmingNone