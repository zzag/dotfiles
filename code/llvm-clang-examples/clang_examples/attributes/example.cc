#define OBJECT __attribute__((annotate("object")))
#define FUNCTION __attribute__((annotate("function")))
#define DEPRECATED __attribute__((deprecated))



FUNCTION
DEPRECATED
int
f() {
    return 0;
}


class OBJECT
Object {
};


DEPRECATED
int
deprecatedFunction() {
	return 123;
}