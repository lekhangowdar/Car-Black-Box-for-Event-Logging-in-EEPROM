#ifndef MAIN
#define MAIN

#define DASHBOARD               0
#define PASSWORD                1
#define MENU                    2
#define MENU_ENTER              3
#define VIEWLOG                 0
#define DOWNLOADLOG             1
#define CLEARLOG                2
#define SETTIME                 3
#define CHANGEPASS              4

void dashboard();               
void store_event();
void password(char key);
void menu(char key);
void view_log(char key);
void download_log();
void clear_log(char key);
void set_time(char key);
void change_pass(char key);



#endif