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

mutex mtx; 
string weather_[4] = {"sunny", "rainy", "cloudy", "windy"}; 
string season_[4] = {"spring", "summer", "autumn", "winter"}; 
map<string, int>sseason_map = {{"spring", 0}, {"summer", 1}, {"autumn", 2}, {"winter", 3}}; 

int random_i(int start, int end)
{
    int sd = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine gene(sd);
    uniform_int_distribution<int> int_random(start, end);
    return int_random(gene);
}

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

void plant::reducenutritionbytime()
{
    nutrition -= random_d(0, 1);
}

string plant::get_name() 
{
    return name;
}

int plant::get_age()
{
    return age;
}

void plant::update_age()
{
    age++;
}

int plant::get_lifespan() 
{
    return lifespan;
}

double plant::get_waterlevel() 
{
    return waterlevel;
}

double plant::get_nutrition() 
{
    return nutrition;
}

string plant::get_currentweather() 
{
    return current_weather;
}

bool plant::check_alive() 
{   
    unique_lock<mutex> lock(mtx);
    if (waterlevel <= 0) return false;
    if (nutrition <= 0) return false;
    if (age == lifespan) return false;
    lock.unlock();
    return true;
}

void plant::change_state_of_deleted() 
{
    deleted = true;
}

bool plant::get_state_of_deleted() 
{
    return deleted;
}

void plant::change_state_of_deleteforever() 
{
    delete_forever = true;
}

bool plant::get_state_of_deleteforever() 
{
    return delete_forever;
}   

string flower::current_season = season_[random_i(0, 3)]; 

void flower::add_plant(string name_, int age_, int lifespan_) 
{
    name = change_to_lower(name_); 
    age = age_; 
    lifespan = lifespan_; 

    cout << "[+] Enter the season that flower bloom. You have 4 options: ";
    cout << "\n    1: Spring"; 
    cout << "\n    2: Summer";
    cout << "\n    3: Autumn";
    cout << "\n    4: Winter";
    cout << "\n    Press enter to skip";
    int take_option_from_user; 
    cout << "\n====> Enter your choice (number) or press enter to skip: ";

    while (true) {
        string input_from_user;
        getline(cin, input_from_user); 

        if (input_from_user.empty()) { 
            blooming_season = ""; 
            break;
        }

        bool check = check_number(input_from_user); 
        if (check) { 
            take_option_from_user = stoi(input_from_user); 
            if (take_option_from_user >= 1 && take_option_from_user <= 4) {
                blooming_season = season_[take_option_from_user - 1];
                break; 
            } 
        }
        cout << "[-] Invalid input. Please enter a number between 1 and 4 or press enter to skip: ";
    }

    if (blooming_season == "") { 
        cout << "[-] You have not entered the season in which the flowers will bloom. The program automatically detects that the flower will bloom in ";
        blooming_season = season_[random_i(0, 3)]; 
        cout << blooming_season << '\n';
    }
    nutrition = random_d(65, 100); 
    waterlevel = random_d(65, 100);
}

bool flower::isbloom() 
{
    if (current_season != blooming_season) return false; 
    if (waterlevel < 60) return false; 
    if (nutrition < 70) return false; 
}

void flower::change_weather(string x) 
{
    current_weather = x;
}

void tree::add_plant(string name_, int age_, int lifespan_) 
{
    name = change_to_lower(name_); 
    age = age_; 
    lifespan = lifespan_; 

    cout << "====> Enter the height of tree: ";

    while (true) {
        string input_from_user;
        getline(cin, input_from_user);

        if (input_from_user.empty()) { 
            height = -1; 
            break; 
        }

        bool check = check_number(input_from_user); 
        if (check) { 
            height = stod(input_from_user);
            if (height >= 0) break;
        }
        cout << "[-] Invalid input. Please enter a valid height: ";
    }

    if (height == -1) { 
        cout << "[-] You do not enter height for tree, so system will estimate. The height of tree is ";
        height = random_d(30, 90);
        cout << height << '\n';
    }
    nutrition = random_d(65, 100); 
    waterlevel = random_d(65, 100); 
}

string flower::get_currentseason() 
{
    return current_season;
}

double tree::get_height() 
{
    return height;
}

void tree::increaseheightbytime() 
{
    height += random_d(0, 0.3);
}

int weather::time_to_change_season = 0; 

void weather::set_time(int time_) 
{
    time_to_change_season = time_;
}

int weather::get_time()
{
    return time_to_change_season;
}

int getMapSeason = sseason_map[weather::get_currentseason()]; 
void weather::change_season()
{
    if (getMapSeason == 3) { 
        getMapSeason = 0; 
        current_season = season_[getMapSeason]; 
    } else {
        getMapSeason++; 
        current_season = season_[getMapSeason]; 
    }
}