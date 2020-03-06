#include "main.h"
#ifndef GUI_H
#define GUI_H

extern int auton_sel;
extern float kP;
extern float kI;
extern float kD;

extern lv_obj_t * tabview;
extern lv_obj_t * debugtxt;
extern lv_obj_t * debugpid;

static lv_obj_t * tab1;
static lv_obj_t * tab2;
static lv_obj_t * tab3;
static lv_obj_t * tab4;
static lv_obj_t * g_sb_label;  // sb text label
extern lv_obj_t * debuglabel;
static lv_obj_t * pid_label;

static lv_obj_t *g_btn_region; //tab view region of the screen
static lv_obj_t *g_sb_region; //status bar region of the screen

void lv_ex_tabview_1(void);
void gui(void);
#endif // GUI_H
