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
};

class knapsack {
private:
    int KnapsackMaxWeight=0;
    vector<int> ValuesOfAllItems;
    vector<int> WeightsOfAllItems;
    vector<string> NamesOfAllItems;

    vector<int> ValuesOfItemsInBag;
    vector<int> WeightsOftemsInBag;
    vector<string> NamesOfItemsInBag;

    vector<int>TheIndexOfTheResultNumbers;
    vector<int> TheResultNumbersPrices;
    vector<string>TheResultNumbersNames;
public:
    vector<string> GetWinningItems() { return TheResultNumbersNames; }
    knapsack(int W) {
        KnapsackMaxWeight = W;
    }
    void InsertItem(Item &temp) {
        ValuesOfAllItems.push_back(temp.price);
        WeightsOfAllItems.push_back(temp.weight);
        NamesOfAllItems.push_back(temp.name);
        temp.SetItemIndex(ValuesOfAllItems.size() - 1);
    }
    
    // A utility function that returns maximum of two integers  
    int max(int a, int b) { return (a > b) ? a : b; }
// Returns the maximum value that can be put in a knapsack of capacity W 
    int knapSackMaxValue()
    {
        
        int i,w;
        vector<vector<int>> K(ValuesOfAllItems.size() + 1, std::vector<int>(KnapsackMaxWeight + 1, 0));
        // Build table K[][] in bottom up manner 
        for (i = 0; i <= ValuesOfAllItems.size(); i++)
        {
            for (w = 0; w <= KnapsackMaxWeight; w++)
            {
                if (i == 0 || w == 0)
                    K[i][w] = 0;
                else if (WeightsOfAllItems[i - 1] <= w)
                    K[i][w] = max(ValuesOfAllItems[i - 1] + K[i - 1][w - WeightsOfAllItems[i - 1]], K[i - 1][w]);
                else
                    K[i][w] = K[i - 1][w];
            }
        }
        // stores the result of Knapsack 
        int res;
        res= K[ValuesOfAllItems.size()][KnapsackMaxWeight];

        w = KnapsackMaxWeight;
        for (i = ValuesOfAllItems.size(); i > 0 && res > 0; i--) {
            if (res == K[i - 1][w])
                continue;
            else {
                // This item is included.
                TheIndexOfTheResultNumbers.push_back(i - 1);
                TheResultNumbersPrices.push_back(ValuesOfAllItems[i - 1]);
                // Since this weight is included its  
                // value is deducted 
                res = res - ValuesOfAllItems[i - 1];
                w = w - WeightsOfAllItems[i - 1];
            }
        }
        SetWinningItems();
        return K[ValuesOfAllItems.size()][KnapsackMaxWeight];
    }
    //game functions
    bool InsertItemToBag(Item tmp) {
        if (find(NamesOfItemsInBag.begin(), NamesOfItemsInBag.end(), tmp.name)== NamesOfItemsInBag.end()) {
            ValuesOfItemsInBag.push_back(ValuesOfAllItems[tmp.index]);
            WeightsOftemsInBag.push_back(WeightsOfAllItems[tmp.index]);
            NamesOfItemsInBag.push_back(NamesOfAllItems[tmp.index]);
            return 1;
        }
        else { cout << NamesOfAllItems[tmp.index] << " already in bag" << endl; return 0; }
    }
    bool RemoveItemFromBag(Item tmp) {
        //find the place of the Item in the vector
        vector<string>::iterator indexString = find(NamesOfItemsInBag.begin(), NamesOfItemsInBag.end(), tmp.name);
        if (indexString != NamesOfItemsInBag.end()) {
            //convert the iterator to the index to use it with the other vectors
            int index = distance(NamesOfItemsInBag.begin(), indexString);
            vector<int>::iterator indexIntV = ValuesOfItemsInBag.begin() + index;
            vector<int>::iterator indexIntW = WeightsOftemsInBag.begin() + index;
            ValuesOfItemsInBag.erase(indexIntV);
            WeightsOftemsInBag.erase(indexIntW);
            NamesOfItemsInBag.erase(indexString);
            return 1;
        }
        else { cout <<tmp.name<< " is not in the bag" << endl; return 0; }
    }
    int PriceOfItemsInBag(){
        if (accumulate(WeightsOftemsInBag.begin(), WeightsOftemsInBag.end(), 0) <= KnapsackMaxWeight) {
            return accumulate(ValuesOfItemsInBag.begin(), ValuesOfItemsInBag.end(), 0);
        }
        else { cout << "the max weight has been exceeded" << endl; return -1; }

    }    
    void SetWinningItems() {
        for (int i = 0; i < TheIndexOfTheResultNumbers.size(); i++) {
            TheResultNumbersNames.push_back(NamesOfAllItems[TheIndexOfTheResultNumbers[i]]);
        }
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
        cout << Temp.GetWinningItems()[i] << endl;
    }

    return 0;
}