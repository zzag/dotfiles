#include <iostream>
#include <vector>
#include <cmath>


using std::cout;
using std::endl;


struct SegmentTree {
  std::vector<int> data;
  int size;
};


void build(std::vector<int>& data, int node, int l, int r, SegmentTree& tree) {
  if (l == r) {
    tree.data[node] = data[l];
  }
  else {
    int m = (l + r) / 2;
    build(data, node*2+1, l, m, tree);
    build(data, node*2+2, m + 1, r, tree);
    tree.data[node] = tree.data[node*2+1] + tree.data[node*2+2];
  }
}


int sum(SegmentTree& tree, int node, int l, int r, int ql, int qr) {
  if (ql <= l && qr >= r) return tree.data[node];
  if (qr < l || r < ql) return 0;
  int m = (l + r) / 2;
  return sum(tree, node*2+1, l, m, ql, qr) +
         sum(tree, node*2+2, m+1, r, ql, qr);
}


void update(SegmentTree& tree, int node, int l, int r, int target, int value) {
  if (l == r) {
    tree.data[node] = value;
  }
  else {
    int m = (l + r) / 2;
    if (target <= m) update(tree, node*2+1, l, m, target, value);
    else update(tree, node*2+2, m+1, r, target, value);
    tree.data[node] = tree.data[node*2+1] + tree.data[node*2+2];
  }
}


SegmentTree buildSegmentTree(std::vector<int> a) {
  int n = 2*static_cast<int>(pow(2.0, ceil(log2(a.size())))) - 1;
  SegmentTree tree;
  tree.data.resize(n);
  tree.size = a.size();

  build(a, 0, 0, a.size() - 1, tree);

  return tree;
}


int sumSegmentTree(SegmentTree& tree, int l, int r) {
  return sum(tree, 0, 0, tree.size - 1, l, r);
}


void updateSegmentTree(SegmentTree& tree, int target, int value) {
  update(tree, 0, 0, tree.size - 1, target, value);
}


int main()
{
  std::vector<int> data = { 1, 3, 5, 7, 9, 11 };
  SegmentTree tree = buildSegmentTree(data);

  for (auto i : tree.data) {
    cout << i << endl;
  }

  cout << "Sum [2, 5] = " << sumSegmentTree(tree, 1, 4) << endl;

  updateSegmentTree(tree, 2, 6);

  for (auto i : tree.data) {
    cout << i << endl;
  }

  cout << "Sum [2, 5] = " << sumSegmentTree(tree, 1, 4) << endl;
  return 0;
}
