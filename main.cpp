#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <string>
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

    static std::string eval_card(int card_val){
        int d = (int) card_val%13;
        int r = (int) card_val/13;
        char color;
        std::string value;

        switch (r) {
        case 0:
            color = 'c';
            break;

        case 1:
            color = 'd';
            break;

        case 2:
            color = 's';
            break;

        case 3:
            color = 'h';
            break;
        default:
            color=' ';
            break;
        }

        switch (d) {
        case 0:
            value="Ace ";
            break;
        case 12:
            value="King ";
            break;
        case 11:
            value="Queen ";
            break;
        case 10:
            value="Jack ";
            break;
        default:
            value=std::to_string(d+1)+" ";
            break;
        }
        return value+color;
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
        //print_deck();
        return tmp;
    }

        void print_deck(){
        for(int i : deck_){
            std::cout<<i<<" ";
        }
        std::cout << '\n';
    }
};


class Blackjack{
    Deck deck = Deck();
    int player_count;
    int* bank;
    std::vector<std::vector<int>> hands;

public:

    Blackjack(int pCount,int money){
        player_count = pCount;
        bank = (int*) malloc(player_count*2*sizeof(int));
        hands.reserve(player_count+1);// +Dealer
        std::cout<<hands.size();

        set_balance(money);
        game_loop();
    }

    void set_balance(int money){
        for(int i=0; i<player_count;i++){
            bank[2*i]=money;
        }
    }

    void set_bets(){
        for (int i = 0; i<player_count; i++) {
            printf("Player %d place your bet:\n", i);
            std::cin>>bank[2*i+1];
        }
    }

    void deal_cards(){ // init hands

        for(int i = 0; i<player_count+1; i++){
            hands.emplace_back(std::vector<int>());
            hands[i].reserve(10);
        }
        for(int i=0; i<hands.size();i++){
            hands[i].push_back(0); //sum as first elem
        }
        for(int i=0;i<hands.size();i++){
            for(int a=0; a<2; a++){
                int tmp=deck.get_card();
                hands[i][0] += tmp;
                hands[i].push_back(tmp);
            }

        }
    }

    void print_hands(){
        std::cout<<"---Hands---\n";
        for(int i = 0; i<hands.size(); i++){
            printf("Player %d' Hand is: \n",i);
            for(int a = 1; a < hands[i].size(); a++){
                std::cout<<Deck::eval_card(hands[i][a])<<std::endl;
            }
        }
    }

    void game_loop(){
        std::cout<<"--Bet Phase--\n\n";
        set_bets();
        deal_cards();
        print_hands();

    }

};



int main()
{
    char new_game;
    int player;
    int start_money;
    do {
        std::cout<<12/13;
        std::cout<<"How many Player?\n";
        std::cin>>player;
        std::cout<<"How much Money?\n";
        std::cin>>start_money;
        Blackjack blackjack(player,start_money);
        std::cout<<"New Game?(y|n)\n";
        std::cin>>new_game;
    }while (new_game==121); //y
    return 0;
}
