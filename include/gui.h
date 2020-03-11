#include "main.h"
#include "motor.hpp"
#ifndef GUI_H
#define GUI_H

extern int auton_sel;
extern float kP;
extern float kI;
extern float kD;

extern lv_obj_t * tabview;
extern lv_obj_t * debugdrive;
extern lv_obj_t * debugpid;

static lv_obj_t * tab1;
static lv_obj_t * tab2;
static lv_obj_t * tab3;
static lv_obj_t * tab4;
static lv_obj_t * tab5;
static lv_obj_t * gauge1;
static lv_obj_t * g_sb_label;  // sb text label
extern lv_obj_t * debugauto;
static lv_obj_t * pid_label;

static lv_obj_t *g_btn_region; //tab view region of the screen
static lv_obj_t *g_sb_region; //status bar region of the screen

// Remember this is a C header so protect ourselves when this is compiled using C
#ifdef __cplusplus 
// This 'extern "C"' syntax is only valid in C++, not in C.
extern "C" { 
// From now everything has C linkage. 
#endif

void lv_ex_tabview_1(void);

#ifdef __cplusplus 
// Close the brace opened above
}
#endif

#endif // GUI_H
