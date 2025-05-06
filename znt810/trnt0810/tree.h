#ifndef TREE_H
#define TREE_H
#include <iostream>
#include <string.h>
#include <map>
#include <mutex>

using namespace std;

extern mutex mtx; // mutex for thread safety
bool check_number(string); // check if the string is a number
int random_i(int start, int end); // generate a random integer in the range [start, end]
double random_d(double start, double end); // generate a random double in the range [start, end]

extern string weather_[4]; // weather options
extern string season_[4]; // season options
extern map<string, int>sseason_map; // map for season to int conversion

class plant // base class for flower and tree
{
    protected:
        string name; // name of the plant
        int age; // age of the plant
        int lifespan; // lifespan of the plant
        double waterlevel; // water level of the plant
        double nutrition; // nutrition level of the plant
        static string current_weather; // current weather
        bool deleted = false; // state of the plant (deleted from current flower/tree list or not, this variable is used to force the thread in setup3.cpp to stop)
        bool delete_forever = false; // This variable is used to mark that the tree should be permanently removed from the list 
    public:
        virtual void add_plant(string, int, int) = 0; // add plant or flower to current plant/flower list
        void feed(); // feed the plant (nutrition, fertilizer)
        void water(); // water the plant
        void reducewaterbytime(); // reduce water level by time (1 second)
        void reducenutritionbytime(); // reduce nutrition level by time (1 second)
        string get_name(); // get name of the plant 
        int get_age(); // get age of the plant
        void update_age();  // update age of the plant (1 second)
        int get_lifespan(); // get lifespan of the plant
        double get_waterlevel(); // get water level of the plant
        double get_nutrition(); // get nutrition level of the plant
        static string get_currentweather(); // get current weather
        bool check_alive(); // check if the plant is alive (water level, nutrition level, age must meet the conditions)
        void change_state_of_deleted(); // change state of deleted variable to true
        bool get_state_of_deleted(); // get value of deleted variable
        void change_state_of_deleteforever(); // change state of delete_forever variable to true
        bool get_state_of_deleteforever(); // get value of delete_forever variable
};

class flower : public plant
{
    protected:
        static string current_season; // current season
        string blooming_season; // season in which the flower will bloom
    public:
        void add_plant(string, int, int) override; // override add_plant function
        bool isbloom(); // check if the flower bloomed or not
        static void change_weather(string); // change weather
        static string get_currentseason(); // get current season
};

class tree: public plant
{
    protected:
        double height = 0; // height of the tree

    public:
        void add_plant(string, int, int) override; // override add_plant function
        void increaseheightbytime(); // increase height of the tree by time (1 second)
        double get_height(); // get height of the tree
};

class weather : public flower 
{
    protected:
        static int time_to_change_season; // time to change season
    public:
        static void change_season(); // change season
        static int get_time(); // get time to change season
        static void set_time(int); // set time to change season
};

#endif