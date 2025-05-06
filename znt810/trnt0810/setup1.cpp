#include <random>
#include <chrono>
#include <algorithm>
#include <cctype>
#include "tree.h"

using namespace std;

bool check_number(string s)
{
    for (auto c : s) {
        if (!isdigit(c)) return false;
    }
    return true;
}

/*
check_Number function is used to check if the string is a number or not. 
Loop through each character in the string and check if it is a digit. 
If any character is not a digit, return false. Otherwise, return true.
*/

mutex mtx; // declare mutex for thread safety
string weather_[4] = {"sunny", "rainy", "cloudy", "windy"}; // an array of weather types
string season_[4] = {"spring", "summer", "autumn", "winter"}; // an array of the four seasons
map<string, int>sseason_map = {{"spring", 0}, {"summer", 1}, {"autumn", 2}, {"winter", 3}}; // a map to convert season names to their corresponding indices

int random_i(int start, int end)
{
    int sd = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine gene(sd);
    uniform_int_distribution<int> int_random(start, end);
    return int_random(gene);
}

/*
random_i function is used to generate an integer in range [start, end]. 
Get current time since epoch (chrono::system_clock::now().time_since_epoch().count()).
Initialize the random number generator gene with seed sd (default_random_engine gene(sd)).
Then, create a uniform integer distribution int_random(start, end) and return the generated random number, uniform_int_distribution is a 
uniform distribution in statistical probability, ensuring that each integer value in the specified interval has the same probability of occurring.
*/

double random_d(double start, double end) // same as random_i but for double
{
    unsigned int sd = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine gene(sd);
    uniform_real_distribution<double> double_random(start, end);
    return double_random(gene);
}

string change_to_lower(string str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
} 

/*
change_to_lower function is used to convert a string to lowercase.
Use the transform function from the algorithm library to apply the ::tolower function to each character in the string.
The result is stored back in the original string str.
*/

void plant::feed()
{   
    unique_lock<mutex> lock(mtx);
    if (nutrition >= 85) {
        cout << "\n[-] Current nutrition is greater than 85, no need to feed\n";
        lock.unlock();
        return;
    } else {
        while (true) {
            double rd = random_d(1, 100);
            if (nutrition + rd <= 100) {
                nutrition += rd; break;
            } else continue;
        }
    }
    lock.unlock();
    cout << "\n[+] Sucessful\n";
}

/*
feed method is used to feed the plant (nutrition, fertilizer). 
The mutex is locked immediately after acquiring the mutex to avoid conflicts with other threads when they access the same  
variable (a common resource), causing a race condition. There will be a thread that does the continuous nutrition reduction 
over time in the setup3.cpp file and it is put into a separate thread to run independently from the main thread. After the work is done, 
the mutex is unlocked so that other threads can acquire the mutex (mtx).
The feed method will check the nutrition variable to see if it is greater than 85. If it is greater than 85, it will release the mutex and return. 
If it is not greater than or equal to 85, it will randomly generate a number so that the nutrition variable plus the random number does not exceed 100 
(100 is the maximum). If it is greater than 100, it will randomize again.
*/

string plant::current_weather = weather_[random_i(0, 3)]; // Every time the program is started, it will automatically randomly generate one of the four weather types

void plant::water()
{   
    unique_lock<mutex> lock(mtx);
    if (current_weather == "rainy") {
        cout << "\n[-] Current weather is rainy, do not need to water\n";
        lock.unlock(); 
        return;
    } 
    if (waterlevel >= 85) {
        cout << "\n[-] No need to water because water level now is greater than 85\n";
        lock.unlock();
        return;
    }

    while (true) {
        double randd = random_d(1, 100);
        if (waterlevel + randd > 100) continue;
        else {
            waterlevel += randd; break;
        }
    }
    lock.unlock();
    cout << "\n[+] Sucessful\n";
}

/*
water method is used to water the plant. Locking mutex has the same purpose as feed method (avoid causing race condition)
The method will check if the current_weather variable is rainy or not, if it is rainy then there is no need to water, otherwise 
it will continue to check if the waterlevel is greater than 85 or not, if it is greater than 85 then there is no need to water. Otherwise, 
it will randomly generate a real number, if the waterlevel variable plus the random real number is greater than 100 then it will be random again.
*/

