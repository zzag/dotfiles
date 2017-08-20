#include <iostream>
#include <vector>


using std::cout;
using std::endl;
using std::vector;


template<typename Value>
class FenwickTree {
    public:
        FenwickTree(size_t size) {
            tree.resize(size + 1);

            for (Value &item : tree) {
                item = Value();
            }
        }

        void add(int index, Value value) {
            for (int i = index; i < tree.size(); i += -i & i) {
                tree[i] += value;
            }
        }

        Value sum(int to, int from = 0) {
            Value retVal = Value();

            while (to > 0) {
                retVal += tree[to];
                to -= -to & to;
            }

            if (from) {
                retVal -= sum(from);
            }

            return retVal;
        }

    private:
        vector<Value> tree;
};


int main() {
    auto tree = new FenwickTree<int>(10);
    cout << "Add [1] 10... ";
    tree->add(1, 10);
    cout << "done" << endl;
    cout << "Sum to 1: " << tree->sum(1) << endl;
    cout << "Sum to 10: " << tree->sum(10) << endl;
    cout << endl;

    cout << "Add [5] 18... ";
    tree->add(5, 18);
    cout << "done" << endl;
    cout << "Sum to 1: " << tree->sum(1) << endl;
    cout << "Sum to 5: " << tree->sum(5) << endl;
    cout << "Sum to 10: " << tree->sum(10) << endl;
    cout << endl;

    cout << "Sum from 4 to 10: " << tree->sum(10, 4) << endl;
    cout << endl;

    cout << "Update 5" << endl;
    tree->add(5, -10);
    cout << "Sum to 1: " << tree->sum(1) << endl;
    cout << "Sum to 5: " << tree->sum(5) << endl;
    cout << "Sum to 10: " << tree->sum(10) << endl;
    
    return 0;
}
