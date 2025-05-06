#ifndef DISPLAY_H
#define DISPLAY_H
#include "tree.h"
#include "manage.h"

void increase_time(); // function to increase time by 1 second

void update_weather(); // function to update weather every 1 minute (60 seconds)

void update_season(); // function to update season every 2 minutes (120 seconds)

void check_alive_by_time(); // function to check if the plant is alive every 1 second

void update_WaterNutrition(flower*); // function to update water and nutrition level of flower every 1 second

extern bool stop_all_th; // global variable to stop all threads

class display // class to display the main menu and options
{
    public:
        void show_scr(); // function to show the main menu and options
};

#endif