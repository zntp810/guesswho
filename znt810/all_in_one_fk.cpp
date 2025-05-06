#include <iostream>
#include <string.h>
#include <map>
#include <mutex>
#include <random>
#include <chrono>
#include <algorithm>
#include <cctype>
#include <thread>
#include <iomanip>

using namespace std;

class plant
{
    protected:
        string name; 
        int age;
        int lifespan;
        double waterlevel;
        double nutrition;
        static string current_weather;
        bool deleted = false;
        bool delete_forever = false;
    public:
        virtual void add_plant(string, int, int) = 0;
        void feed();
        void water();
        void reducewaterbytime();
        void reducenutritionbytime();
        string get_name();
        int get_age();
        void update_age(); 
        int get_lifespan();
        double get_waterlevel();
        double get_nutrition();
        static string get_currentweather();
        bool check_alive();
        void change_state_of_deleted();
        bool get_state_of_deleted();
        void change_state_of_deleteforever();
        bool get_state_of_deleteforever();
};

class flower : public plant
{
    protected:
        static string current_season; 
        string blooming_season;
        bool bloom = false;
        int options[4] = {1,2,3,4};
    public:
        void add_plant(string, int, int) override;
        bool isbloom();
        static void change_weather(string);
        static string get_currentseason();
};

class tree: public plant
{
    protected:
        double height = 0;

    public:
        void add_plant(string, int, int) override;
        void increaseheightbytime();
        double get_height() const;
};

class weather : public flower 
{
    protected:
        static int time_to_change_season;
    public:
        static void change_season();
        static int get_time();
        static void set_time(int);
};

void drawTable(vector<vector<string>>);

class user
{
    private:
        vector<flower*> current_list_flower;
        vector<tree*> current_list_tree;
        vector<flower*> died_flower;
        vector<tree*> died_tree;
    public:
        int count_flower = 0, count_tree = 0;
        void show_plant_list(char);
        void show_died_plant_list(char);
        void addtoFlowerList(flower*);
        void addtoTreeList(tree*);
        void removediedplant();
        void removeoneplant(int, char);
        void clear_list(char);
        void sortby(string);
        flower *take_flower(int);
        tree *take_tree(int);
        bool check_flower_list();
        bool check_tree_list();
        bool check_died_flowerlist();
        bool check_died_treelist();
        void clear_died_list(char);
};

void increase_time();

void update_weather();

void update_season();

void check_alive_by_time();

void update_WaterNutrition(flower*);

extern bool stop_all_th;

class display
{
    public:
        void show_scr();
};

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

double random_d(double start, double end) 
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

string plant::current_weather = weather_[random_i(0, 3)];

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
    cout << "\n====> Enter your choice (number): ";

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
    return true;   
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

double tree::get_height() const 
{
    return height;
}

