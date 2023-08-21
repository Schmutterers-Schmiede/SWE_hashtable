#include "hashtable.h"
#include <iostream>
#include <memory>
#include <string>
#include <iomanip>
using namespace std;

void print_header(string text){
	cout << "+";
	for(int i = 0; i < text.size() + 10; i++)
		cout << "-";
	cout << "+" << endl;

	cout << "|     " << text << "     |" << endl;

	cout << "+";
	for (int i = 0; i < text.size() + 10; i++)
		cout << "-";
	cout << "+" << endl << endl;
}


template<	typename V, typename H, typename C>
void test_queries(shared_ptr<hashtable<V, H, C>> ht, int expected_capacity, const V& test_value){
	cout << "====  Testing: queries  ====" << endl;
	cout << "size: " << ht->size() << endl;
	cout << "expected 0 \n" << endl;
	cout << "capacity: " << ht->capacity() << endl;
	cout << "expected " << expected_capacity << "\n" << endl;
	cout << "empty: " << ht->empty() << endl;
	cout << "expected true" << endl;
	cout << "load factor: " << ht->load_factor() << endl;
	cout << "expected 0.1" << endl;
	
	cout << "\n" << "inserting " << test_value << "..." << endl;
	ht->insert(test_value);
	
	cout << "size: " << setw(14) << ht->size() << "expected 1" << endl;	
	cout << "empty: " << setw(14) << ht->empty() << "expected false" << endl;

	if(	ht->empty() &&
			ht->size() == 1 &&
			ht->capacity() == expected_capacity &&
			ht->load_factor() >= 0.09)
		cout << "SUCCESS" << endl;
	else 
		cout << "FAILED" << endl;

	ht->clear();
}

template<	typename V, typename H, typename C>
void test_insert(shared_ptr<hashtable<V,H,C>> ht, const V& value) {
	cout << "====  Testing: insert (includes contains())  ====" << endl;
	cout << "inserting: " << value << endl;
	ht->insert(value);
	cout << "contents of hashtable: " << endl;
	cout << *ht << endl;	
	if(ht->contains(value)) cout << "SUCCESS" << endl;
	else cout << "FAILED" << endl;
	ht->clear();
}

template<	typename V, typename H, typename C>
void test_clear(shared_ptr<hashtable<V, H, C>> ht, const V* test_values){
	cout << "====  Testing: insert (includes contains())  ====" << endl;
	cout << "inserting values..." << endl;
	for(int i = 0; i < sizeof(test_values)/sizeof(int); i++){
		ht->insert(test_values[i]);
	}	
	cout << "contents of hashtable: " << endl;
	cout << *ht << endl;
	cout << "clearing..." << endl;
	ht->clear();
	if(ht->empty()) cout << "SUCCESS" << endl;
	else cout << "FAILED";
}

template<	typename V, typename H, typename C>
void test_erase(shared_ptr<hashtable<V,H,C>> ht, const V& value1, const V& value2, const V& value3){
	cout << "====  Testing: erase (includes contains()) ====" << endl;
	cout << "inserting: " << value1 <<", " << value2 << ", " << value3 << endl;
	ht->insert(value1);
	ht->insert(value2);
	ht->insert(value3);
	cout << "contents of hashtable before erasing: " << endl;
	cout << *ht << endl;
	cout << "erasing first value..." << endl;
	ht->erase(value1);
	cout << "contents of hashtable after erasing: " << endl;
	cout << *ht << endl;
	if(!ht->contains(value1)) cout << "SUCCESS" << endl;
	else cout << "FAILED" << endl;
	ht->clear();
}

template<	typename V, typename H, typename C>
void test_rehash(shared_ptr<hashtable<V, H, C>> ht, V* test_values){	
	cout << "====  Testing: rehash ====" << endl;
	cout << "capacity before rehash: " << ht->capacity() << endl;
	cout << "inserting vales..." << endl;

	for(int i = 0; i < sizeof(test_values)/sizeof(int); i++){
		ht->insert(test_values[i]);
	}

	cout << "capacity after rehash: " << ht->capacity() << endl;
	if(ht->capacity() == 20) 
		cout << "SUCCESS" << endl;
	else 
		cout << "FAILED" << endl;
	ht->clear();
}

int main() {
	shared_ptr<hashtable<int>> ht = make_shared<hashtable<int>>(10);
	int rehash_test_values[10] = {0,1,2,3,4,5,6,7,8,9};
	int clear_test_values[5] = {1,2,3,4,5};
	print_header("SEPARATE CHAINING");
	test_insert(ht, 5);
	test_clear(ht, clear_test_values);
	test_queries(ht, 10, 4);
	test_erase(ht, 5, 6, 7);
	test_rehash(ht, rehash_test_values);


	return 0;
}