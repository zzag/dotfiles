void f() {
    int index = 0;
    while (index < 1000) {
        index += 1;
    }
}

int magicFunction (int a, int b) {
    if (a > b) {
        return a;
    }
    else {
        return b;
    }
}

void foo(int& a) {
    a = 123;
}
