#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#define DECKSIZE 52


class Deck{

private:
    std::vector<int> deck_;

public:
    Deck(){
        add_deck();
        print_deck();
    }

    void add_deck(){

        deck_.reserve(deck_.size() + DECKSIZE);

        for(int i = deck_.size(); i<DECKSIZE; i++){
            deck_.push_back(i);
        }
        std::shuffle(deck_.begin(),deck_.end(),std::random_device());
    }

    void print_deck(){
        for(int i : deck_){
            std::cout<<i<<" ";
        }
        std::cout << '\n';
    }
};
int main()
{

    Deck deck = Deck();
    return 0;
}
