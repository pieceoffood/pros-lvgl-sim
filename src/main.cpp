

#include "main.h"
#include "motor.hpp"


#if USE_PROS_LVGL_SIM == 1
#include "ncrapi_lvgl_sim_kernel/src/ncrLvglSimKernel.hpp"


#endif





#include "gui.h"
#include "main.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

// credit to https://github.com/timeconfusing/v5gui with modify
// credit to https://github.com/kunwarsahni01/Vex-Autonomous-Selector

// forward declaration of following objects
// text to diplay motor and others

lv_obj_t * debugdrive ;
lv_obj_t * tabview ;
lv_obj_t * debugauto ;
lv_obj_t * debugpid;

int auton_sel = 0;
float kP=0.0;
float kI=0.0;
float kD=0.0;
float kM=1.0;

/**
 * call back function for btn
 * get button ID assigned by user
 */
static void btnevent(lv_obj_t * obj, lv_event_t event){
  if(event == LV_EVENT_CLICKED) {
    int id=9;
    id = (int ) lv_obj_get_user_data(obj);
    printf("%s %d was pressed\n", "button", id );
  }
}

/**
 * create btn
 * get it's number assigned by use with call-back function
 */
void gui_btn(void) {
  lv_obj_t * btn = lv_btn_create( tab5, NULL);
  lv_obj_align(btn, NULL, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_user_data(btn,  (lv_obj_user_data_t) 70);
  lv_obj_set_event_cb(btn,  btnevent);
  lv_btn_set_fit2(btn, LV_FIT_TIGHT, LV_FIT_TIGHT);

  lv_obj_t * label1;//declare label1 for btn1
  label1 = lv_label_create(btn, NULL);//initiate label1
  lv_label_set_text(label1, "Press");
}  

/*
void gui_btn(void) {
  lv_obj_t * btn = lv_btn_create( tab3, NULL);
  lv_obj_set_user_data(btn,  (int *) buttonid);
  lv_obj_set_event_cb(btn,  btnevent);
  lv_btn_set_fit2(btn, LV_FIT_TIGHT, LV_FIT_TIGHT);

  lv_obj_t * label1;//declare label1 for btn1
  label1 = lv_label_create(btn, NULL);//initiate label1
  lv_label_set_text(label1, "Press");
}  
*/

/**
 * call back function to select auton in button matrix
 */
static void btnm_action(lv_obj_t * btnm, lv_event_t event) {
  const char * btnmtxt ;
    
  int btnm_num=0;
  if(event == LV_EVENT_VALUE_CHANGED) {
    uint16_t id = lv_btnm_get_active_btn(btnm);    
    btnmtxt = lv_btnm_get_active_btn_text(btnm);
    btnm_num = lv_btnm_get_active_btn(btnm)+1;
    lv_btnm_clear_btn_ctrl_all(btnm,LV_BTNM_CTRL_TGL_STATE); // void the second click. btn will still be toggoled
    lv_label_set_text(g_sb_label, btnmtxt);
    auton_sel = btnm_num;
    printf("%s was pressed button num %d\n", btnmtxt, btnm_num);
  }
  lv_obj_align(g_sb_label, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -20);
}

/**
 * use button matrix create auton selection menu
 */
void gui_btnm(void) {
  // Create a button descriptor string array w/ no repeat "\224"
  static const char * btnm_map[] = { "\2241 red big", "\2242 red small", "\2243 blue big", "\2244 blue small", "\n",
                                     "\2245 Skill 1", "\2246 Skill 2", "\2247 Skill 3", "" };
  //The escape section prevents a press of the button being interpreted as a multipress of the button
  // Create a default button matrix* no repeat
  lv_obj_t *btnm = lv_btnm_create(tab1, NULL);
  lv_obj_set_size(btnm, lv_obj_get_width(tab1)-20,
                  lv_obj_get_height(tab1)-60);
  lv_btnm_set_map(btnm, btnm_map);
  lv_btnm_set_btn_ctrl_all(btnm, LV_BTNM_CTRL_TGL_ENABLE);
  lv_btnm_set_one_toggle(btnm, true);
  lv_obj_set_event_cb(btnm, btnm_action);
  g_sb_label = lv_label_create(tab1, NULL);
  lv_label_set_text(g_sb_label, "Please select Auton");
  lv_obj_align(g_sb_label, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -20); // must be after set_text
}


/**
 * call back funtion for button matrix to tune PID
 */
static void pidbtnm_action(lv_obj_t * btnm, lv_event_t event) {
  char pidtext[100];
  const char * btnmtxt;
  static int btnm_num =0 ;
    if(event == LV_EVENT_VALUE_CHANGED) {
      btnm_num = lv_btnm_get_active_btn(btnm)+1;
      btnmtxt = lv_btnm_get_active_btn_text(btnm);

      switch (btnm_num) {
        case 1:
        kP += kM;
        break;
        case 5:
        kP -= kM;
        break;
        case 2:
        kI += kM;
        break;
        case 6:
        kI -= kM;
        break;
        case 3:
        kD += kM;
        break;
        case 7:
        kD -= kM;
        break;
        case 4:
        kM = kM*10;
        break;
        case 8:
        kM = kM/10;
        break;
      }
      // force kP kI kD be non-negative
      kP = (kP >= 0 ? kP : 0);
      kI = (kI >= 0 ? kI : 0);
      kD = (kD >= 0 ? kD : 0);
      printf("button %s button %d was pressed kP %8.2f  kI %8.2f kD %8.2f kM %8.2f \n",  btnmtxt, btnm_num, kP, kI, kD, kM);
      sprintf(pidtext, "kP %3.5f kI %3.5f  kD %3.5f  kM %4.5f  ",
                kP, kI, kD, kM
      );
      lv_label_set_text(pid_label, pidtext);
    }
  // must be after set_text
  lv_obj_align(pid_label, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -20);
}

/**
 * use button matrix create auton selection menu
 */
void pid_btnm(void) {
  // Create a button descriptor string array w/ no repeat "\224"
  //gui_btnm();
  static const char * btnm_map[] = { "\2241 P+", "\2242 I+", "\2243 D+", "\2244 M+", "\n",
                                      "\n",
                                     "\2245 P-", "\2246 I-", "\2247 D-", "\2248 M-","" };

  // Create a default button matrix* no repeat
  lv_obj_t *btnm = lv_btnm_create(tab4, NULL);
  lv_obj_set_size(btnm, lv_obj_get_width(tab1)-30,
                        lv_obj_get_height(tab1)-60);
  lv_btnm_set_map(btnm, btnm_map);
  lv_obj_set_event_cb(btnm, pidbtnm_action);
}


static void event_handler(lv_obj_t * obj, lv_event_t event)
{
  switch(event) {
    case LV_EVENT_PRESSED:
      printf("Pressed\n");
      break;

    case LV_EVENT_SHORT_CLICKED:
      printf("Short clicked\n");
      break;

    case LV_EVENT_CLICKED:
      printf("Clicked\n");
      break;

    case LV_EVENT_LONG_PRESSED:
      printf("Long press\n");
      break;

    case LV_EVENT_LONG_PRESSED_REPEAT:
      printf("Long press repeat\n");
      break;

    case LV_EVENT_RELEASED:
      printf("Released\n");
      break;
  }
}

static lv_obj_t * label2;
 
static void BttonEventCb(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) {
      int id;
      id = (int ) lv_obj_get_user_data(obj);
      printf("button %d was Clicked\n", (int ) lv_obj_get_user_data(obj) );
    }
    else if(event == LV_EVENT_VALUE_CHANGED) {
      printf("Toggled\n");
		if(lv_btn_get_state(obj) == LV_BTN_STATE_REL){
			lv_label_set_text(label2, "Toggoled\n#ff0000 OFF#");
		}else{
			lv_label_set_text(label2, "Toggoled\n#00ff00 ON#");
		}
    }
}	
 
