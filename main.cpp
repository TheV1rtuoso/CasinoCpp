#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <random>


#define DECKSIZE 52


class Deck{

private:
    std::vector<int> deck_;

public:
    Deck(){
        add_deck();
    }

    void add_deck(){
        deck_.reserve(deck_.size()+DECKSIZE);

        for(int i = deck_.size(); i<DECKSIZE; i++){
            deck_.push_back(i);
        }
        std::shuffle(deck_.begin(),deck_.end(),std::random_device());
    }

    int get_card(){
        if(deck_.size()<DECKSIZE/2){
            add_deck();
        }

        int tmp = deck_.back();
        deck_.pop_back();
        return tmp;
    }

    /*    void print_deck(){
        for(int i : deck_){
            std::cout<<i<<" ";
        }
        std::cout << '\n';
    }*/
};


class Blackjack{
    Deck deck = Deck();
    int player_count;
    int* bank;
    std::vector<std::vector<int>*> hands;

public:

    Blackjack(int pCount,int money){
        player_count = pCount;
        bank = (int*) malloc(player_count*2*sizeof(int));
        hands.reserve(player_count+1);// +Dealer

        set_balance(money);
        game_loop();
    }

    void set_balance(int money){
        for(int i=0; i<player_count;i++){
            bank[2*i]=money;
        }
    }

    void get_bets(){
        for (int i = 0; i<player_count; i++) {
            printf("Player %d place your bet:\n", i);
            std::cin>>bank[2*i+1];
        }
    }

    void deal_cards(){ // init hands
        for(int i = 0; i<player_count+1; i++){
            hands.push_back(new std::vector<int>(10));
        }
        for(int i=0;i<player_count+1;i++){
            hands[i]->push_back(deck.get_card());
            hands[i]->push_back(deck.get_card());
        }
    }

    void game_loop(){
        std::cout<<"--Bet Phase--\n\n";
        get_bets();
        deal_cards();
    }

};



int main()
{
    char new_game;
    int player;
    int startmoney;
    do {
        std::cout<<"How many Player?\n";
        std::cin>>player;
        std::cout<<"How much Money?\n";
        std::cin>>startmoney;
        Blackjack blackjack(player,startmoney);
        std::cout<<"New Game?(y|n)\n";
        std::cin>>new_game;
    }while (new_game==121); //y
    return 0;
}
