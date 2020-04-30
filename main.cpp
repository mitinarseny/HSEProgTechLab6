#include<iostream>

template<typename T>
class SP {
private:
	T* data;
	int* ref_count;

	void check_free() {
		if (--(*ref_count) == 0) {
			std::cout << "freeing " << *this << std::endl;
			delete data;
			data = nullptr;
			delete ref_count;
			ref_count = nullptr;
		}
	}
public:
	explicit SP(T* _data): data(_data), ref_count(new int(1)) {
		std::cout << "creating " << *this << std::endl;
	}
	
	SP(const SP& copy): data(copy.data), ref_count(copy.ref_count) {
		std::cout << "copying " << copy << " to " << *this << std::endl;
		if (ref_count != nullptr) {
			++(*ref_count);
		}
	}

	SP& operator=(const SP& copy) {
		std::cout << "copying " << copy << " to " << *this << std::endl;
		check_free();
		data = copy.data;
		ref_count = copy.ref_count;
		if (ref_count != nullptr) {
			(*ref_count)++;
		}
		return *this;
	}

	T& operator*() const { return *data; }
	T* operator->() const { return data; }

	~SP() {
		std::cout << "deleting " << *this << std::endl;
		check_free();
	}

	template<typename U>
	friend std::ostream& operator<<(std::ostream &os, SP<U> const& sp);
};

template<typename T>
std::ostream& operator<<(std::ostream &os, SP<T> const& sp) {
	return os << "SP(" << sp.data << ")";
}

class A {
public:
	int i;

	explicit A(int _i): i(_i) {
		std::cout << "creating " << *this << std::endl;
	}

	~A() {
		std::cout << "deleting " << *this << std::endl;
	}

	friend std::ostream& operator<<(std::ostream&, const A&);

	void greet() {
		std::cout << "Hi! I am " << *this << std::endl;
	}
};

std::ostream& operator<<(std::ostream& os, A const& a) {
	return os << "A(" << a.i << ")";
}

int main() {
	SP<A> a(new A(1));
	{
		SP<A> b(new A(2));
		b->greet();
	}
	{
		SP<A> c(new A(3));
		c->greet();
		a = c;
		c->greet();
		a->greet();
	}
	{
		SP<A> d(a);
		d->greet();
	}
	a->greet();
}
