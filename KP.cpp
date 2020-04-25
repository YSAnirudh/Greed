#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;
struct PriorityQueue {
	int capacity;
	int size;
	int* pos; //for decrease key
	pair<int, int>* keyVal; //stores(vertex no, key)
	PriorityQueue(int capacity) :capacity(capacity), size(0) {
		keyVal = new pair<int, int>[capacity];
		pos = new int[capacity];
	}
	bool isEmpty() {
		return size == 0;
	}
	//heapify for min heap
	void heapify(int index) {
		int leastWeight = index;
		int l = 2 * index + 1;
		int r = 2 * index + 2;

		// If left child's key is smaller
		if (l < size && keyVal[l].second < keyVal[leastWeight].second)
			leastWeight = l;

		// If right child's key is smaller 
		if (r < size && keyVal[r].second < keyVal[leastWeight].second)
			leastWeight = r;

		// If smallest weight(key) is not root, swap
		if (leastWeight != index) {
			//swapping positions as swap changes vertex positions
			pos[keyVal[leastWeight].first] = index;
			pos[keyVal[index].first] = leastWeight;
			
			swap(keyVal[index], keyVal[leastWeight]);
			// heapify to maintain min heap
			heapify(leastWeight);

		}
	}
	void heapifyDec(int i) {
		// Find parent 
		int parent = (i - 1) / 2;

		if (keyVal[parent].second > 0) {
			//if key of i is less than parent, swap
			if (keyVal[i].second < keyVal[parent].second) {
				//swapping positions as swap changes vertex positions
				pos[keyVal[i].first] = parent;
				pos[keyVal[parent].first] = i;
				swap(keyVal[i], keyVal[parent]);

				// heapify to maintain min heap
				heapifyDec(parent);
			}
		}
	}
	// Builds Min Heap
	void minHeap() {
		for (int i = size / 2 - 1; i >= 0; i--)
			heapify(i);
	}
	int extractMinKey() {
		int ret = keyVal[0].first; //gets value of vertex with min key value

		//swap with position changes
		pos[keyVal[0].first] = size - 1;
		pos[keyVal[size - 1].first] = 0;
		swap(keyVal[0], keyVal[size - 1]);

		//size reduces as we are removing an element
		size--;

		// heapify to maintain min heap
		heapify(0);
		return ret;
	}
	void insertKey(int vertNo, int keyV) {
		if (size == capacity) return;
		//size increase as we are inserting an element
		size++;
		//adding (vertNo, key) pair to array
		keyVal[size - 1] = make_pair(vertNo, keyV);
		//heapify to maintain min heap
		heapifyDec(size - 1);
	}
	void decreaseKey(int vertNo, int newKey) {
		//get position of vertex
		int i = pos[vertNo];

		//go to that position and change the key value for vertNo
		keyVal[i] = make_pair(vertNo, newKey);
		//heapofy to maintain min heap
		heapify(i);
		return;
	}
	//checks if the element is in the minheap
	bool isInQueue(int v) {
		if (pos[v] < size) {
			return true;
		}
		return false;
	}
};
class Prim {
public:
	int* key; //key array
	bool* color; //color (W or B)
	int* pred;//stores vertex no attached behind to the present vertex
	vector<Edge>* adjList;//adjacency list to store adjacent nodes
	int sizeDyn;
	void addEdgeToList(int f, int s, int w) {
		//adding edge to adj list of first vertex of edge
		adjList[f].push_back(Edge(f, s, w));
		//adding edge to adj list of second vertex of edge
		adjList[s].push_back(Edge(s, f, w));
	}
	//input of edges pairs
	void makeEdgePairs() {
		int f, s, w;
		int edgesSize;
		cout << "No of edges in your graph: " << flush;
		cin >> edgesSize;
		for (int i = 0; i < edgesSize;) {
			cout << "Egde " << i << ": " << endl;
			cout << "	Start vertex: " << flush;
			cin >> f;
			cout << "	End vertex: " << flush;
			cin >> s;
			cout << "	Weight: " << flush;
			cin >> w;
			if (f < sizeDyn && s < sizeDyn) {
				addEdgeToList(f, s, w);
				i++;
			}
			else {
				cout << "Start and End vertex no.s shld be below: " << sizeDyn << endl;
			}
		}
	}
	void printResultMST() {
		int totalWeightMST = 0;
		cout << "MST: " << endl;
		for (int i = 1; i < sizeDyn; i++) {
			cout << "	Edge: " << pred[i] << " -> " << i << endl;
			cout << "	Weight: " << key[i] << endl;
			cout << endl;
			totalWeightMST += key[i];
		}
		cout << "Total MST Weight: " << totalWeightMST << endl;
	}
public:
	Prim() {}
	Prim(int V) {
		sizeDyn = V;
		key = new int[sizeDyn];
		color = new bool[sizeDyn];
		pred = new int[sizeDyn];
		adjList = new vector<Edge>[sizeDyn];
	}
	void PrimAlgo() {
		makeEdgePairs();

		//initialization of key andd color arrays
		for (int i = 0; i < sizeDyn; i++) {
			key[i] = INT_MAX;
			color[i] = false;//white
		}
		key[0] = 0;
		pred[0] = -1;//no vertex before root
		PriorityQueue queue(sizeDyn);
		//adding all vectors to priority queue
		for (int i = 0; i < sizeDyn; i++) {
			queue.insertKey(i, key[i]);
		}

		//if queue is not empty
		while (!queue.isEmpty()) {
			//extracting the least weighted element
			//and going through adjacency list of each vextex
			int u = queue.extractMinKey();
			for (auto edgeUV = adjList[u].begin(); edgeUV != adjList[u].end(); edgeUV++) {
				//the destination vertex of the edge
				int v = edgeUV->second;
				if ((queue.isInQueue(v)) && (color[v] == false) && (edgeUV->weight < key[v])) {
					//updating key array
					//decreasing key value
					//updating pred array
					key[v] = edgeUV->weight;
					queue.decreaseKey(v, key[v]);
					pred[v] = u;
				}
			}
			color[u] = true;//black
		}
		printResultMST();
	}
	void printAdjList() {
		for (int i = 0; i < sizeDyn; i++) {
			cout << "Vertex " << i << endl;
			for (auto it = adjList[i].begin(); it != adjList[i].end(); it++) {
				cout << "Edge " << it->first << " - " << it->second << endl;
			}
		}
	}
};

int main() {
	Prim p(5);
	p.PrimAlgo();

	system("pause");
	return 0;
}