void tree::increaseheightbytime() 
{
    double rand = random_d(0.01, 0.2);
    if (height + rand > 100) return;
    else height += rand;
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

void drawTable(vector<vector<string>> table)
{
    if (table.empty()) return;

    size_t cot = 0;
    for (vector row : table) {
        cot = max(cot, row.size());
    }

    vector<size_t> colWidths(cot, 0);
    for (auto row : table) {
        for (int i = 0; i < cot; ++i) {
            if (i < row.size()) colWidths[i] = max(colWidths[i], row[i].length());
            
        }
    }

    auto printSeparator = [&]() {
        cout << '+';
        for (size_t w : colWidths) cout << string(w + 2, '-') << '+';
        cout << '\n';
    };

    for (auto row : table) {
        printSeparator();
        for (int i = 0; i < cot; ++i) {
            string cell = (i < row.size() ? row[i] : "");
            cout << "| " << left << setw(colWidths[i]) << cell << " ";
        }
        cout << "|\n";
    }
    printSeparator();
}

void user::addtoFlowerList(flower *x) 
{
    current_list_flower.push_back(x);
}

void user::addtoTreeList(tree *x)
{
    current_list_tree.push_back(x);
}

void user::removediedplant()
{
        for (auto i = current_list_flower.begin(); i != current_list_flower.end(); ) {
            if ((*i)->check_alive() == false) {
                (*i)->change_state_of_deleted();
                died_flower.push_back(*i);
                i = current_list_flower.erase(i);
            } else ++i;
        }
 
        for (auto i = current_list_tree.begin(); i != current_list_tree.end(); ) {
            if((*i)->check_alive() == false) {
                (*i)->change_state_of_deleted();
                died_tree.push_back(*i);
                i = current_list_tree.erase(i);
            } else ++i;
        }
}

void user::removeoneplant(int index, char x) 
{
    if (x == 'f') {
        flower* take = current_list_flower[index];
        current_list_flower.erase(current_list_flower.begin() + index);
        take->change_state_of_deleted();
        take->change_state_of_deleteforever();
    } else {
        tree* take = current_list_tree[index];
        current_list_tree.erase(current_list_tree.begin() + index);
        take->change_state_of_deleted();
        take->change_state_of_deleteforever();
    }
}

void user::show_plant_list(char x)
{
    
    vector<vector<string>> that_need_print;
    vector<string> print;
    string tmp;
    if (x == 'f') tmp = "Flower"; 
    else tmp = "Tree";
    print.push_back("ON");
    print.push_back(tmp + " Name");
    print.push_back(tmp + " Age");
    print.push_back(tmp + " Lifespan");
    print.push_back("Water Level");
    print.push_back("Nutrition");
    if (x != 'f') print.push_back("Height");
    print.push_back("Status");
    that_need_print.push_back(print);
    print.clear();

    if (x == 'f') {
    for (int i = 0; i < current_list_flower.size(); i++) {
        print.push_back(to_string(i+1));
        print.push_back(current_list_flower[i]->get_name());
        print.push_back(to_string(current_list_flower[i]->get_age()));
        print.push_back(to_string(current_list_flower[i]->get_lifespan()));
        print.push_back(to_string(current_list_flower[i]->get_waterlevel()));
        print.push_back(to_string(current_list_flower[i]->get_nutrition()));
        if (current_list_flower[i]->isbloom()) print.push_back("Bloomed");
        else print.push_back("not bloom");
        that_need_print.push_back(print);
        print.clear();
        cout << "\n##### TABLE OF FLOWER #####";
    }
} else {
    for (int i = 0; i < current_list_tree.size(); i++) {
        print.push_back(to_string(i+1));
        print.push_back(current_list_tree[i]->get_name());
        print.push_back(to_string(current_list_tree[i]->get_age()));
        print.push_back(to_string(current_list_tree[i]->get_lifespan()));
        print.push_back(to_string(current_list_tree[i]->get_waterlevel()));
        print.push_back(to_string(current_list_tree[i]->get_nutrition()));
        print.push_back(to_string(current_list_tree[i]->get_height()));
        if (current_list_tree[i]->check_alive()) print.push_back("Alive");
        else print.push_back("Died");
        that_need_print.push_back(print);
        print.clear();
        cout << "\n##### TABLE OF TREE #####";
    }
}
    cout << "\n[*] Current weather: " << plant::get_currentweather() << "\n";
    cout << "[*] Current seasson: " << flower::get_currentseason() << "\n";
    drawTable(that_need_print);
    that_need_print.clear();
}

void user::show_died_plant_list(char x)
{
    vector<vector<string>> that_need_print;
    vector<string> print;
    string tmp;
    if (x == 'f') tmp = "Flower"; 
    else tmp = "Tree";
    print.push_back("ON");
    print.push_back(tmp + " Name");
    print.push_back(tmp + " Age");
    print.push_back(tmp + " Lifespan");
    print.push_back("Water Level before die");
    print.push_back("Nutrition before die");
    that_need_print.push_back(print);
    print.clear();

    if (x == 'f') {
    for (int i = 0; i < died_flower.size(); i++) {
        print.push_back(to_string(i+1));
        print.push_back(died_flower[i]->get_name());
        print.push_back(to_string(died_flower[i]->get_age()));
        print.push_back(to_string(died_flower[i]->get_lifespan()));
        print.push_back(to_string(died_flower[i]->get_waterlevel()));
        print.push_back(to_string(died_flower[i]->get_nutrition()));
        that_need_print.push_back(print);
        print.clear();
    }
} else {
    for (int i = 0; i < died_tree.size(); i++) {
        print.push_back(to_string(i+1));
        print.push_back(died_tree[i]->get_name());
        print.push_back(to_string(died_tree[i]->get_age()));
        print.push_back(to_string(died_tree[i]->get_lifespan()));
        print.push_back(to_string(died_tree[i]->get_waterlevel()));
        print.push_back(to_string(died_tree[i]->get_nutrition()));
        that_need_print.push_back(print);
        print.clear();
    }
}
    drawTable(that_need_print);
    that_need_print.clear();
}

bool compare_fname (flower *a, flower *b) {return a->get_name() < b->get_name();}
bool compare_fage (flower *a, flower *b) {return a->get_age() > b->get_age();}
bool compare_tname (tree *a, tree *b) {return a->get_name() < b->get_name();}
bool compare_tage (tree *a, tree *b) {return a->get_age() > b->get_age();}
bool compare_hage (tree *a, tree *b) {return a->get_height() > b->get_height();}

void user::sortby(string x) 
{
    if (x == "fn") sort(current_list_flower.begin(), current_list_flower.end(), compare_fname);
    if (x == "fa") sort(current_list_flower.begin(), current_list_flower.end(), compare_fage);
    if (x == "tn") sort(current_list_tree.begin(), current_list_tree.end(), compare_tname);
    if (x == "ta") sort(current_list_tree.begin(), current_list_tree.end(), compare_tage);
    if (x == "th") sort(current_list_tree.begin(), current_list_tree.end(), compare_hage);
}

void user::clear_list(char x) 
{
    if (x == 'f') {
        for (auto each : current_list_flower) {
            each->change_state_of_deleted();
            each->change_state_of_deleteforever();
        }
        current_list_flower.clear();
    } else {
        for (auto each : current_list_tree) {
            each->change_state_of_deleted();
            each->change_state_of_deleteforever();
        }
        current_list_tree.clear();
    }
}

flower *user::take_flower(int index)
{
    return current_list_flower[index];
}

tree *user::take_tree(int index)
{
    return current_list_tree[index];
}

bool user::check_flower_list()
{
    return current_list_flower.empty();
}

bool user::check_tree_list()
{
    return current_list_tree.empty();
}

bool user::check_died_flowerlist() 
{
    return died_flower.empty();
}

bool user::check_died_treelist()
{
    return died_tree.empty();
}

void user::clear_died_list(char x) 
{
    if (x == 'f') {
        for (auto each : died_flower) delete each;
        died_flower.clear();
    } else {
        for (auto each : died_tree) delete each;
        died_tree.clear();
    }
}

bool stop_all_th = false;
int day_by_day = 0;
user xxx;

void increase_time()
{

    while (!stop_all_th) {
        unique_lock<mutex> lock(mtx);
        day_by_day++;
        lock.unlock();
        this_thread::sleep_for(chrono::seconds(1));
    }
    cout << "*";
    
}

void update_weather()
{   
    while (!stop_all_th) {
        unique_lock<mutex> lock(mtx);
        int current_dbd = day_by_day;
        lock.unlock();
        if (current_dbd % 60 == 0) {
            flower::change_weather(weather_[random_i(0, 3)]);
        }
    }
    cout << "*";
}

void update_season() 
{   
    while (!stop_all_th) {
        unique_lock<mutex> lock(mtx);
        int current_dbd = day_by_day;
        lock.unlock();
        if (current_dbd % (weather::get_time()*60) == 0) {
            weather::change_season();
            this_thread::sleep_for(chrono::seconds(2));
        }
    }
    cout << "*";
}

void update_FlowerWaterNutritionAge(flower* x)
{   
    int live = 0;
    while (!x->get_state_of_deleted()) {
        unique_lock<mutex> lock(mtx);
        x->reducewaterbytime();
        x->reducenutritionbytime();
        if (++live % 60 == 0) {
            x->update_age();
        }
        lock.unlock();
        this_thread::sleep_for(chrono::seconds(1));
    }
    if (x->get_state_of_deleteforever()) delete x;
}

void update_TreeWaterNutritionAge(tree* x)
{   
    int live = 0;
    while (!x->get_state_of_deleted()) {
        unique_lock<mutex> lock(mtx);
        x->reducewaterbytime();
        x->reducenutritionbytime();
        x->increaseheightbytime();
        if (++live % 60 == 0) {
            x->update_age();
        }
        lock.unlock();
        this_thread::sleep_for(chrono::seconds(1));
    }
    if (x->get_state_of_deleteforever()) delete x;
}

void check_alive_by_time() 
{   
    while (!stop_all_th) {
        xxx.removediedplant();
        this_thread::sleep_for(chrono::seconds(1));
    }
    cout << "*";
}

bool checkinputfromuser(string input, int start = -1, int end = -1) 
{
    bool check = true;
    for (auto each : input) { if (!isdigit(each)) { return false;} }
    if (start != -1 && end != -1) { if (!(stoi(input) >= start && stoi(input) <= end)) check = false; }
    if (start != -1) if (!(stoi(input) >= start)) check = false;
    return check;
}

void show_options() 
{
    vector<vector<string>> options;
    vector<string> option;
    option.push_back("1"); option.push_back("Add a flower"); options.push_back(option); option.clear();
    option.push_back("2"); option.push_back("Add a tree"); options.push_back(option); option.clear();  
    option.push_back("3"); option.push_back("Show flower list"); options.push_back(option); option.clear();
    option.push_back("4"); option.push_back("Show tree list"); options.push_back(option); option.clear();
    option.push_back("5"); option.push_back("Show flower list that died"); options.push_back(option); option.clear();
    option.push_back("6"); option.push_back("Show tree list that died"); options.push_back(option); option.clear();
    option.push_back("7"); option.push_back("Delete flower list"); options.push_back(option); option.clear();
    option.push_back("8"); option.push_back("Delete tree list"); options.push_back(option); option.clear();
    option.push_back("9"); option.push_back("Delete died flower list"); options.push_back(option); option.clear();
    option.push_back("10"); option.push_back("Delete died tree list"); options.push_back(option); option.clear();
    option.push_back("11"); option.push_back("Exit the program"); options.push_back(option); option.clear();
    cout <<"### MAIN OPTIONS ###\n";
    drawTable(options);
}

void additional_opt(char x, user &xx) 
{
    vector<vector<string>> options;
    vector<string> option;
    option.push_back("1"); option.push_back("Show table that sort by name (default)"); options.push_back(option); option.clear();
    option.push_back("2"); option.push_back("Show table that sort by age"); options.push_back(option); option.clear();
    option.push_back("3"); option.push_back("Water a plant"); options.push_back(option); option.clear();
    option.push_back("4"); option.push_back("Feed a plant"); options.push_back(option); option.clear();
    option.push_back("5"); option.push_back("Show this table again"); options.push_back(option); option.clear();
    option.push_back("6"); option.push_back("Delete a plant"); options.push_back(option); option.clear();

    if (x == 't') {
        option.push_back("7"); option.push_back("Show table sort by height"); options.push_back(option); option.clear();
    }
    string get_aopt;
    while (true) {
        cout << "\n###You can select one of the following options or press enter to exit###\n";
        drawTable(options);
        cout << "====> Enter your choice: ";
        getline(cin, get_aopt);
        if (get_aopt.empty()) { cout << "\n[-] Back to main Menu\n\n"; this_thread::sleep_for(chrono::seconds(1)); return; }
        if (x == 't') {
            if (!checkinputfromuser(get_aopt, 1, 8)) {cout << "\n[-] Error input, try again\n"; this_thread::sleep_for(chrono::seconds(1)); continue; } }
        else { if (!checkinputfromuser(get_aopt, 1, 6)) {cout << "\n[-] Error input, try again\n"; this_thread::sleep_for(chrono::seconds(1)); continue; } }
        int get_aopt_i = stoi(get_aopt);
        if ((x != 't' && xx.check_flower_list()) || (x == 't' && xx.check_tree_list())) {
            cout << "\n[-] List now is empty, back to main menu\n\n";
            this_thread::sleep_for(chrono::seconds(1)); return;
        }
        if (get_aopt_i == 1) { 
            if (x == 't') xx.sortby("tn"); else xx.sortby("fn"); 
            cout << "\n[*] Successfully sorted by name\n\n"; this_thread::sleep_for(chrono::seconds(1)); continue;
        }
        if (get_aopt_i == 2) { 
            if (x == 't') xx.sortby("ta"); else xx.sortby("fa");
            cout << "\n[*] Sucessfully sorted by age\n\n"; this_thread::sleep_for(chrono::seconds(1)); continue;
        }
        if (get_aopt_i == 3 || get_aopt_i == 4 || get_aopt_i == 6 || get_aopt_i == 8) {
            if (get_aopt_i == 3) cout << "====> Choose a plant to water (1 - "; 
            else if (get_aopt_i == 4) cout << "====> Choose a plant to feed (1 - ";
            else if (get_aopt_i == 6) cout << "====> Choose a plant to delete (1 - ";
            if (x != 't') cout << xx.count_flower; else cout << xx.count_tree; cout << ")" << " or press enter to back: "; 
            string water_feed_delete;

            while (true) {
                getline(cin, water_feed_delete);
                if (water_feed_delete.empty()) break;
                if (x != 't') {
                    if (checkinputfromuser(water_feed_delete, 1, xx.count_flower)) break;
                    else { cout << "[-] Invalid input, try again, enter a valid input in range from 1 to " << xx.count_flower << " or press enter to back: "; continue; }
                } else {
                    if (checkinputfromuser(water_feed_delete, 1, xx.count_tree)) break;
                    else { cout << "[-] Invalid input, try again, enter a valid input in range from 1 to " << xx.count_tree << " or press enter to back: "; continue; }
                }
            }
            if (water_feed_delete.empty()) continue;
            if (get_aopt_i == 3) {
                if (x != 't') {
                    flower *tmp = xx.take_flower(stoi(water_feed_delete) - 1);
                    tmp->water();
                    this_thread::sleep_for(chrono::seconds(1));
                } else {
                    tree *tmp = xx.take_tree(stoi(water_feed_delete) - 1);
                    tmp->water();
                    this_thread::sleep_for(chrono::seconds(1));
                }
            } else if (get_aopt_i == 4) {
                if (x != 't') {
                    flower *tmp = xx.take_flower(stoi(water_feed_delete) - 1);
                    tmp->feed();
                    this_thread::sleep_for(chrono::seconds(1));
                } else {
                    tree *tmp = xx.take_tree(stoi(water_feed_delete) - 1);
                    tmp->feed();
                    this_thread::sleep_for(chrono::seconds(1));
                }
            } else if (get_aopt_i == 6) {
                if (x != 't') {
                    xx.removeoneplant(stoi(water_feed_delete) - 1, 'f');
                    xx.count_flower--;
                } else {
                    xx.removeoneplant(stoi(water_feed_delete) - 1, 't');
                    xx.count_tree--; }
                cout << "\n[+] Delete successfully\n\n";
                this_thread::sleep_for(chrono::seconds(1));
                if (x != 't') {
                    if (xx.check_flower_list()) {
                        cout << "[*] The flower list now is empty, back to the main menu\n\n";
                        this_thread::sleep_for(chrono::seconds(1));
                        break;
                    }
                } else {
                    if (xx.check_tree_list()) {
                        cout << "[*] The tree list now is empty, back to the main menu\n\n";
                        this_thread::sleep_for(chrono::seconds(1));
                        break;
                    }
                }
            }
            else {
                tree *tmp = xx.take_tree(stoi(water_feed_delete) - 1);
                this_thread::sleep_for(chrono::seconds(1));
            }
            continue;
        }

        if (get_aopt_i == 5) {
            if (x != 't') xx.show_plant_list('f');
            else xx.show_plant_list('t');
            this_thread::sleep_for(chrono::seconds(1));
            continue;
        }

        if (get_aopt_i == 7) {
            cout << "\n[*] Sucessfully sort by height\n";
            this_thread::sleep_for(chrono::seconds(1)); 
        }
    }
}

void display::show_scr() 
{   
    string takeoptionfromuser;
    while (true) {
        show_options();
        cout << "[*] Enter your option (1-11): ";
        while (true) {
            getline(cin, takeoptionfromuser);
            if (takeoptionfromuser.empty()) {
                cout << "[*] Enter your option (1-11): ";
                continue;
            }
            if (checkinputfromuser(takeoptionfromuser, 1, 11)) break;
            else { cout << "[-] Error, please try again, enter a number in range 1 - 11: "; continue;}
        }

        int option = stoi(takeoptionfromuser);

        if (option == 1) {
            flower *tmp = new flower;
            string name = "", age, lifespan;
            cout << "====> Enter name of flower or press enter to back to main menu: "; 
            while (true) {
                getline(cin, name);
                if (!name.empty()) break;
                else break;
            }
            if (name.empty()) {cout << "\n[-] Back to main menu\n\n"; this_thread::sleep_for(chrono::seconds(1)); continue;}
            cout << "====> Enter current age of flower or press enter to back to main menu: ";
            while (true) {
                getline(cin, age);
                if (age.empty()) break;
                if (checkinputfromuser(age, 1)) break;
                else {cout << "[-] Error, please try again, enter a valid age (must greater than 0) or press enter to back to main menu: "; continue;}
            }
            if (age.empty()) {cout << "\n[-] Back to main menu\n\n"; this_thread::sleep_for(chrono::seconds(1)); continue;}
            cout << "====> Enter lifespan of flower or press enter to back to main menu: ";
            while (true) {
                getline(cin, lifespan);
                if (lifespan.empty()) break;
                if (checkinputfromuser(lifespan, stoi(age) + 1)) break;
                else { cout << "[-] Error, please try again, enter a valid lifespan (must be greater than the age) or press enter to back to main menu: "; continue;}
            }
            if (lifespan.empty()) {cout << "\n[-] Back to main menu\n\n"; this_thread::sleep_for(chrono::seconds(1)); continue;}
            tmp->add_plant(name, stoi(age), stoi(lifespan));
            xxx.addtoFlowerList(tmp); xxx.sortby("fn"); xxx.count_flower++;
            thread t([](flower* x) { update_FlowerWaterNutritionAge(x); }, tmp); t.detach();
            cout << "\n[+] Successfully added flower\n\n"; this_thread::sleep_for(chrono::seconds(1));
        }

        if (option == 2) {
            tree *tmp = new tree;
            string name, age, lifespan;
            cout << "====> Enter name of tree or press enter to back to main menu: "; 
            while (true) {
                getline(cin, name);
                if (!name.empty()) break;
                else break;
            }
            if (name.empty()) {cout << "\n[-] Back to main menu\n\n"; this_thread::sleep_for(chrono::seconds(1)); continue;}
            cout << "====> Enter current age of tree or press enter to back to main menu: ";
            while (true) {
                getline(cin, age);
                if (age.empty()) break;
                if (checkinputfromuser(age, 1)) break;
                else {cout << "[-] Error, please try again, enter a valid age (must be greater than 0) or press enter to back to main menu: "; continue;}
            }
            if (age.empty()) {cout << "\n[-] Back to main menu\n\n"; this_thread::sleep_for(chrono::seconds(1)); continue;}
            cout << "====> Enter lifespan of tree or press enter to back to main menu: ";
            while (true) {
                getline(cin, lifespan);
                if (lifespan.empty()) break;
                if (checkinputfromuser(lifespan, stoi(age) + 1)) break;
                else { cout << "[-] Error, please try again, enter a valid lifespan (must be greater than the age) or press enter to back to main menu: "; continue;}
            }
            if (lifespan.empty()) {cout << "\n[-] Back to main menu\n\n"; this_thread::sleep_for(chrono::seconds(1)); continue;}
            tmp->add_plant(name, stoi(age), stoi(lifespan));
            xxx.addtoTreeList(tmp); xxx.sortby("tn"); xxx.count_tree++;
            thread t([](tree *x) { update_TreeWaterNutritionAge(x); }, tmp); t.detach();
            cout << "\n[+] Successfully added tree\n\n"; this_thread::sleep_for(chrono::seconds(1));
        }

        if (option == 3) {
            if (xxx.check_flower_list()) {
                cout << "\n[-] The list is empty, there are no flowers\n\n";
                this_thread::sleep_for(chrono::seconds(1));
                continue;
            }
            xxx.show_plant_list('f');
            this_thread::sleep_for(chrono::seconds(1));
            additional_opt('f', xxx);
        }

        if (option == 4) {
            if(xxx.check_tree_list()) { 
                cout << "\n[-] The list is empty, there are no trees\n\n"; 
                this_thread::sleep_for(chrono::seconds(1)); continue;
            }
            xxx.show_plant_list('t');
            this_thread::sleep_for(chrono::seconds(1));
            additional_opt('t', xxx);
        }

        if (option == 5) {
            if (xxx.check_died_flowerlist()) { 
                cout << "\n[-] The list is empty, there are no died flowers\n\n"; 
                this_thread::sleep_for(chrono::seconds(1)); continue; } 
            xxx.show_died_plant_list('f'); 
            this_thread::sleep_for(chrono::seconds(1));
        }

        if (option == 6) {
            if (xxx.check_died_treelist()) { 
                cout << "\n[-] The list is empty, there are no died trees\n\n"; 
                this_thread::sleep_for(chrono::seconds(1)); continue; }
            xxx.show_died_plant_list('t');
            this_thread::sleep_for(chrono::seconds(1));
        }

        if (option == 7) {
            if (xxx.check_flower_list()) { cout << "\n[-] There are no flowers in the list\n\n"; this_thread::sleep_for(chrono::seconds(1)); continue;} 
            xxx.clear_list('f'); 
            cout << "\n[-] Delete successfully\n\n"; 
            this_thread::sleep_for(chrono::seconds(1));}
        if (option == 8) { 
            if (xxx.check_tree_list()) { cout << "\n[-] There are no trees in the list\n\n"; this_thread::sleep_for(chrono::seconds(1)); continue;}
            xxx.clear_list('t'); 
            cout << "\n[-] Delete successfully\n\n"; 
            this_thread::sleep_for(chrono::seconds(1));}
        if (option == 9) { 
            if (xxx.check_died_flowerlist()) { cout << "\n[-] There are no dead flowers in the list\n\n"; this_thread::sleep_for(chrono::seconds(1)); continue;}
            xxx.clear_died_list('f'); 
            cout << "\n[-] Delete successfully\n\n"; 
            this_thread::sleep_for(chrono::seconds(1));}
        if (option == 10) { 
            if (xxx.check_died_treelist()) { cout << "\n[-] There are no dead trees in the list\n\n"; this_thread::sleep_for(chrono::seconds(1)); continue;}
            xxx.clear_died_list('t'); 
            cout << "\n[-] Delete successfully\n\n"; 
            this_thread::sleep_for(chrono::seconds(1));}
        if (option == 11) {
            xxx.clear_list('f');
            xxx.clear_list('t');
            xxx.clear_died_list('f');
            xxx.clear_died_list('t');
            stop_all_th = true;
            break;
        }
    }
    this_thread::sleep_for(chrono::seconds(4));
}

int main() {
    weather::set_time(2);
    thread t1(increase_time); t1.detach();
    thread t2(update_weather); t2.detach();
    thread t3(update_season); t3.detach();
    thread t4(check_alive_by_time); t4.detach();

    display x;
    x.show_scr();
}