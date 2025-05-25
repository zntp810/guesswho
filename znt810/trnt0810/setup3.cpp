#include "display.h"
#include <mutex>

using namespace std;

bool stop_all_th = false; // global variable to stop all threads
int day_by_day = 0; // global variable to count time, each 60 seconds is a week
user xxx; 

void increase_time() // function to increase time by 1 second 
{
    while (!stop_all_th) { // check if the program is running
        unique_lock<mutex> lock(mtx);
        day_by_day++; // increase time by 1 second
        lock.unlock();
        this_thread::sleep_for(chrono::seconds(1)); // sleep for 1 second
    }
    cout << "*";
}

void update_weather() // function to update weather every 1 minute (60 seconds)
{   
    while (!stop_all_th) { // check if the program is running
        unique_lock<mutex> lock(mtx);
        int current_dbd = day_by_day; // get current time in seconds
        lock.unlock();
        if (current_dbd % 60 == 0) { // if current time is a multiple of 60 seconds
            flower::change_weather(weather_[random_i(0, 3)]); // change weather randomly
            this_thread::sleep_for(chrono::seconds(2)); // sleep for 2 seconds
        } 
    }
    cout << "*";
}

void update_season() // function to update season every 2 minutes (120 seconds)
{   
    while (!stop_all_th) {
        unique_lock<mutex> lock(mtx);
        int current_dbd = day_by_day; // get current time in seconds
        lock.unlock();
        if (current_dbd % (weather::get_time()*60) == 0) { // if current time is a multiple of (weather::get_time()*60) seconds
            weather::change_season(); // change to the next season
            this_thread::sleep_for(chrono::seconds(2)); // sleep for 2 seconds
        }
    }
    cout << "*";
}

void update_FlowerWaterNutritionAge(flower* x) // function to update water, nutrition and age of flower every 1 second
{   
    int live = 0; // variable to count time, each 60 seconds is a week
    while (!x->get_state_of_deleted()) { // while !(the deleted variable) is not false
        unique_lock<mutex> lock(mtx);
        x->reducewaterbytime(); // call reducewaterbytime() function to reduce water level 
        x->reducenutritionbytime(); // call reducenutritionbytime() function to reduce nutrition level
        if (++live % 60 == 0) { // if live is a multiple of 60 seconds (1 minute), it means that 1 week has passed, assume 1 week = 1 age
            x->update_age(); // call update_age() function to update age of flower
        }
        lock.unlock();
        this_thread::sleep_for(chrono::seconds(1));
    }
    if (x->get_state_of_deleteforever()) delete x; // if the deleted_forever variable is true, delete the flower object
    if (stop_all_th) cout << "*";
}

void update_TreeWaterNutritionAge(tree* x) /* function to update water, nutrition and age of 
                                            tree every 1 second, same as above */
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
    if (stop_all_th) cout << "*";
}

void check_alive_by_time() // function to check if the plant is alive every 1 second
{   
    while (!stop_all_th) { // check if the program is running
        xxx.removediedplant(); // call removediedplant() function to check if the plant is alive or not
        this_thread::sleep_for(chrono::seconds(1));
    }
    cout << "*";
}

bool checkinputfromuser(string input, int start = -1, int end = -1) // function to check if the input is a number or not, and check if it is in the range [start, end] if start and end are not -1
{
    bool check = true;
    for (auto each : input) { if (!isdigit(each)) { return false;} }
    if (start != -1 && end != -1) { if (!(stoi(input) >= start && stoi(input) <= end)) check = false; }
    if (start != -1) if (!(stoi(input) >= start)) check = false;
    return check;
}

void show_options() // function to show main options
{
    vector<vector<string>> options; // 2D vector containing table data, Each element of options is a row in the table (contains options)
    vector<string> option; // A temporary vector to store the data of each row
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
    /*Add options to the table
      it will look like this:
        +----+---------------------------------+
        | ON |            Options              |
        +----+---------------------------------+
        | 1  | Add a flower                    |
        +----+---------------------------------+
        | 2  | Add a tree                      |
    */
    cout << "### MAIN OPTIONS ###\n";
    drawTable(options);
}

