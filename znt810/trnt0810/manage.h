#ifndef MANAGE_H
#define MANAGE_H
#include "tree.h"
#include <vector>
#include <thread>

using namespace std;

void drawTable(vector<vector<string>>); // function to draw table

class user
{
    private:
        vector<flower*> current_list_flower; // vector of current flower list
        vector<tree*> current_list_tree; // vector of current tree list
        vector<flower*> died_flower; // vector of died flower list
        vector<tree*> died_tree; // vector of died tree list
    public:
        int count_flower = 0, count_tree = 0; // count of flower and tree after each call to add_plant method to add flower/plant
        void show_plant_list(char); // show plant list (flower or tree)
        void show_died_plant_list(char); // show died plant list (flower or tree)
        void addtoFlowerList(flower*);  // add flower to current flower list (currrent_list_flower vector)
        void addtoTreeList(tree*);  // add tree to current tree list (currrent_list_tree vector)
        void removediedplant(); // remove died plant from current flower/tree list and add to died flower/tree list
        void removeoneplant(int, char); // remove one plant from current flower/tree list (release memory)
        void clear_list(char); // clear current flower/tree list (release memory)
        void sortby(string); // sort current flower/tree list by name or age
        flower *take_flower(int); // take flower from current flower list (current_list_flower vector)
        tree *take_tree(int); // take tree from current tree list (current_list_tree vector)
        bool check_flower_list(); // check if current flower list is empty or not (current_list_flower vector)
        bool check_tree_list(); // check if current tree list is empty or not (current_list_tree vector)
        bool check_died_flowerlist(); // check if died flower list is empty or not (died_flower vector)
        bool check_died_treelist(); // check if died tree list is empty or not (died_tree vector)
        void clear_died_list(char); // clear died flower/tree list (release memory)
};

#endif