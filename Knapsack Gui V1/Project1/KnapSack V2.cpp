#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <fstream>
#include<vector>
#include<string>
#include<algorithm>
#include <numeric>      // std::accumulate
#include  <random>
#include  <iterator>
#include <chrono>
using namespace std;

// class item just to mange items in a simple and organized manner
class Item {
public:
    int price, weight;
    string name,pic;
    Item() {
        price = 0;
        weight = 0;
        name = "";
        pic = "";
    }
    Item(int Price, int Weight, string Name,string Pic) {
        price = Price;
        name = Name;
        weight = Weight;
        pic = Pic;
    }
    bool operator==(const Item rhs) const { return (this->price == rhs.price && this->weight == rhs.weight && this->name == rhs.name && this->pic == rhs.pic); }
};
//class for level and its scores
class Level {
public:
    int NumberOfitems, KnapSackMax,score=0,time=0;
    Level() {
        NumberOfitems = 0;
        KnapSackMax = 0;

    }
    Level(int numberOfitems, int knapSackMax) {
        NumberOfitems = numberOfitems;
        KnapSackMax = knapSackMax;
    }
};
//class for knapsack logic
class knapsack {
private:
    //max weight of knapsack
    int KnapsackMaxWeight = 0;
    //max price of knapsack 
    int MaxPrice = 0;
    //vector for all items that a person can put in the knapsack
    vector<Item> AllItems;
    //vector for all items in the knapsack
    vector<Item> ItemsInBag;
    //vector for the items that will make the user win
    vector<Item> WinningItems;
    //vector for shuffled items index in the all vector
    vector<int> relative_position;
public:
    //setters and getters
    void SetKnapsackMaxWeight(int Max) { KnapsackMaxWeight = Max; }
    int GetKnapsackMaxWeight() { return KnapsackMaxWeight; }
    //returns vector for all items that a person can put in the knapsack
    vector<Item> GetAllItems() { return AllItems; }
    //returns vector for all items in the knapsack
    vector<Item> GetItemsInBag() { return ItemsInBag; }
    //returns vector for the items that will make the user win
    vector<Item> GetWinningItems() { return WinningItems; }
    //returns vector for shuffled items index in the all vector
    vector<int> Getrelative_position() { return relative_position; }
    //constructor
    knapsack(int W) {
        KnapsackMaxWeight = W;
    }
    //insert item in the AllItems vector
    bool InsertItem(Item tmp) {
        if (find(AllItems.begin(), AllItems.end(), tmp) == AllItems.end()) {
            AllItems.push_back(tmp);
            return 1;
        }
        else { cout << tmp.name << " already in all items list" << endl; return 0; }

    }
    //insert random limited items in the AllItems vector from file
    void InsertRandomItemsFromFile(string &FileName, int elements) {
        fstream File{ FileName, ios::in };
        //all items
        vector<Item> all;
        //shuffled all items
        vector<Item> allS;
        // exit program if fstream cannot open file
        if (!File) {
            cerr << "File could not be opened." << endl;
            exit(EXIT_FAILURE);
        }
        int price, weight;
        string name, pic;
        while (File >> price >> weight >> name >> pic) {
            all.push_back(Item(price, weight, name, pic));
            allS.push_back(Item(price, weight, name, pic));
        }
        File.close();
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine e(seed);
        shuffle(allS.begin(), allS.end(), e);

        for (int i = 0; i < elements; i++) {
            InsertItem(allS[i]);
            relative_position.push_back(find(all.begin(), all.end(), allS[i]) - all.begin());
        }

    }
    //insert all items from file to vector  AllItems
    void InsertItemsFromFile(string FileName) {
        fstream File{ FileName, ios::in };

        // exit program if fstream cannot open file
        if (!File) {
            cerr << "File could not be opened." << endl;
            exit(EXIT_FAILURE);
        }

        int price, weight;
        string name, pic;

        while (File >> price >> weight >> name >> pic) {
            InsertItem(Item(price, weight, name, pic));
        }

        File.close();

    }
    // A utility function that returns maximum of two integers  
    int max(int a, int b) { return (a > b) ? a : b; }
    // Returns the maximum value that can be put in a knapsack of capacity (KnapsackMaxWeight)
    int knapSackMaxValue()
    {
        int i, w;
        if (WinningItems.size() == 0) {
            vector<vector<int>> K(AllItems.size() + 1, std::vector<int>(KnapsackMaxWeight + 1, 0));
            // Build table K[][] in bottom up manner 
            for (i = 0; i <= AllItems.size(); i++)
            {
                for (w = 0; w <= KnapsackMaxWeight; w++)
                {
                    if (i == 0 || w == 0)
                        K[i][w] = 0;
                    else if (AllItems[i - 1].weight <= w)
                        K[i][w] = max(AllItems[i - 1].price + K[i - 1][w - AllItems[i - 1].weight], K[i - 1][w]);
                    else
                        K[i][w] = K[i - 1][w];
                }
            }
            // stores the result of Knapsack 
            int res;
            res = K[AllItems.size()][KnapsackMaxWeight];

            w = KnapsackMaxWeight;
            for (i = AllItems.size(); i > 0 && res > 0; i--) {
                if (res == K[i - 1][w])
                    continue;
                else {
                    // This item is included.
                    WinningItems.push_back(AllItems[i - 1]);
                    // Since this weight is included its  
                    // value is deducted 
                    res = res - AllItems[i - 1].price;
                    w = w - AllItems[i - 1].weight;
                }
            }
            MaxPrice = K[AllItems.size()][KnapsackMaxWeight];
            return MaxPrice;
        }
        else { return MaxPrice; }
    }
    //game functions
    //insert item to ItemsInBag vector
    bool InsertItemToBag(Item tmp) {
        if (find(ItemsInBag.begin(), ItemsInBag.end(), tmp) == ItemsInBag.end()) {
            if (find(AllItems.begin(), AllItems.end(), tmp) != AllItems.end()) {
                ItemsInBag.push_back(tmp);
                cout << tmp.name << " has been inserted" << endl;
                return 1;
            }
            else { cout << tmp.name << " is not all items list" << endl; return 0; }
        }
        else { cout << tmp.name << " already in bag" << endl; return 0; }
    }
    //remove item from ItemsInBag vector
    bool RemoveItemFromBag(Item tmp) {
        //find the place of the Item in the vector
        vector<Item>::iterator indexItem = find(ItemsInBag.begin(), ItemsInBag.end(), tmp);
        if (indexItem != ItemsInBag.end()) {
            ItemsInBag.erase(indexItem);
            return 1;
        }
        else { cout << tmp.name << " is not in the bag" << endl; return 0; }
    }
    //returns the price of items in bag if didnt exceed the weight
    int PriceOfItemsInBag() {
        int sum = 0;
        for (int i = 0; i < ItemsInBag.size(); i++) { sum = sum + ItemsInBag[i].weight; }
        if (sum <= KnapsackMaxWeight) {
            sum = 0;
            for (int i = 0; i < ItemsInBag.size(); i++) { sum = sum + ItemsInBag[i].price; }
            return sum;
        }
        else { cout << "the max weight has been exceeded" << endl; return -1; }

    }
    //returns the remaining weight 
    int RemainingWeight() {
        int sum = 0;
        for (int i = 0; i < ItemsInBag.size(); i++) { sum = sum + ItemsInBag[i].weight; }
        return (KnapsackMaxWeight - sum);
    }
    //check if the item is in bag or not
    Item InBag(Item tmp) {
        if (find(ItemsInBag.begin(), ItemsInBag.end(), tmp) == ItemsInBag.end()) {
                return tmp;
        }
        else { return Item(0, 0,"",""); }
    }
    //will return the index of the item in the all vector to be able to click it in bot mode
    int Index_In_All(Item tmp) {
        if (find(AllItems.begin(), AllItems.end(), tmp) != AllItems.end()) {
            return find(AllItems.begin(), AllItems.end(), tmp) - AllItems.begin();
        }
        else { return -1; }
    }
    //clear all vectors
    void clear() {
        AllItems.clear();
        ItemsInBag.clear();
        WinningItems.clear();
        relative_position.clear();
    }
};