void additional_opt(char x, user &xx) /* function to show additional options based on the type of plant 
                                        (flower or tree), this function is called only if the user chooses 
                                        to show the flower or tree list (3 or 4) */
{
    vector<vector<string>> options; // 2D vector containing table data (contain options)
    vector<string> option; // A temporary vector to store the data of each row
    option.push_back("1"); option.push_back("Show table that sort by name (default)"); options.push_back(option); option.clear();
    option.push_back("2"); option.push_back("Show table that sort by age"); options.push_back(option); option.clear();
    option.push_back("3"); option.push_back("Water a plant"); options.push_back(option); option.clear();
    option.push_back("4"); option.push_back("Feed a plant"); options.push_back(option); option.clear();
    option.push_back("5"); option.push_back("Show this table again"); options.push_back(option); option.clear();
    option.push_back("6"); option.push_back("Delete a plant"); options.push_back(option); option.clear();
    // lines 136 - 141 is initializing data for the options table

    if (x == 't') {
        option.push_back("7"); option.push_back("Show table sort by height"); options.push_back(option); option.clear();
    } // If x == 't' (user selects tree), add option to sort table by height
    /*
      it will look like this:
        +----+---------------------------------+
        | ON |            Options              |
        +----+---------------------------------+
        | 1  | Show table that sort by name    |
        +----+---------------------------------+
        | 2  | Show table that sort by age     |
    */

    string get_aopt;
    while (true) { // this while loop displays options panel and input processing 
        cout << "\n###You can select one of the following options or press enter to exit###\n";
        drawTable(options); // Display the options panel by calling drawTable function
        cout << "====> Enter your choice: ";
        getline(cin, get_aopt); // get input from user
        if (get_aopt.empty()) { cout << "\n[-] Back to main Menu\n\n"; this_thread::sleep_for(chrono::seconds(1)); return; } // if user press enter, back to main menu
        if (x == 't') {
            if (!checkinputfromuser(get_aopt, 1, 7)) {cout << "\n[-] Error input, try again\n"; this_thread::sleep_for(chrono::seconds(1)); continue; } }
        else { if (!checkinputfromuser(get_aopt, 1, 6)) {cout << "\n[-] Error input, try again\n"; this_thread::sleep_for(chrono::seconds(1)); continue; } }
        /* lines 163 - 166: 
           - if user input is empty (get_aopt = ''), back to main menu (before that, it will display a message)
           - if user's input is not empty, check whether the input is a number or not, and check 
             it is in the range [1, 7] if x == 't' (tree) or [1, 6] if x == 'f' (flower). If it does 
             not satisfy the condition, print out the message and skip the whole thing to start over
        */

        int get_aopt_i = stoi(get_aopt);
        if ((x != 't' && xx.check_flower_list()) || (x == 't' && xx.check_tree_list())) {
            cout << "\n[-] List now is empty, back to main menu\n\n";
            this_thread::sleep_for(chrono::seconds(1)); return;
        }
        /* If the tree or flower list is empty, print a message and return to the main menu
           - The flower list can be empty because the tree or flower has died for some reason 
             and it is immediately removed from current_list_flower and so on until at some point 
             the current flower list is empty.
        */
        if (get_aopt_i == 1) { 
            if (x == 't') xx.sortby("tn"); else xx.sortby("fn"); 
            cout << "\n[*] Successfully sorted by name\n\n"; this_thread::sleep_for(chrono::seconds(1)); continue;
        } /* option 1: Call the sortby method to sort the list by name
             - tn: sort list by name for tree
             - fn: sort list by name for flower
          */
        if (get_aopt_i == 2) { 
            if (x == 't') xx.sortby("ta"); else xx.sortby("fa");
            cout << "\n[*] Sucessfully sorted by age\n\n"; this_thread::sleep_for(chrono::seconds(1)); continue;
        } /* option 2: Call the sortby method to sort the list by age
             - ta: sort list by age for tree
             - fa: sort list by age for flower
          */
        if (get_aopt_i == 3 || get_aopt_i == 4 || get_aopt_i == 6) {
            /*
                option 3: Water a plant, if user enter this option, it will display a 
                        message to choose a plant to water
                option 4: Feed a plant, if user enter this option, it will display a 
                        message to choose a plant to feed
                option 6: Delete a plant, if user enter this option, it will display a 
                        message to choose a plant to delete
            */
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
            /*
                lines 212 - 223: 
                - get input from user, if user press enter, break the loop and continue (based on the if statement below)
                - if user's input is not empty, check whether the input is a number or not, and check it is in the range [1, xx.count_flower] 
                  if x == 'f' (flower) or [1, xx.count_tree] if x == 't' (tree). If it does not satisfy 
                  the condition, print out the message and skip the whole thing to start over
                - water_feed_delete will be the location of the flower or plant in the table (numerical order) that 
                  the user wants to water, feed or delete depending on whether get_aopt is 3, 4 or 6
                  for example:
                  +-----+------------------+------------------+-------------------+-----------------+
                  | No  |      Name        |       Age        |      Lifespan     |   Water level   | ...
                  +-----+------------------+------------------+-------------------+-----------------+
                  | 1   |      ft1         |        2         |         5         |       50        | ...
                  +-----+------------------+------------------+-------------------+-----------------+ 
                  | 2   |      ft3         |        3         |         9         |       60        | ...
                  +-----+------------------+------------------+-------------------+-----------------+
                  |3    |      ft2         |        1         |         2         |       70        | ...
                  if water_feed_delete = 2, it means that the user wants to water 
                  or feed or delete the flower or plant at index 2 in the table (ft3), not ft2
            */

            if (water_feed_delete.empty()) continue; // if user press enter, continue to the next iteration of the while loop

            if (get_aopt_i == 3) {
                /* 
                    if option is 3, call the water() method of the object flower(tree) 
                    that is choosen by the user
                */
                if (x != 't') { // if x == 'f' (flower)
                    flower *tmp = xx.take_flower(stoi(water_feed_delete) - 1); // get flower object from the list by index (water_feed_delete - 1)
                    tmp->water();
                    this_thread::sleep_for(chrono::seconds(1));
                } else { // else if x == 't' (tree)
                    tree *tmp = xx.take_tree(stoi(water_feed_delete) - 1); // get tree object from the list by index (water_feed_delete - 1)
                    tmp->water();
                    this_thread::sleep_for(chrono::seconds(1));
                }
            } else if (get_aopt_i == 4) {
                /*
                    if option is 4, call the feed() method of the object flower(tree) 
                    that is choosen by the user
                */
                if (x != 't') { // if x == 'f' (flower)
                    flower *tmp = xx.take_flower(stoi(water_feed_delete) - 1); // get flower object from the list by index (water_feed_delete - 1)
                    tmp->feed();
                    this_thread::sleep_for(chrono::seconds(1));
                } else { // else if x == 't' (tree)
                    tree *tmp = xx.take_tree(stoi(water_feed_delete) - 1);
                    tmp->feed();
                    this_thread::sleep_for(chrono::seconds(1));
                }
            } else if (get_aopt_i == 6) {
                /*
                    if option is 6, call the removeoneplant() method of the object flower(tree) 
                    that is choosen by the user to delete it from the current flower/tree list
                */
                if (x != 't') { // if x == 'f' (flower)
                    xx.removeoneplant(stoi(water_feed_delete) - 1, 'f');
                    xx.count_flower--; // decrease the count of flower by 1
                } else { // else if x == 't' (tree)
                    xx.removeoneplant(stoi(water_feed_delete) - 1, 't');
                    xx.count_tree--; } // decrease the count of tree by 1
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
                /* 
                    lines 289 - 301: check if the current flower/tree list is empty or not
                    after deleting a flower/tree, if the current flower/tree list is empty, 
                    print a message and return to the main menu
                */
            }
            continue;
        }

        if (get_aopt_i == 5) {
            /*
                if option is 5, call the show_plant_list() method to show all the flower(tree)
                in current flower/tree list again
            */
            if (x != 't') xx.show_plant_list('f');
            else xx.show_plant_list('t');
            this_thread::sleep_for(chrono::seconds(1));
            continue;
        }

        if (get_aopt_i == 7) {
            // call sortby method with parameter is "th" to sort tree list by height
            xx.sortby("th");
            cout << "\n[*] Sucessfully sort by height\n";
            this_thread::sleep_for(chrono::seconds(1)); 
        }
    }
}

