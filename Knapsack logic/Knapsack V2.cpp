#include <iostream>
#include<vector>
#include<string>
#include<algorithm>
#include <numeric>      // std::accumulate

using namespace std;

// class item just to mange items in a simple and organized manner
class Item {
public:
    int price, weight,index=-1;
    string name;
    Item() { price = 0;
    weight = 0;
    name = "";
    }
    Item(int Price, int Weight,string Name) {
        price = Price;
        name = Name;
        weight = Weight;
    }
    void SetItemIndex(int Index) {
        index = Index;
    }
    bool operator==(const Item &rhs) const { return this->price == rhs.price; }
};

class knapsack {
private:
    int KnapsackMaxWeight=0;
    vector<Item> AllItems;

    vector<Item> ItemsInBag;

    vector<Item> WinningItems;

public:
    vector<Item> GetWinningItems() { return WinningItems; }
    knapsack(int W) {
        KnapsackMaxWeight = W;
    }
    void InsertItem(Item &temp) {
        AllItems.push_back(temp);
    }
    
    // A utility function that returns maximum of two integers  
    int max(int a, int b) { return (a > b) ? a : b; }
// Returns the maximum value that can be put in a knapsack of capacity W 
    int knapSackMaxValue()
    {
        
        int i,w;
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
        res= K[AllItems.size()][KnapsackMaxWeight];

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
        return K[AllItems.size()][KnapsackMaxWeight];
    }
    //game functions
    bool InsertItemToBag(Item &tmp) {
        if (find(ItemsInBag.begin(), ItemsInBag.end(), tmp)== ItemsInBag.end()) {
            ItemsInBag.push_back(tmp);
            return 1;
        }
        else { cout << tmp.name << " already in bag" << endl; return 0; }
    }
    bool RemoveItemFromBag(Item tmp) {
        //find the place of the Item in the vector
        vector<Item>::iterator indexItem = find(ItemsInBag.begin(), ItemsInBag.end(), tmp);
        if (indexItem != ItemsInBag.end()) {
            ItemsInBag.erase(indexItem);
            return 1;
        }
        else { cout <<tmp.name<< " is not in the bag" << endl; return 0; }
    }
    
    int PriceOfItemsInBag(){
        int sum = 0;
        for (int i = 0; i < ItemsInBag.size(); i++) { sum = sum + ItemsInBag[i].weight; }
        if (sum<= KnapsackMaxWeight) {
            sum = 0;
            for (int i = 0; i < ItemsInBag.size(); i++) { sum = sum + ItemsInBag[i].price; }
            return sum;
        }
        else { cout << "the max weight has been exceeded" << endl; return -1; }

    }    
};


int main()
{
    knapsack Temp(10);
    Item item1(42, 7, "item1");
    Item item2(12, 3, "item2");
    Item item3(40, 4, "item3");
    Item item4(25, 5, "item4");
    Temp.InsertItem(item1);
    Temp.InsertItem(item2);
    Temp.InsertItem(item3);
    Temp.InsertItem(item4);
    cout<<Temp.knapSackMaxValue()<<endl;
    Temp.InsertItemToBag(item1);
    Temp.InsertItemToBag(item1);
    Temp.InsertItemToBag(item2);
    Temp.InsertItemToBag(item2);

    Temp.RemoveItemFromBag(item3);

    cout<< Temp.PriceOfItemsInBag() << endl;

    cout << "winning state" << endl;
    for (int i = 0; i < Temp.GetWinningItems().size(); i++) {
        cout << Temp.GetWinningItems()[i].name << endl;
    }

    return 0;
}