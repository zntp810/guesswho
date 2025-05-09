#include "manage.h"
#include <iomanip>
#include <algorithm>

using namespace std;

void printSeparator(const vector<size_t>& colWidths) {
    cout << '+';
    for (size_t w : colWidths) {
        cout << string(w + 2, '-') << '+';
    }
    cout << '\n';
} // function to print separator line for table (used in drawTable function)

void drawTable(vector<vector<string>> table) // function to draw table to print to console screen
{
    if (table.empty()) return; // If the table is empty (has no data), the function returns immediately

    size_t cot = 0;
    for (vector hang : table) {
        cot = max(cot, hang.size());
    } /*Iterate through each row (hang) in the table variable to find the largest number of columns (cot). 
        This ensures that the table can handle rows with different numbers of columns.*/

    vector<size_t> colWidths(cot, 0);
    for (auto row : table) {
        for (int i = 0; i < cot; ++i) {
            if (i < row.size()) colWidths[i] = max(colWidths[i], row[i].length());
            
        }
    } /*Iterate through each row and column, updating the maximum 
    width of each column based on the length of the strings in the table.*/

    for (auto hang : table) {
        printSeparator(colWidths);
        for (int i = 0; i < cot; ++i) {
            string cell = (i < hang.size() ? hang[i] : "");
            cout << "| " << left << setw(colWidths[i]) << cell << " ";
        }
        cout << "|\n";
    }
    printSeparator(colWidths);
    /*Call printSeparator() to print a separator before each row. 
    For each column in the row, print the cell value with left alignment 
    and fixed width (setw(colWidths[i]))*/
}

void user::addtoFlowerList(flower *x) // add flower to current flower list (currrent_list_flower vector)
{
    current_list_flower.push_back(x);
}

void user::addtoTreeList(tree *x) // add tree to current tree list (currrent_list_tree vector)
{
    current_list_tree.push_back(x);
}

void user::removediedplant() // remove died plant (flower and tree) from current flower/tree list and add to died flower/tree list
{
        for (auto i = current_list_flower.begin(); i != current_list_flower.end(); ) {
            if ((*i)->check_alive() == false) {
                (*i)->change_state_of_deleted();
                died_flower.push_back(*i);
                count_flower--;
                i = current_list_flower.erase(i);
            } else ++i;
        }
 
        for (auto i = current_list_tree.begin(); i != current_list_tree.end(); ) {
            if((*i)->check_alive() == false) {
                (*i)->change_state_of_deleted();
                died_tree.push_back(*i);
                count_tree--;
                i = current_list_tree.erase(i);
            } else ++i;
        }
}

/*
This method will be called continuously in a separate thread in the setup3.cpp file to continuously check 
if there are any dead flowers or plants. if it is died, it will only change the state of the deleted variable 
to true to force the thread that is updating the values ​​of age, nutrition, and waterlevel to stop, then it will 
add to the list of dead flowers/trees (died_flower, died_tree)
*/

void user::removeoneplant(int index, char x) // remove one plant from current flower/tree list (release memory)
{
    if (x == 'f') { // if user choose flower
        flower* take = current_list_flower[index]; // take flower from current flower list (current_list_flower vector)
        current_list_flower.erase(current_list_flower.begin() + index); // remove flower from current flower list (current_list_flower vector)
        take->change_state_of_deleted(); // change state of deleted variable to true to force the thread that is updating the values ​​of age, nutrition, and waterlevel in file setup3.cpp to stop
        take->change_state_of_deleteforever(); // change state of delete_forever variable to true to release memory
    } else { // if user choose tree, do the same as above
        tree* take = current_list_tree[index];
        current_list_tree.erase(current_list_tree.begin() + index);
        take->change_state_of_deleted();
        take->change_state_of_deleteforever();
    }
}

