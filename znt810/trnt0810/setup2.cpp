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
} 

void drawTable(vector<vector<string>> table) 
{
    if (table.empty()) return; 

    size_t cot = 0;
    for (vector hang : table) {
        cot = max(cot, hang.size());
    } 

    vector<size_t> colWidths(cot, 0);
    for (auto row : table) {
        for (int i = 0; i < cot; ++i) {
            if (i < row.size()) colWidths[i] = max(colWidths[i], row[i].length());
            
        }
    }

    for (auto hang : table) {
        printSeparator(colWidths);
        for (int i = 0; i < cot; ++i) {
            string cell = (i < hang.size() ? hang[i] : "");
            cout << "| " << left << setw(colWidths[i]) << cell << " ";
        }
        cout << "|\n";
    }
    printSeparator(colWidths);
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

void user::removeoneplant(int index, char x) 
{
    if (x == 'f') { 
        flower* take = current_list_flower[index]; 
        current_list_flower.erase(current_list_flower.begin() + index); 
        take->change_state_of_deleted(); 
        take->change_state_of_deleteforever(); 
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
    /*it will display like this:
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
        /*it will display like this:
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
    if (x == 'f') cout << "\n##### TABLE OF DEAD FLOWER #####\n\n";
    else cout << "\n##### TABLE OF DEAD TREE #####\n\n";
    drawTable(that_need_print);
    cout << '\n';
    that_need_print.clear();
}

bool compare_fname (flower *a, flower *b) {return a->get_name() < b->get_name();} 
bool compare_fage (flower *a, flower *b) {return a->get_age() > b->get_age();} 
bool compare_tname (tree *a, tree *b) {return a->get_name() < b->get_name();} 
bool compare_tage (tree *a, tree *b) {return a->get_age() > b->get_age();}
bool compare_theight (tree *a, tree *b) {return a->get_height() > b->get_height();} 

void user::sortby(string x) 
{
    if (x == "fn") sort(current_list_flower.begin(), current_list_flower.end(), compare_fname); 
    if (x == "fa") sort(current_list_flower.begin(), current_list_flower.end(), compare_fage); 
    if (x == "tn") sort(current_list_tree.begin(), current_list_tree.end(), compare_tname); 
    if (x == "ta") sort(current_list_tree.begin(), current_list_tree.end(), compare_tage); 
    if (x == "th") sort(current_list_tree.begin(), current_list_tree.end(), compare_theight); 
}

void user::clear_list(char x) 
{
    if (x == 'f') { 
        for (auto each : current_list_flower) { 
            each->change_state_of_deleted();
            each->change_state_of_deleteforever();
        }
        count_flower = 0;
        current_list_flower.clear();
    } else {
        for (auto each : current_list_tree) {
            each->change_state_of_deleted();
            each->change_state_of_deleteforever();
        }
        count_tree = 0;
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