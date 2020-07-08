#include <iostream>
#include <vector>

using namespace std;

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


template <typename K, typename V>
class HashMap {
private:
	static const int DEFAULT_CAPACITY = 10;
	static constexpr float DEFAUTL_LOAD_FACTOR= 0.75;
	
	vector<Node<K, V>*> array;
	int size;
	int capacity;
	
public:
	unsigned int hash(const K& key, int tableSize){
		return reinterpret_cast<int>(key) % tableSize;
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

		Node<K, V>* prev = NULL;
		Node<K, V>* curr = vec[idx];
		
		while(curr && curr->getKey() != key){
			prev = curr;
			curr = curr->getNext();
		}
		
		if (curr != NULL){
			curr->setValue(val);
			return false;
		}
		else if (prev == NULL){
			vec[idx] = new Node<K, V>(key, val);
			return true;
		}
		else{
			prev->setNext(new Node<K, V>(key, val));
			return true;
		}		
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
		cout << "Rehashing: -------------" << endl;
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


int main(){
	HashMap<int, string> myMap;
	
	myMap.put(2, "folks");
	myMap.put(3, "NRA");
	myMap.put(4, "Jacobi");
	
	myMap.put(5, "lion");
	myMap.put(15, "lion15");
	myMap.put(25, "lion25");
	myMap.put(35, "lion35");
	cout << myMap.getSize() << endl;
	
	
	
	
	myMap.put(6, "wd");
	myMap.put(36, "ff");
	myMap.put(56, "some");
	myMap.put(16, "random");
	myMap.put(1, "things");
	myMap.put(0, "noav");
	cout << myMap.getSize() << endl;
	
	
	
	
	cout << myMap.get(5) << endl;
	cout << myMap.get(15) << endl;
	cout << myMap.get(25) << endl;
	cout << myMap.get(35) << endl;
	
	cout << myMap.get(6) << endl;
	cout << myMap.get(16) << endl;
	cout << myMap.get(7) << endl;
	cout << myMap.get(1) << endl;
	
	myMap.put(6, "wdc");
	myMap.put(16, "not that random");
	myMap.put(7, "ctmd nra");
	myMap.put(1, "things arefull");	
	cout << myMap.getSize() << endl;
	
	
	
	
	myMap.put(5, "lion-new");
	myMap.put(15, "lion15-new");
	myMap.put(25, "lion25-new");
	myMap.put(35, "lion35-new");
	myMap.put(2, "folks-new");
	myMap.put(3, "NRA-new");
	myMap.put(4, "Jacobi=new");

	cout << endl;
	cout << myMap.get(6) << endl;
	cout << myMap.get(16) << endl;
	cout << myMap.get(7) << endl;
	cout << myMap.get(1) << endl;

	cout << myMap.get(5) << endl;
	cout << myMap.get(15) << endl;
	cout << myMap.get(25) << endl;
	cout << myMap.get(35) << endl;	

	
	myMap.remove(5);
	myMap.remove(15);
	myMap.remove(25);
	myMap.remove(35);

	cout << myMap.get(5) << endl;
	cout << myMap.get(15) << endl;
	cout << myMap.get(25) << endl;
	cout << myMap.get(35) << endl;	
	cout << myMap.getSize() << endl;
}



