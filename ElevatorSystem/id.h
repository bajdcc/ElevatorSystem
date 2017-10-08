#pragma once

// Rect
#define UI_WINDOW_W                 800
#define UI_WINDOW_H                 600
#define UICFG_OS_W                  110
#define UICFG_OS_H                  43
#define UICFG_IS_W                  110
#define UICFG_IS_H                  38
#define UICFG_LL_W                  38
#define UICFG_LL_H                  38

// ID
#define UID_TOP_LAYOUT              9
#define UID_SYS_LAYOUT              10
#define UID_STATE_PANEL             40
#define UID_LEVEL_PANEL             50
#define UID_OUTSIDE_PANEL           60
#define UID_INSIDE_PANEL            70
#define UID_TIMER                   100
#define UID_X_MASK                  2
#define UID_X_DIALOG                3
#define UID_X_CONTROL_CENTER        980
#define UID_X_CONTROL_DISPATCHER    991
#define UID_X_TIMER                 995

// Timer
#define UICFG_X_ELAPSE              30
#define UICFG_NORMAL_TIMER_ELAPSE   1000

// Data
#define UICFG_META_CLK              1
#define UICFG_NPLS                  5
#define UICFG_WAIT                  5
#define UICFG_LEVEL                 6
#define UICFG_START                 1
#define UICFG_ORIGIN                1

// Message
#define UIMSG_Init                  0x1000
#define UIMSG_Reset                 0x1001
#define UIMSG_InvalidValue          0x1002
#define UIMSG_SetValue              0x1003
#define UIMSG_GetValue              0x1004
#define UIMSG_SetTimer              0x1005
#define UIMSG_Disable               0x1006
#define UIMSG_Restore               0x1007
#define UIMSG_Delay                 0x1008