void plant::reducewaterbytime()
{   
    if (current_weather == "sunny") {
        waterlevel -= random_d(1, 1.5);
        return;
    }

    if (current_weather == "rainy") {
        if (waterlevel < 85) waterlevel += random_d(1, 1.5);
        else return;
    }
    waterlevel -= random_d(0, 1);
}

/*
reducewaterbytime method is used to reduce the water level of the plant over time (1 second).
it checks if the weather is sunny or not, if it is sunny then it will lose more water than usual, 
randomly output a number between 1 and 1.5 and subtract it from the warerlevel variable, but if the weather 
is rainy then it will add a random amount of water between 1 and 1.5. The other 2 types of weather will each be 
subtracted a random amount between 0 and 1 every 1 second
*/

void plant::reducenutritionbytime() // The reducenutritionbytime method is used to reduce the nutrition variable by any amount between 0 and 1 every 1 second
{
    nutrition -= random_d(0, 1);
}

string plant::get_name() // return the name of the plant
{
    return name;
}

int plant::get_age() // return the age of the plant
{
    return age;
}

void plant::update_age() // update the age of the plant by 1 second
{
    age++;
}

int plant::get_lifespan() // return the lifespan of the plant
{
    return lifespan;
}

double plant::get_waterlevel() // return the water level of the plant (waterlevel variable)
{
    return waterlevel;
}

double plant::get_nutrition() // return the nutrition level of the plant (nutrition variable)
{
    return nutrition;
}

string plant::get_currentweather() // return the current weather (current_weather variable)
{
    return current_weather;
}

bool plant::check_alive() // check if the plant is alive (water level, nutrition level, age must meet the conditions)
{   
    unique_lock<mutex> lock(mtx);
    if (waterlevel <= 0) return false;
    if (nutrition <= 0) return false;
    if (age == lifespan) return false;
    lock.unlock();
    return true;
}

void plant::change_state_of_deleted() // change state of deleted variable to true
{
    deleted = true;
}

bool plant::get_state_of_deleted() // get value of deleted variable
{
    return deleted;
}

void plant::change_state_of_deleteforever() // change state of delete_forever variable to true
{
    delete_forever = true;
}

bool plant::get_state_of_deleteforever() // get value of delete_forever variable
{
    return delete_forever;
}   

string flower::current_season = season_[random_i(0, 3)]; // Every time the program is started, it will automatically randomly generate one of the four seasons

void flower::add_plant(string name_, int age_, int lifespan_) // add_plant method is used to add a flower to the current flower list)
{
    name = change_to_lower(name_); // get from user input and convert to lowercase (This is in the setup3.cpp file)
    age = age_; // get from user input (This is in the setup3.cpp file)
    lifespan = lifespan_; // get from user input (This is in the setup3.cpp file)

    // Next, the user will be asked to enter the season in which this flower will bloom. If the user does not know, the system will automatically give it (by random)
    cout << "[+] Enter the season that flower bloom. You have 4 options: ";
    cout << "\n    1: Spring"; 
    cout << "\n    2: Summer";
    cout << "\n    3: Autumn";
    cout << "\n    4: Winter";
    cout << "\n    Press enter to skip";
    int take_option_from_user; // the option that the user chooses (1, 2, 3, or 4)
    cout << "\n====> Enter your choice (number) or press enter to skip: ";

    while (true) {
        string input_from_user;
        getline(cin, input_from_user); 

        if (input_from_user.empty()) { // if the user does not enter anything (press enter), the program will automatically assign a random season to the flower
            blooming_season = ""; // set blooming_season to empty string
            break;
        }

        bool check = check_number(input_from_user); // check if the input is a number or not
        if (check) { // if the input is a number, convert it to an integer and check if it is in the range [1, 4]
            take_option_from_user = stoi(input_from_user); 
            if (take_option_from_user >= 1 && take_option_from_user <= 4) {
                blooming_season = season_[take_option_from_user - 1];
                break; // if the input is in the range [1, 4], break the loop
            } 
        }
        cout << "[-] Invalid input. Please enter a number between 1 and 4 or press enter to skip: "; // if the input is invalid, ask the user to enter again
    }

    if (blooming_season == "") { // if the user does not enter anything (press enter), the program will automatically assign a random season to the flower
        cout << "[-] You have not entered the season in which the flowers will bloom. The program automatically detects that the flower will bloom in ";
        blooming_season = season_[random_i(0, 3)]; // assign a random season to the flower
        cout << blooming_season << '\n';
    }
    nutrition = random_d(65, 100); // Let's assume that the system automatically measures, assign a random nutrition level to the flower (between 65 and 100)
    waterlevel = random_d(65, 100); // Let's assume that the system automatically measures, assign a random water level to the flower (between 65 and 100)
}

