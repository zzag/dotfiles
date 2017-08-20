#include <iostream>
#include <vector>
#include <cmath>


using std::cout;
using std::endl;
using std::vector;


struct PriorityQueue {
  vector<int> data;
  int size;
};


void swap(int* a, int* b)
{
  int tmp = *a;
  *a = *b;
  *b = tmp;
}


bool compare(int a, int b)
{
  return a < b;
}


void bubbleDown(PriorityQueue& queue, int index)
{
  int leftChild = 2*index + 1;
  int rightChild = 2*index + 2;
  int minIndex = index;

  if (leftChild < queue.size && compare(queue.data[leftChild], queue.data[minIndex])) { minIndex = leftChild; }
  if (rightChild < queue.size && compare(queue.data[rightChild], queue.data[minIndex])) { minIndex = rightChild; }

  if (minIndex != index) {
    swap(&queue.data[index], &queue.data[minIndex]);
    bubbleDown(queue, minIndex);
  }
}


PriorityQueue makeHeap(vector<int>& data)
{
  PriorityQueue queue;
  int n = data.size();
  queue.size = n;
  queue.data.resize(static_cast<int>(pow(2, ceil(log2(n)))) - 1);

  for (auto i = 0; i < n; i++) {
    queue.data[i] = data[i];
  }

  for (auto i = n - 1; i >= 0; i--) {
    bubbleDown(queue, i);
  }

  return queue;
}


int extractMin(PriorityQueue& queue)
{
  if (queue.size <= 0) throw "noo-nooo...";
  int minValue = queue.data[0];
  queue.data[0] = queue.data[queue.size-1];
  queue.size--;
  bubbleDown(queue, 0);
  return minValue;
}


vector<int> heapsort(vector<int>& data)
{
  PriorityQueue queue = makeHeap(data);
  vector<int> sorted;
  sorted.resize(data.size());

  for (auto i = 0; i < data.size(); i++) {
    sorted[i] = extractMin(queue);
  }

  return sorted;
}


int main()
{
  vector<int> data = { 7, 4, 3, 0, 1, 2, -1, 0, -1 };
  vector<int> sorted = heapsort(data);

  for (auto i : data) cout << i << ' ';
  cout << endl;

  for (auto i : sorted) cout << i << ' ';
  cout << endl;

  return 0;
}