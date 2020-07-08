#include <iostream>
#include <vector>
#include <stdexcept>

using namespace std;

class minHeap{
private:
	vector<int> array;

	//int leftChild(int index);
	//int rightChild(int index);
	//int parent(int index);

	void percolateUp(int index);
	void percolateDown(int index);
	void heapify();
public:
	minHeap();
	minHeap(const vector<int>& input);

	int size();
	bool empty();

	void push(int val);
	void pop();
	int peek();
	void update(int index, int newVal);

	void printHeap();
};

minHeap::minHeap(){

}

minHeap::minHeap(const vector<int>& input){
	if (input.empty())
		throw invalid_argument("input vector is empty for construction");
	this->array = input;
	heapify(); // will implement shortly
}

void minHeap::percolateUp(int index){
	while(index > 0){
		int parentIdx = (index - 1) / 2;
		if (array[index] < array[parentIdx]){
			swap(array[index], array[parentIdx]);
			index = parentIdx;
		}
		else
		break;
	}
}
	
void minHeap::percolateDown(int index){
	while(index <= size() / 2 - 1){
		int leftChildIdx = 2 * index + 1;
		int rightChildIdx = 2 * index + 2;
		int swapCandidateIdx = leftChildIdx;
		if (rightChildIdx <= array.size() - 1 && array[rightChildIdx] <= array[leftChildIdx])
			swapCandidateIdx = rightChildIdx;
		if (array[index] <= array[swapCandidateIdx])
			break;
		else{
			swap(array[index], array[swapCandidateIdx]);
			index = swapCandidateIdx;
		}
	}
}
	
void minHeap::heapify(){
	for (int i = size() / 2 - 1; i >= 0; i--)
		percolateDown(i);
}

int minHeap::size(){
	return array.size();
}
	
bool minHeap::empty(){
	return array.empty();
}


void minHeap::push(int val){
	array.push_back(val);
	percolateUp(size() - 1);
}
	
void minHeap::pop(){
	if (array.empty())
		throw invalid_argument("minHeap is empty!");
	array[0] = array.back();
	array.pop_back();
	percolateDown(0);
}

int minHeap::peek(){
	if (array.empty())
		throw invalid_argument("minHeap is empty!");
	return array[0];
}

void minHeap::update(int index, int newVal){
	if (index < 0 || index >= size())
		throw invalid_argument("invalid index");
	int oldVal = array[index];
	array[index] = newVal;
	if (newVal == oldVal) 
		return;
	else if (newVal > oldVal)
		percolateDown(index);
	else
		percolateUp(index);
}

void minHeap::printHeap(){
	for (int a : array)
		cout << a << " ";
cout << endl; 
}






int main(){
	vector<int> input{8,7,3,1,5,2,6,14,10};
	minHeap myMinHeap(input);
	myMinHeap.printHeap();
	
	myMinHeap.update(4, -2);
	myMinHeap.printHeap();
	
	myMinHeap.update(1, 9);
	myMinHeap.printHeap();
	
	myMinHeap.push(-2);
	myMinHeap.printHeap();
	myMinHeap.push(9);
	myMinHeap.printHeap();
	
	cout << myMinHeap.peek() << endl;
	
	while(myMinHeap.size() > 0){
		myMinHeap.pop();
		myMinHeap.printHeap();
	}
	
	return 0;
}





