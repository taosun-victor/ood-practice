#include <iostream>
#include <vector>

using namespace std;

// ============== custom class, test purpose ==================
class Coordinate {
private:
	int x;
	int y;
	
public:
	Coordinate(){
		x = 0;
		y = 0;
	}
	
	Coordinate(int x, int y){
		this->x = x;
		this->y = y;
	}
	
	int getX(){
		return this->x;
	}
	
	int getY(){
		return this->y;
	}
	
	Coordinate& operator=(const Coordinate& other){
		this->x = other.x;
		this->y = other.y;
		return *this;
	}
	
	bool operator==(Coordinate other){
		return other.x== this->x && other.y== this->y;
	}
	
	bool operator!=(Coordinate other){
		return other.x != this->x || other.y != this->y;
	}
};


namespace std{
	template<>
	struct hash<Coordinate>{
		size_t operator() (Coordinate coord) const {
			return hash<int>()(coord.getX()) * 101 + hash<int>()(coord.getY());
		}
	};
}


// =================== Node in the linked list ==============================
template <typename K, typename V>
class Node {
private:
	K key;
	V val;
	
	Node<K, V>* next;
	
public:
	Node(K key, V val){
		this->key = key;
		this->val = val;
		next = NULL;
	}
	
	K getKey(){
		return key;
	}
	
	V getValue(){
		return val;
	}
	
	void setValue(V value){
		val = value;
	}
	
	Node* getNext(){
		return next;
	}
	
	Node* setNext(Node* node){
		next = node;
	}
}; 

// ====================== hashMap implementations ============================
template <typename K, typename V>
class HashMap {
private:
	static const int DEFAULT_CAPACITY = 10;
	static constexpr float DEFAUTL_LOAD_FACTOR= 0.75;
	
	vector<Node<K, V>*> array;
	int size;
	int capacity;
	
public:
	unsigned int hash(K key, int tableSize){
		return std::hash<K>()(key) % tableSize;
	}
	
	HashMap(){
		array.resize(DEFAULT_CAPACITY, NULL);
		capacity = DEFAULT_CAPACITY;
		size = 0;
	}	
	
	int getSize(){
		return size;
	}
	
	V get(const K& key){
		int idx = hash(key, capacity);
		Node<K, V>* curr = array[idx];
		while(curr){
			if (curr->getKey() == key)
				return curr->getValue();
			curr = curr->getNext();
		}
		return V();
	}	
	
	bool putIntoArray(const K& key, const V& val, int capacity, vector<Node<K, V>*>& vec){
		int idx = hash(key, capacity);

		//Node<K, V>* prev = NULL;
		Node<K, V>* curr = vec[idx];
		
		while(curr && curr->getKey() != key){
			//prev = curr;
			curr = curr->getNext();
		}
		
		if (curr != NULL){
			curr->setValue(val);
			return false;
		}
		else{ // put to the head of vec[idx] could introduce fewer corner cases
			Node<K, V>* node = new Node<K, V>(key, val);
			node->setNext(vec[idx]);
			vec[idx] = node;
			return true;
		}
		/*
		else if (prev == NULL){
			vec[idx] = new Node<K, V>(key, val);
			return true;
		}
		else{
			prev->setNext(new Node<K, V>(key, val));
			return true;
		}*/		
	}
	
	
	void put(const K& key, const V& val){
		if (putIntoArray(key, val, capacity, array))
			size++;
		
		if (needRehash())
			rehash();
			
		return;
	}
	
	bool remove(const K& key){
		int idx = hash(key, capacity);
			
		Node<K, V>* prev = NULL;
		Node<K, V>* curr = array[idx];
		
		while(curr && curr->getKey() != key){
			prev = curr;
			curr = curr->getNext();
		}	
		
		if (curr == NULL)
			return false;
		
		if (prev == NULL)
			array[idx] = curr->getNext();
		else
			prev->setNext(curr->getNext()); 
		
		delete curr;
		size--;
		return true;
	}
	
	bool needRehash(){
		return size / capacity > DEFAUTL_LOAD_FACTOR;
	}
	
	void rehash(){
		//cout << "Rehashing: -------------" << endl;
		int newCapacity = 2 * capacity;
		vector<Node<K, V>*> newArray(newCapacity, NULL);
		
		for (int i = 0; i < array.size(); i++){			
			Node<K, V>* curr = array[i];
			Node<K, V>* prev = NULL;
			while(curr){
				K currKey = curr->getKey();
				V currVal = curr->getValue();
				putIntoArray(currKey, currVal, newCapacity, newArray);
				
				prev = curr;
				curr = curr->getNext();
				
				delete prev;
			}
		}
		
		array = newArray;
		capacity = newCapacity;
	}
};


// =================== TEST ==========================
int main(){
	HashMap<Coordinate, string> myMap;

	Coordinate coord1(1, 2);	
	Coordinate coord2(2, 3);
	Coordinate coord3(3, 4);
	Coordinate coord4(1, 2);
	
	Coordinate coord5(1, 20);	
	Coordinate coord6(2, 30);
	Coordinate coord7(30, 4);
	Coordinate coord8(6, 3);
	Coordinate coord9(300, 4);	
	Coordinate coord10(16, 3);
	Coordinate coord11(3, 4);
	Coordinate coord12(1, 24);	
	

	myMap.put(coord1, "folks");
	myMap.put(coord2, "NRA");
	myMap.put(coord3, "SBSB");
	cout << myMap.get(coord1) << " " << 
	        myMap.get(coord2) << " " << 
			myMap.get(coord3) << endl;
			
	myMap.put(coord4, "ctmd");
	cout << myMap.get(coord1) << " " << 
	        myMap.get(coord2) << " " << 
			myMap.get(coord3) << " " <<
			myMap.get(Coordinate(10, 20)) << endl;
	cout << myMap.getSize() << endl;
	
	myMap.put(coord5, "some");
	myMap.put(coord6, "random");
	myMap.put(coord7, "stuff");
	myMap.put(coord8, "to");
	myMap.put(coord9, "increase");
	myMap.put(coord10, "size");	
	myMap.put(coord11, "of");
	myMap.put(coord12, "hashMap");
	cout << myMap.get(coord5) << " " << 
	        myMap.get(coord6) << " " << 
			myMap.get(coord7) << " " <<
			myMap.get(Coordinate(6, 3)) << endl;
	cout << myMap.getSize() << endl;	
	
}