void display::show_scr() 
{   
    string takeoptionfromuser;
    while (true) { // this is main loop of program, show main menu to console screen
        show_options(); // show main menu to console by calling show_options() func
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
        /*
            lines 337 - 345: check valid input
            - if user's input is empty, print message "Enter option" again and continue loop
            - if users' input != empty, check if it is a number and if it is between 1-11 
            (because there are only 11 options in the show_options function). If it does not 
            satisfy the condition, the loop continues, else break
        */

        int option = stoi(takeoptionfromuser); /* This function will never fail because to get to this 
                                                  step it has to pass the above check (which checks that 
                                                  the variable takeoptionfromuser must be a number but in 
                                                  string form)
                                               */

        if (option == 1) {
            /*
                - if user's input is 1, it will create a new flower object, call add_plant() method to 
                enter data for this new flower (may be press enter to back to main menu again if user 
                do not want to add flower). After enter data for flower, this flower object will be added to
                a list that used to manage flower that alived (current_list_flower). 
                - After initializing a flower object using the add_plant method and adding it to the
                current_list_flower list, it will immediately initiate a thread and separate it to run separately  
                until there is a signal to stop the thread, this thread is used to subtract a certain amount of water  
                (except for rain), nutrients and update the age. Every second that passes, the flower will lose a random  
                amount of water and nutrients. Every 1 minute, the age is added by 1
            */
            flower *tmp = new flower;
            string name = "", age, lifespan;
            cout << "====> Enter name of flower or press enter to back to main menu: "; 
            while (true) {
                getline(cin, name);
                if (!name.empty()) break;
                else break;
            } // enter name, if user's input is empty, it mean that user do not want to add plant, so break the loop
            if (name.empty()) {cout << "\n[-] Back to main menu\n\n"; this_thread::sleep_for(chrono::seconds(1)); continue;}
            cout << "====> Enter current age of flower or press enter to back to main menu: ";
            while (true) {
                getline(cin, age);
                if (age.empty()) break;
                if (checkinputfromuser(age, 1)) break;
                else {cout << "[-] Error, please try again, enter a valid age (must greater than 0) or press enter to back to main menu: "; continue;}
            } // enter age, if user's input is empty, it mean that user do not want to add_plant, else check condition
            if (age.empty()) {cout << "\n[-] Back to main menu\n\n"; this_thread::sleep_for(chrono::seconds(1)); continue;}
            cout << "====> Enter lifespan of flower or press enter to back to main menu: ";
            while (true) {
                getline(cin, lifespan);
                if (lifespan.empty()) break;
                if (checkinputfromuser(lifespan, stoi(age) + 1)) break;
                else { cout << "[-] Error, please try again, enter a valid lifespan (must be greater than the age) or press enter to back to main menu: "; continue;}
            } // enter lifespan, if press enter, back to main menu else check condition. (it must be greater than age)
            if (lifespan.empty()) {cout << "\n[-] Back to main menu\n\n"; this_thread::sleep_for(chrono::seconds(1)); continue;}
            tmp->add_plant(name, stoi(age), stoi(lifespan)); 
            xxx.addtoFlowerList(tmp); xxx.sortby("fn"); xxx.count_flower++; // add to current_list_flower
            thread t([](flower* x) { update_FlowerWaterNutritionAge(x); }, tmp); t.detach(); 
            cout << "\n[+] Successfully added flower\n\n"; this_thread::sleep_for(chrono::seconds(1));
        }

        if (option == 2) { 
            /*
                if user's input is 2, it will also create a new object but this time is tree, call add_plant() method to 
                enter data for this new tree (may be press enter to back to main menu again if user 
                do not want to add tree). The rest is exactly the same as what happened in option 1
            */
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
            if (xxx.check_flower_list()) { // before show list, it will check empty
                cout << "\n[-] The list is empty, there are no flowers\n\n";
                this_thread::sleep_for(chrono::seconds(1));
                continue;
            } 
            xxx.show_plant_list('f'); // if current_list_flower is not empty, it will call show_plant_method with parameter 'f' to show flower list (current)
            this_thread::sleep_for(chrono::seconds(1));
            additional_opt('f', xxx); /* after print out the current flower list,
                                        it will do everything in additional_opt function 
                                        until additional_opt function ends */
        }

        if (option == 4) { // similar to option 3 but this time for tree
            if(xxx.check_tree_list()) { 
                cout << "\n[-] The list is empty, there are no trees\n\n"; 
                this_thread::sleep_for(chrono::seconds(1)); continue;
            }
            xxx.show_plant_list('t');
            this_thread::sleep_for(chrono::seconds(1));
            additional_opt('t', xxx);
        }

        if (option == 5) {
            if (xxx.check_died_flowerlist()) { // before show died flower list, it also check empty
                cout << "\n[-] The list is empty, there are no died flowers\n\n"; 
                this_thread::sleep_for(chrono::seconds(1)); continue; } 
            xxx.show_died_plant_list('f');  // call method show_died_plant_list with param 'f' to show died flower
            this_thread::sleep_for(chrono::seconds(1));
        }

        if (option == 6) { // similar to option 5 but this time is for tree
            if (xxx.check_died_treelist()) { 
                cout << "\n[-] The list is empty, there are no died trees\n\n"; 
                this_thread::sleep_for(chrono::seconds(1)); continue; }
            xxx.show_died_plant_list('t');
            this_thread::sleep_for(chrono::seconds(1));
        }

        if (option == 7) { // clear current_list_flower (the list of flowers that are still alive)
            // before clear list, it will check empty
            if (xxx.check_flower_list()) { cout << "\n[-] There are no flowers in the list\n\n"; this_thread::sleep_for(chrono::seconds(1)); continue;} 
            xxx.clear_list('f'); // if current_list_flower is not empty, clear it
            cout << "\n[-] Delete successfully\n\n"; 
            this_thread::sleep_for(chrono::seconds(1));}
        if (option == 8) { // clear current_list_tree (the list of trees that are still alive)
            // before clear list, it will also check empty
            if (xxx.check_tree_list()) { cout << "\n[-] There are no trees in the list\n\n"; this_thread::sleep_for(chrono::seconds(1)); continue;}
            xxx.clear_list('t'); // if current_list_tree is not empty, clear it
            cout << "\n[-] Delete successfully\n\n"; 
            this_thread::sleep_for(chrono::seconds(1));}
        if (option == 9) {  // clear the list of dead flowers
            // before clear list, it will check empty
            if (xxx.check_died_flowerlist()) { cout << "\n[-] There are no dead flowers in the list\n\n"; this_thread::sleep_for(chrono::seconds(1)); continue;}
            xxx.clear_died_list('f'); // if dead flower list is not empty, clear it
            cout << "\n[-] Delete successfully\n\n"; 
            this_thread::sleep_for(chrono::seconds(1));}
        if (option == 10) { // clear the list of dead trees
            // before clear list, it also check empty
            if (xxx.check_died_treelist()) { cout << "\n[-] There are no dead trees in the list\n\n"; this_thread::sleep_for(chrono::seconds(1)); continue;}
            xxx.clear_died_list('t'); // if dead trees list is not empty, clear it
            cout << "\n[-] Delete successfully\n\n"; 
            this_thread::sleep_for(chrono::seconds(1));}
        if (option == 11) { // exit program and before exit, it will clean up memory
            xxx.clear_list('f');
            xxx.clear_list('t');
            xxx.clear_died_list('f');
            xxx.clear_died_list('t');
            stop_all_th = true; // force 4 running threads (initialized in main.cpp) to stop
            break;
        }
    }
    this_thread::sleep_for(chrono::seconds(4));
}