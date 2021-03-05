#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <string>
#include <random>

#define DECKSIZE 52
#define BLACKJACK 21

class Hand;

class Deck{

private:
    std::vector<int> deck_;
  //  const std::string suits [4] = {"C","H","S","D"};
  //  const std::string values [13] = {"Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen","King"};

public:
    Deck(){
        add_deck();
        //print_deck();
    }

    static int eval_card_value(int card_val){
        int m = card_val % 13;
        int value;
        switch (m) {
        case 0:
            value=11;
            break;
        case 12:
            value=10;
            break;
        case 11:
            value=10;
            break;
        case 10:
            value=10;
            break;
        default:
            value=m+1;
            break;
        }
        return value;
    }

    static std::string eval_card(int card_val){
        int value = (int) card_val%13;
        int suit = (int) card_val/13;

        std::string val;
        switch (value) {
        case 0:
            val="Ace";
            break;
        case 11:
            val="Queen";
            break;
        case 10:
            val="Jack";
            break;
        case 12:
            val="King";
            break;
        default:
            val=std::to_string(value+1);
            break;
        }
        char st;
        switch (suit) {
        case 0:
            st='C';
            break;
        case 1:
            st='H';
            break;
        case 2:
            st='S';
            break;
        case 3:
            st='D';
            break;
        default:
            st=' ';
            break;
        }
        return val+st;

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
            std::cout<<"++Deck refilled++\n";
            add_deck();
        }

        int tmp = deck_.back();
        deck_.pop_back();
        return tmp;
    }

        void print_deck(){
        for(int i : deck_){
            std::cout<<i<<" ";
        }
        std::cout << '\n';
    }
};

class Hand{

    std::vector<int> hand;
    int value=0;
    int ace_count=0; //aces that count 11p

public:

    Hand(){
        hand.reserve(12);

    }

    [[nodiscard]] int get_val() const{
        return value;
    }

    void insert_card(int val){
        int p = Deck::eval_card_value(val);
        if(p==11){
            ace_count++;
        }
        value += p;
        while (value>BLACKJACK && ace_count){
            value-=10;
            ace_count--;
        }
        hand.push_back(val);
    }

    void print_hand(int i,char dealer){
        if(dealer){
            printf("Dealer's Hand is: ");
        }else{
            printf("Player %d' Hand is: ",i);
        }
        for(int a : hand){
            std::cout<<Deck::eval_card(a)<<" ";
        }
        if(value==BLACKJACK){
            std::cout<<"| Blackjack\n";
        }
        std::cout<<"| Handvalue is " << value << std::endl;
    }



};

class Blackjack{
    Deck deck = Deck();
    unsigned int player_count;
    unsigned int start_player;
    int* bank;
    std::vector<Hand*> hands;
    int game_count =  1;
public:

    Blackjack(int pCount,int money){
        bank = (int*) malloc(start_player*2*sizeof(int));
        player_count = pCount;
        start_player = pCount;
        hands.reserve(player_count+1);// +Dealer
        set_balance(money);
        game_loop();
    }

    void display_balance(){
        for (int i = 0; i<start_player; i++) {
            std::cout << "Player " << i << "'s bank balance is: " << bank[2*i]<<std::endl;

        }
    }

    void set_balance(int money){
        for(int i=0; i<player_count;i++){
            bank[2*i]=money;
        }
    }

    void set_bets(){
        for (int i = 0; i<player_count; i++) {
            do {
                printf("Player %d place your bet(max. %d):\n", i, bank[2*i]);
                std::cin >> bank[2*i+1];
            }while (bank[2*i+1]>bank[2*i]);
            bank[2*i]-=bank[2*i+1];
        }
    }




    void init_hands(){ // init hands
        hands.clear();
        for(int i=0; i<player_count+1;i++){
            hands.push_back(new Hand());
        }
        for(auto & hand : hands){
            for(int a=0; a<2; a++){
              hand->insert_card(deck.get_card());
            }
        }
    }


    void game_loop(){

        do {
            std::cout << "Round "<< game_count <<" ";
            std::cout << "--Bet Phase--\n\n";
            set_bets();
            init_hands();

            // Print all Hands
            std::cout << "---Hands---\n\n";
            for (int i = 0; i<player_count; i++) {
                hands[i]->print_hand(i, 0);
            }

            hands.back()->print_hand(0, 1);

            //Draw Phase
            std::cout << "\n\n--Draw Phase--\n\n";
            char new_card = 'y';
            for (int i = 0; i<player_count; i++) {
                while (new_card=='y' && hands[i]->get_val()<BLACKJACK) {
                    hands[i]->print_hand(i, 0);
                    std::cout << "New Card?\n";
                    std::cin >> new_card;
                    if (new_card=='y') {
                        hands[i]->insert_card(deck.get_card());
                    }
                }
                hands[i]->print_hand(i, 0);
                new_card = 'y';
            }

            // Dealer's Turn
            std::cout << "\n--Dealer's Turn--\n";
            while (hands.back()->get_val()<17) {
                hands.back()->insert_card(deck.get_card());
                hands.back()->print_hand(0, 1);
            }
            hands.back()->print_hand(0,1);
            std::cout<<"--Dealer's Turn ended--\n";

            //calculate new bank
            for (int i = 0; i<player_count; i++) {
                if (hands[i]->get_val()>BLACKJACK) {
                    printf("Player %d lost %d$\n",i,bank[2*i+1]);
                }
                else if (hands.back()->get_val()>BLACKJACK) {
                    bank[2*i] += bank[2*i+1];
                    printf("Player %d won %d$\n",i,bank[2*i+1]);
                }
                else if (hands[i]->get_val()>hands.back()->get_val()) {
                    bank[2*i] += bank[2*i+1];
                    printf("Player %d won %d$\n",i,bank[2*i+1]);
                }
                else if(hands[i]->get_val()==hands.back()->get_val()){
                     printf("Player %d is even\n",i);
                }
                else{
                    printf("Player %d lost %d$\n",i,bank[2*i+1]);
                }
                

                if (bank[2*i]<=0) {
                    printf("Player %d is out\n",i);
                    player_count--;
                }
            }

            game_count++;
            display_balance();
        }
        while (player_count>0);

    }
};





int main(){
    char new_game;
    int player;
    int start_money;
    do {
        std::cout<<"How many Player?\n";
        std::cin>>player;
        std::cout<<"How much Money?\n";
        std::cin>>start_money;
        Blackjack blackjack(player,start_money);
        std::cout<<"New Game?(y|n)\n";
        std::cin>>new_game;
    }while (new_game=='y'); //y
    return 0;
}