/**
 * draw a button example
 */ 
void DrawButton()
{
	//1.  button 1
  lv_obj_t * btn1 = lv_btn_create(tab5, NULL);// create btn1 in current screen
  lv_obj_set_event_cb(btn1, BttonEventCb);//set callback function
  lv_obj_align(btn1, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
  lv_obj_set_user_data(btn1,  (lv_obj_user_data_t) 1);
	
  lv_obj_t * label1;//declare label1 for btn1
  label1 = lv_label_create(btn1, NULL);//initiate label1
  lv_label_set_recolor(label1, true); 
  lv_label_set_text(label1, "#0000ff Blue Button#");
 
	//2. button 2
  lv_obj_t * btn2 = lv_btn_create(tab5, NULL);
  lv_obj_set_event_cb(btn2, BttonEventCb);
  lv_obj_align(btn2, NULL, LV_ALIGN_IN_LEFT_MID, 0, 40);
  lv_obj_set_user_data(btn2,  (lv_obj_user_data_t) 2);

  lv_btn_set_toggle(btn2, true);	//set to on/off button，default is release
  lv_btn_toggle(btn2); //set to toggle (on)
  lv_btn_set_fit2(btn2, LV_FIT_NONE, LV_FIT_TIGHT);
 
  label2 = lv_label_create(btn2, NULL);
  lv_label_set_recolor(label2, true);  
  lv_label_set_align(label2, LV_LABEL_ALIGN_CENTER);
  lv_label_set_text(label2, "Toggled");	
 
}

/**
 * display absolute velocity of chassis
 */
void odometer() {
  /*Set the values*/
  lv_gauge_set_value(gauge1, 0, fabs(left_front.get_actual_velocity()));
  lv_gauge_set_value(gauge1, 1, fabs(right_front.get_actual_velocity()));
  lv_gauge_set_value(gauge1, 2, fabs(left_back.get_actual_velocity()));
  lv_gauge_set_value(gauge1, 3, fabs(right_back.get_actual_velocity()));
}

/**
 * create odometer
 */
void lv_ex_gauge_1(void)
{
    /*Create a style*/
    static lv_style_t style;
    lv_style_copy(&style, &lv_style_pretty_color);
    style.body.main_color = lv_color_hex3(0x666);     /*Line color at the beginning*/
    style.body.grad_color =  lv_color_hex3(0x666);    /*Line color at the end*/
    style.body.padding.left = 10;                      /*Scale line length*/
    style.body.padding.inner = 8 ;                    /*Scale label padding*/
    style.body.border.color = lv_color_hex3(0x333);   /*Needle middle circle color*/
    style.line.width = 3;
    // style.text.color = lv_color_hex3(0x333);
    style.text.color = LV_COLOR_WHITE;
    style.line.color = LV_COLOR_RED;                  /*Line color after the critical value*/

    /*Describe the color for the needles*/
    static lv_color_t needle_colors[4];
    needle_colors[0] = LV_COLOR_BLUE;
    needle_colors[1] = LV_COLOR_ORANGE;
    needle_colors[2] = LV_COLOR_PURPLE;
    needle_colors[3] = LV_COLOR_WHITE;

    /*Create a gauge*/
    gauge1 = lv_gauge_create(tab5, NULL);
    lv_gauge_set_style(gauge1, LV_GAUGE_STYLE_MAIN, &style);
    lv_gauge_set_range(gauge1, 0, 200);
    lv_gauge_set_critical_value(gauge1, 160);
    lv_gauge_set_needle_count(gauge1, 4, needle_colors);
    lv_obj_set_size(gauge1, 150, 150);
    lv_obj_align(gauge1, NULL, LV_ALIGN_IN_RIGHT_MID, -20, 20);

    /*Set the values to random number of 0 - 200 */
    lv_gauge_set_value(gauge1, 0, rand() % 200);
    lv_gauge_set_value(gauge1, 1, rand() % 200);
    lv_gauge_set_value(gauge1, 2, rand() % 200);
    lv_gauge_set_value(gauge1, 3, rand() % 200);
}


/**
 * create tab view as main GUI
 */
void lv_ex_tabview_1(void)
{
  // lvgl theme
  lv_theme_t *th = lv_theme_night_init(240, NULL); //Set a HUE 240 value and keep font default BLUE
  //Set a HUE 360 value and keep font default RED
  lv_theme_set_current(th);
  //th = lv_theme_get_current();
  /*Create a Tab view object*/


  tabview = lv_tabview_create(lv_scr_act(), NULL);
  lv_tabview_set_sliding(tabview, false);

  /*Add 4 tabs (the tabs are page (lv_page) and can be scrolled*/
  tab1 = lv_tabview_add_tab(tabview, "Select");
  tab2 = lv_tabview_add_tab(tabview, "Auton");
  tab3 = lv_tabview_add_tab(tabview, "Driver");
  tab4 = lv_tabview_add_tab(tabview, "PID");
  tab5 = lv_tabview_add_tab(tabview, "Odometer");

  lv_tabview_set_tab_act(tabview, 0 , LV_ANIM_OFF);

  debugauto = lv_label_create(tab2, NULL);
  lv_label_set_text(debugauto, "auto sensors and motors");
  debugpid = lv_label_create(tab2, NULL);
  lv_obj_align(debugpid, debugauto, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 80);
  lv_label_set_text(debugpid, "debug PID");

  debugdrive = lv_label_create(tab3,NULL);
  lv_label_set_long_mode(debugdrive, LV_LABEL_LONG_BREAK);     /*Break the long lines*/
  lv_label_set_recolor(debugdrive, true);                      /*Enable re-coloring by commands in the text*/
  lv_label_set_align(debugdrive, LV_LABEL_ALIGN_LEFT);       /*Center aligned lines*/
  lv_label_set_text(debugdrive, "driver sensors and motors");
  lv_obj_set_width(debugdrive, 500);                           /*Set a width*/
  lv_obj_align(debugdrive, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 20);      /*Align to center*/

  pid_label = lv_label_create(tab4, NULL);
  lv_obj_set_style(pid_label, &lv_style_pretty_color);
  lv_label_set_text(pid_label, "PID tuning");
  lv_obj_align(pid_label, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -20);

  gui_btnm();
  pid_btnm();
  gui_btn();
  DrawButton();
  lv_ex_gauge_1();

}



/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */


/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize()
{
  pros::delay(100);
  pros::delay(20);
  lv_ex_tabview_1(); 
  pros::delay(20);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() { std::cout << "disable" << std::endl; }

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() { std::cout << "comp" << std::endl; }

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous()
{
    std::cout << "auto" << std::endl;
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */


void opcontrol()
{

#if USE_PROS_LVGL_SIM == 1
    while (ncrapi::NcrLvglSimKernel::isSTop)
#else
    while (true)
#endif
    {
          
    }
}
#if USE_PROS_LVGL_SIM == 1
int main()
{
    ncrapi::NcrLvglSimKernel *prosLvglSim = ncrapi::NcrLvglSimKernel::initNcrLvglSimKernel(&initialize, &autonomous, &opcontrol, &competition_initialize, &disabled);
    prosLvglSim->mainLoop();
}
#endif