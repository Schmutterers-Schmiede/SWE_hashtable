#include "hashtable.h"
#include <iostream>
#include <memory>

void print_header(string text){
	cout << "+";
	for(int i = 0; i < text.size() + 8; i++)
		cout << "-";
	cout << "+" << endl;

	cout << "|     " << text << "     |" << endl;

	cout << "+";
	for (int i = 0; i < text.size() + 8; i++)
		cout << "-";
	cout << "+" << endl << endl;
}

using namespace std;
int main(){
	shared_ptr<hashtable<int>> ht = make_shared<hashtable<int>>(10);
	

	print_header("SEPARATE CHAINING");



	return 0;
}

template<	typename V, typename H, typename C>
void test_insert(shared_ptr<hashtable<V,H,C>> ht, V& value) {
	cout << "Testing: insert" << endl;
	cout << "inserting: " << value << endl;
	ht->insert(value);
	cout << "contents of hashtable: " *ht << endl;	

}