bool flower::isbloom() // check if the flower bloomed or not
{
    if (current_season != blooming_season) return false; // if the current season is not the blooming season, return false
    if (waterlevel < 60) return false; // if current season is blooming season but the water level is less than 60, return false
    if (nutrition < 70) return false; // if current season is blooming season and water level is greater than 60 but the nutrition level is less than 70, return false
    return true; // if all conditions are met, return true
}

void flower::change_weather(string x) // change weather each 60 seconds (1 min)
{
    current_weather = x;
}

void tree::add_plant(string name_, int age_, int lifespan_) // add_plant method is used to add a tree to the current tree list
{
    name = change_to_lower(name_); // get from user input and convert to lowercase (This is in the setup3.cpp file)
    age = age_; // get from user input (This is in the setup3.cpp file)
    lifespan = lifespan_; // get from user input (This is in the setup3.cpp file)

    cout << "====> Enter the height of tree: ";

    while (true) {
        string input_from_user;
        getline(cin, input_from_user);

        if (input_from_user.empty()) { // if the user does not enter anything (press enter), the program will automatically assign a random height to the tree
            height = -1; // set height to -1 if user presses enter
            break; 
        }

        bool check = check_number(input_from_user); // check if the input is a number or not
        if (check) { // if the input is a number, convert it to a double and check if it is greater than or equal to 0
            height = stod(input_from_user);
            if (height >= 0) break;
        }
        cout << "[-] Invalid input. Please enter a valid height: "; // if the input is invalid, ask the user to enter again
    }

    if (height == -1) { // if the user does not enter anything (press enter), the program will automatically assign a random height to the tree
        cout << "[-] You do not enter height for tree, so system will estimate. The height of tree is ";
        height = random_d(30, 90);
        cout << height << '\n';
    }
    nutrition = random_d(65, 100); // Let's assume that the system automatically measures, assign a random nutrition level to the tree (between 65 and 100)
    waterlevel = random_d(65, 100); // Let's assume that the system automatically measures, assign a random water level to the tree (between 65 and 100)
}

string flower::get_currentseason() // return the current season (current_season variable)
{
    return current_season;
}

double tree::get_height() // return the height of the tree (height variable)
{
    return height;
}

void tree::increaseheightbytime() // increase height of the tree by time (1 second)
{
    height += random_d(0, 0.3); // increase height by a random amount between 0.01 and 0.3 every second
}

int weather::time_to_change_season = 0; // set a random value (garbage value)

void weather::set_time(int time_) // set time to change season
{
    time_to_change_season = time_;
}

int weather::get_time() // get time to change season
{
    return time_to_change_season;
}

int getMapSeason = sseason_map[weather::get_currentseason()]; // mapping the current season to its corresponding index (0, 1, 2, or 3)
void weather::change_season()
{
    if (getMapSeason == 3) { // if the current season is winter, set it to spring
        getMapSeason = 0; // reset to spring
        current_season = season_[getMapSeason]; // set current season to spring
    } else { // otherwise, increment the season index
        getMapSeason++; // increment the season index
        current_season = season_[getMapSeason]; // set current season to the next season
    }
}

/*
change_season method is used to change the current season
The goal of this function is to change the seasons over time,
and in the order of spring, summer, fall, winter and then spring again.
*/