void user::show_plant_list(char x) // show plant list (flower or tree) to console screen
{
    
    vector<vector<string>> that_need_print; // 2D vector containing table data
    vector<string> print; // A temporary vector to store each row of the table
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
    /*all line from 109 to 118 is Initializing table header
      it will display like this:
        +---+----------------+----------------+----------------+-----------------+-----------------+
        | ON| Flower Name    | Flower Age     | Flower Lifespan| Water Level     | Nutrition       |
    */

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
        /*Insert each data of flower to table (that_need_print vector)
        it will display like this:
        +---+----------------+----------------+----------------+-----------------+-----------------+
        |ON | Flower Name    | Flower Age     | Flower Lifespan| Water Level     | Nutrition       |
        +---+----------------+----------------+----------------+-----------------+-----------------+
        |1  | EX             | 5              | 10             | 80              | 90              |
        */
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
        /*Insert each data of tree to table (that_need_print vector), same above*/
    }
}
    if (x == 'f') cout << "\n##### TABLE OF FLOWER #####";
    else cout << "\n##### TABLE OF TREE #####";
    cout << "\n[*] Current weather: " << plant::get_currentweather() << "\n";
    cout << "[*] Current seasson: " << flower::get_currentseason() << "\n";
    drawTable(that_need_print);
    cout << '\n';
    that_need_print.clear();
}

void user::show_died_plant_list(char x) // show died plant list (flower or tree) to console screen
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
    print.clear(); // initializing table header

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
        /* insert each date of flower that died to table (that_need_print vector), 
        it has the same format as above */
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
        /* insert each date of tree that died to table (that_need_print vector)
        it has the same format as above */
    }
}
    if (x == 'f') cout << "\n##### TABLE OF DEAD FLOWER #####\n\n";
    else cout << "\n##### TABLE OF DEAD TREE #####\n\n";
    drawTable(that_need_print);
    cout << '\n';
    that_need_print.clear();
}

bool compare_fname (flower *a, flower *b) {return a->get_name() < b->get_name();} // comparator function to sort flower by name
bool compare_fage (flower *a, flower *b) {return a->get_age() > b->get_age();} // comparator function to sort flower by age
bool compare_tname (tree *a, tree *b) {return a->get_name() < b->get_name();} // comparator function to sort tree by name
bool compare_tage (tree *a, tree *b) {return a->get_age() > b->get_age();} // comparator function to sort tree by age
bool compare_theight (tree *a, tree *b) {return a->get_height() > b->get_height();} // comparator function to sort tree by height

void user::sortby(string x) // sort current flower/tree list by name or age (may be height)
{
    if (x == "fn") sort(current_list_flower.begin(), current_list_flower.end(), compare_fname); // sort flower by name
    if (x == "fa") sort(current_list_flower.begin(), current_list_flower.end(), compare_fage); // sort flower by age
    if (x == "tn") sort(current_list_tree.begin(), current_list_tree.end(), compare_tname); // sort tree by name
    if (x == "ta") sort(current_list_tree.begin(), current_list_tree.end(), compare_tage); // sort tree by age
    if (x == "th") sort(current_list_tree.begin(), current_list_tree.end(), compare_theight); // sort tree by height
}

void user::clear_list(char x) // clear current flower/tree list (release memory)
{
    if (x == 'f') { // if user choose flower
        for (auto each : current_list_flower) { /* change state of deleted variable to true to force the thread that is updating 
            the values ​​of age, nutrition, and waterlevel in file setup3.cpp to stop and change deleted_forever to true to release memory*/
            each->change_state_of_deleted();
            each->change_state_of_deleteforever();
        }
        count_flower = 0;
        current_list_flower.clear();
    } else { // same as above but for tree
        for (auto each : current_list_tree) {
            each->change_state_of_deleted();
            each->change_state_of_deleteforever();
        }
        count_tree = 0;
        current_list_tree.clear();
    }
}

flower *user::take_flower(int index) // take flower from current flower list (current_list_flower vector)
{
    return current_list_flower[index];
}

tree *user::take_tree(int index) // take tree from current tree list (current_list_tree vector)
{
    return current_list_tree[index];
}

bool user::check_flower_list() // check if current flower list is empty or not (current_list_flower vector)
{
    return current_list_flower.empty();
}

bool user::check_tree_list() // check if current tree list is empty or not (current_list_tree vector)
{
    return current_list_tree.empty();
}

bool user::check_died_flowerlist() // check if died flower list is empty or not (died_flower vector)
{
    return died_flower.empty();
}

bool user::check_died_treelist() // check if died tree list is empty or not (died_tree vector)
{
    return died_tree.empty();
}

void user::clear_died_list(char x) // clear died flower/tree list (release memory)
{
    if (x == 'f') { // if user choose flower
        for (auto each : died_flower) delete each; // release memory of each flower in died_flower vector
        died_flower.clear(); // clear died_flower vector
    } else { // if user choose tree, do the same as above
        for (auto each : died_tree) delete each;
        died_tree.clear();
    }
}