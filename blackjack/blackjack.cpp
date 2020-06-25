#include <iostream>
#include <vector>

using namespace std;

const int NUM_FACEVALS = 13;
const int NUM_SUITS = 4;
const int HONOR_FACE_VAL = 10;
const int ALTERNATE_ACE_VAL = 11;
const int BURST_THRESH = 21;
const int HIT_THRESH = 17;

enum Suit {
	Begin, Club = Begin, Heart, Spade, Diamond, End
};

class Card {
private:
	int faceVal;
	Suit suit;

public:
	Card(int faceVal, Suit suit){
		this->faceVal = faceVal;
		this->suit = suit;
	}
	
	int getFaceVal() {
		return faceVal;
	}
	
	Suit getSuit() {
		return suit;
	}
};

class Deck {
private:
	vector<Card*> cards;
	int numOfCards;
	int dealIdx;

public:
	Deck() {
		numOfCards = 0;
		dealIdx = 0;
		for (int val = 1; val <= NUM_FACEVALS; val++){
			// iteration through enums in C++ is more complicated than in Java ...
			for (int suitInt = Suit::Begin; suitInt != Suit::End; suitInt++){
				Suit suit = static_cast<Suit>(suitInt);
				cards.push_back(new Card(val, suit));
				numOfCards++;
			}
		}
	}
	
	~Deck(){
		for (int i = 0; i < numOfCards; i++)
			delete cards[i];
		cout << "cleared Deck!" << endl;
	}
	
	void shuffle() {
		for (int i = 0; i < numOfCards; i++){
			int j = i + rand() % (numOfCards - i);
			swap(cards[i], cards[j]);
		}
	}
	
	Card* dealCard() {
		if (dealIdx == numOfCards)
			return NULL;
		return cards[dealIdx++];
	}
	
	vector<Card*> dealHand(int num){
		if (numOfCards - dealIdx < num)
			return {};
		vector<Card*> handCards;
		for (int i = 0; i < num; i++)
			handCards.push_back(dealCard());
		return handCards;
	}
	
	int numRemainingCards() {
		return numOfCards - dealIdx;
	}
};

class Hand {
protected:
	vector<Card*> cards;

public:
	virtual ~Hand(){
		for (int i = 0; i < cards.size(); i++)
			delete cards[i];
		cout << "cleared Hand!" << endl; 		
	}
	
	void addCards(const vector<Card*>& c){
		for (Card* ch : c)
			cards.push_back(ch);
	}
	
	virtual calculateScore() = 0;
	
	int size() {
		return cards.size();
	}
};

class BlackJackHand : public Hand {
public:
	~BlackJackHand(){

	}
	
	int calculateScore() override {
		vector<int> possibleScores = getPossibleScores();		
		int minOver = INT_MAX, maxUnder = INT_MIN;
		for (int score : possibleScores){
			if (score > BURST_THRESH && score < minOver)
				minOver = score;
			else if (score <= BURST_THRESH && score > maxUnder)
				maxUnder = score;
		}
		int finalScore = (maxUnder == INT_MIN ? minOver : maxUnder);
		return finalScore;
	}
	
	vector<int> getPossibleScores() {
		vector<int> possibleScores{0};
		
		for (Card* card : cards){			
			vector<int> newScores;
			for (int score : possibleScores){
				if (isHonor(card))
					newScores.push_back(score + HONOR_FACE_VAL);
				else
					newScores.push_back(score + card->getFaceVal());
					
				if (isAce(card))
					newScores.push_back(score + ALTERNATE_ACE_VAL);
					
			}
			possibleScores = newScores;
		}
		return possibleScores;
	}

	bool isAce(Card* c){
		return c->getFaceVal() == 1;
	}
	
	bool isHonor(Card* c){
		return c->getFaceVal() >= 11 && c->getFaceVal() <= 13;
	}

	
	bool isBurst(){
		return calculateScore() > BURST_THRESH;
	}
	
	bool isBlackJack(){
		if (cards.size() != 2)
			return false;
		return (isAce(cards[0]) && isHonor(cards[1])) || 
			   (isAce(cards[1]) && isHonor(cards[0]));
	}
};

class SimpleBlackJackSimulator {
private:
	int numHands;
	Deck* deck;
	vector<BlackJackHand*> hands;

public:
	SimpleBlackJackSimulator(int numPlayers){
		numHands = numPlayers;
		deck = new Deck();
		deck->shuffle();
		
		for (int i = 0; i < numPlayers; i++)
			hands.push_back(new BlackJackHand());	
	}
	
	~SimpleBlackJackSimulator() {
		cout << "++++++ calling simulator destructor ++++++++" << endl;
		deck->~Deck();
		for (int i = 0; i < numHands; i++)
			hands[i]->~BlackJackHand();
	}
	
	bool dealInitial() {
		for (BlackJackHand* hand : hands){
			vector<Card*> cards = deck->dealHand(2);
			if (cards.empty())
				return false;
			hand->addCards(cards);
		}
		return true;
	}
	
	vector<int> getBlackJacks() {
		vector<int> blackJacks;
		for (int i = 0; i < numHands; i++){
			if (hands[i]->calculateScore() == BURST_THRESH)
				blackJacks.push_back(i);
		}
		return blackJacks;
	}
		
	bool playHand(BlackJackHand* hand){
		while(hand->calculateScore() < HIT_THRESH){
			Card* newCard = deck->dealCard();
			if (newCard == NULL)
				return false;
			hand->addCards({newCard});
		}
		return true;
	}
	
	bool playAllHands() {
		for (BlackJackHand* hand : hands){
			if (!playHand(hand))
				return false;
		}
		return true;
	}
	
	vector<int> getWinners() {
		vector<int> winners;
		int maxScore = 0;
		for (int i = 0; i < numHands; i++){
			BlackJackHand* hand = hands[i];
			if (!hand->isBurst()){
				int score = hand->calculateScore();
				if (score > maxScore){
					maxScore = score;
					winners.clear();
					winners.push_back(i);
				}
				else if (score == maxScore)
					winners.push_back(i);
			}
		}
		return winners;
	}
	
	void printScoreInfo(){
		for (int i = 0; i < numHands; i++){
			cout << "Player " << i << " has score " << hands[i]->calculateScore() << endl;
		}
	}
	
	void simulate(){
		bool success = dealInitial();
		if (!success){
			cout << "Out of Cards for the initial deal" << endl;
			return;
		}
		
		cout << "--------initial deal cards--------" << endl;
		printScoreInfo();
		
		
		vector<int> blackJacks = getBlackJacks();
		if (blackJacks.size() > 0){
			cout << "Black Jacks appear after inital deal cards!" << endl;
			for (int idx : blackJacks)
				cout << "black jack " << idx << " has score " << BURST_THRESH << endl;
			return;
		}
		
		
		success = playAllHands();
		if (!success){
			cout << "Out of Cards for playing the game" << endl;
			return;
		}
		
		cout << "----- complete one round of game -----" << endl;
		printScoreInfo();
		
		vector<int> winners = getWinners();
		if (!winners.empty()){
			for (int idx : winners){
				cout << "winner " << idx << " achives score " << hands[idx]->calculateScore() << endl;
			}
		}
		else
			cout << "Draw. All players bursted" << endl;
	}
};


int main() {
	SimpleBlackJackSimulator simulator(4);
	simulator.simulate();
	return 0;
}
