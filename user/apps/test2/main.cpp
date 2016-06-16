
class A {
public:
	A(int i) : i(i) {}

	static int a;
	static A* b;

	int i;
};

int A::a = 0;
A* A::b = new A(3);

int a = 1;

int main() {
	while(1) {

	}
	static int bss[8000];
	(void)bss;
	a = 0;
	return a;
}
