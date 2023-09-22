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
	cout << "====  Test case: queries  ====" << endl;
	cout << "size: " << ht->size() << endl;
	cout << "expected 0 \n" << endl;
	cout << "capacity: " << ht->capacity() << endl;
	cout << "expected " << expected_capacity << "\n" << endl;
	cout << "empty: " << ht->empty() << endl;
	cout << "expected true\n" << endl;
	cout << "load factor: " << ht->load_factor() << endl;
	cout << "expected 0\n" << endl;
	
	cout << "\n" << "inserting " << test_value << "..." << endl;
	ht->insert(test_value);
	
	cout << "size: " << ht->size() << endl;
	cout << "expected 1\n" << endl;	
	cout << "empty: " << ht->empty() << endl;
	cout << "expected false\n" << endl;

	if(	!ht->empty() &&
		ht->size() == 1 &&
		ht->capacity() == expected_capacity &&
		ht->load_factor() >= 0.09)
		cout << "SUCCESS\n" << endl;
	else 
		cout << "FAILED\n" << endl;

	ht->clear();
}

template<	typename V, typename H, typename C>
void test_insert(shared_ptr<hashtable<V,H,C>> ht, const V& value) {
	cout << "====  Test case: insert a vlaue (includes contains())  ====" << endl;
	cout << "inserting: " << value << endl;
	ht->insert(value);
	cout << "contents of hashtable: " << endl;
	cout << *ht << endl;	
	if(ht->contains(value)) cout << "SUCCESS\n" << endl;
	else cout << "FAILED\n" << endl;
	ht->clear();
}

template<	typename V, typename H, typename C>
void test_insert_duplicates(shared_ptr<hashtable<V, H, C>> ht, const V& value){
	cout << "==== Test case: insert duplicate values ====" << endl;
	cout << "inserting " << value << " 5 times..." << endl;
	for(int i = 0; i < 5; i++){
		ht->insert(value);
	}
	cout << "contets of hashtable:" << endl;
	cout << *ht << endl;
	ht->erase(5);
	if(!ht->contains(5)) 
		cout << "SUCCESS\n" << endl;
	else 
		cout << "FAILED\n" << endl;
	ht->clear();
}

template<	typename V, typename H, typename C>
void test_clear(shared_ptr<hashtable<V, H, C>> ht, const V* test_values){
	cout << "====  Test case: clear the hashtable (includes empty())  ====" << endl;
	cout << "inserting values..." << endl;
	for(int i = 0; i < 5; i++){
		ht->insert(test_values[i]);
	}	
	cout << "contents of hashtable: " << endl;
	cout << *ht << endl;
	cout << "clearing...\n" << endl;
	ht->clear();
	if(ht->empty()) cout << "SUCCESS\n" << endl;
	else cout << "FAILED\n";
}

template<	typename V, typename H, typename C>
void test_erase(shared_ptr<hashtable<V,H,C>> ht, const V& value1, const V& value2, const V& value3){
	cout << "====  Test case: erasing a value (includes contains()) ====" << endl;
	cout << "inserting: " << value1 << ", " << value2 << ", " << value3 << endl;
	ht->insert(value1);
	ht->insert(value2);
	ht->insert(value3);
	cout << "contents of hashtable before erasing: " << endl;
	cout << *ht << endl;
	cout << "erasing first value..." << endl;
	ht->erase(value1);
	cout << "contents of hashtable after erasing: " << endl;
	cout << *ht << endl;

	if(!ht->contains(value1)) 
		cout << "SUCCESS\n" << endl;
	else 
		cout << "FAILED\n" << endl;

	ht->clear();
}

template<	typename V, typename H, typename C>
void test_erase_nonexistent_value(shared_ptr<hashtable<V, H, C>> ht, const V& value1, const V& value2, const V& value3){
	if(value1 == value2 || value1 == value3 || value2 == value3){
		cout << "invalid test values." << endl;
	}
	else{
		cout << "====  Test case: erasing a nonexistent value (includes contains()) ====" << endl;
		cout << "inserting: " << value1 << " and " << value2 << endl;
		ht->insert(value1);
		ht->insert(value2);

		cout << "trying to erase " << value3 << "..." << endl;

		cout << "contents of hashtable before erasing: " << endl;
		cout << *ht << endl;

		ht->erase(value3);

		cout << "contents of hashtable after erasing: " << endl;
		cout << *ht << endl;

		bool test_success = true;

		if(!ht->contains(value1) || !ht->contains(value2))
			test_success = false;

		ht->erase(value1);
		ht->erase(value2);
		if(!ht->empty())
			test_success = false;

		if(test_success)
			cout << "SUCCESS\n" << endl;
		else
			cout << "FAILED\n" << endl;
	}
}

template<	typename V, typename H, typename C>
void test_rehash(shared_ptr<hashtable<V, H, C>> ht, V* test_values, int test_values_size)	{	
	cout << "====  Test case: overload triggering rehash ====" << endl;
	cout << "capacity before rehash: " << ht->capacity() << endl;
	cout << "inserting values..." << endl;

	for(int i = 0; i < test_values_size; i++){
		ht->insert(test_values[i]);
	}

	cout << "capacity after rehash: " << ht->capacity() << endl;
	cout << "size: " << ht->size() << endl << endl;
	if(ht->capacity() == 20) 
		cout << "SUCCESS\n" << endl;
	else 
		cout << "FAILED\n" << endl;
	ht->clear();
}

template<	typename V, typename H, typename C>
void test_iterator(shared_ptr<hashtable<V, H, C>> ht, V* test_values, int test_values_size){
	cout << "====  test case: iterate and dereferencing using iterator ====" << endl;
	cout << "inserting test values: ";

	for(int i = 0; i < test_values_size; ++i)
		cout << test_values[i] << " ";
	cout << endl;

	for(int i = 0; i < test_values_size; i++){
		ht->insert(test_values[i]);
	}	

	cout << "Operator ++ post increment:";
	auto iter1 = ht->begin();
	while(iter1 != ht->end()){		
		cout << *iter1 << " ";
		iter1++;		
	}		

	cout << endl;

	cout << "Operator ++ pre increment:";
	auto iter2 = ht->begin();
	while (iter2 != ht->end()) {
		cout << *iter2 << " ";
		++iter2;
	}

	cout << endl;

	cout << "Operator -- post decrement:";
	auto iter3 = ht->end();
	while (iter3 != ht->begin()) {
		iter3--;
		cout << *iter3 << " ";
	}

	cout << endl;

	cout << "Operator -- pre decrement:";
	auto iter4 = ht->end();
	while (iter4 != ht->begin()) {
		--iter4;
		cout << *iter4 << " ";
	}
	cout << endl;
	
	ht->clear();
}

template<typename V, typename H, typename C>
void test_equal_operator(shared_ptr<hashtable<V,H,C>> ht1, shared_ptr<hashtable<V, H, C>> ht2, V* test_values, int test_values_size){
	cout << "====  test case: comparing two hashtables using the == operator ====" << endl;
	cout << "inserting values [";
	for(int i = 0; i < test_values_size; i++){
		cout << test_values[i];
		if(i < 4) cout <<", ";
	}
	cout << "] into both hashtables..." << endl;

	for (int i = 0; i < test_values_size; i++) {
		ht1->insert(test_values[i]);
		ht2->insert(test_values[i]);
	}

	cout << "comparing...\n" << endl;

	if(*ht1 == *ht2) 
		cout << "SUCCESS\n" << endl;
	else cout << "FAILED\n" << endl;


	ht1->clear();
	ht2->clear();
}

int main() {
	shared_ptr<hashtable<int>> ht = make_shared<hashtable<int>>(10);
	shared_ptr<hashtable<int>> ht2 = make_shared<hashtable<int>>(10);
	int rehash_test_values[10] = {0,1,2,3,4,5,6,7,8,9};
	int test_values[5] = {1,2,3,4,5};

	string test_values_string[5] = {"a", "b", "c", "d", "e"};
	shared_ptr<hashtable<string>> ht_string = make_shared<hashtable<string>>(10);
	print_header("SEPARATE CHAINING");
	{
		shared_ptr<hashtable<int>> ht = make_shared<hashtable<int>>(10);
		test_queries(ht, 10, 5);
	}
	{
		shared_ptr<hashtable<int>> ht = make_shared<hashtable<int>>(10);
		test_insert(ht, 5);
	}

	{
		shared_ptr<hashtable<int>> ht = make_shared<hashtable<int>>(10);
		test_insert_duplicates(ht, 42);
	}
	{
		int test_values[5] = { 1,2,3,4,5 };
		test_clear(ht, test_values);
	}
	{
		shared_ptr<hashtable<int>> ht = make_shared<hashtable<int>>(10);
		test_queries(ht, 10, 4);
	}
	{
		shared_ptr<hashtable<int>> ht = make_shared<hashtable<int>>(10);
		test_erase(ht, 5, 6, 7);
	}
	{
		shared_ptr<hashtable<int>> ht = make_shared<hashtable<int>>(10);
		test_erase_nonexistent_value(ht, 1,2,3);
	}
	{
		shared_ptr<hashtable<int>> ht = make_shared<hashtable<int>>(10);
		int rehash_test_values[10] = { 0,1,2,3,4,5,6,7,8,9 };
		test_rehash(ht, rehash_test_values, 10);
	}
	{
		shared_ptr<hashtable<int>> ht = make_shared<hashtable<int>>(10);
		int test_values[5] = { 1,2,3,4,5 };
		test_iterator(ht, test_values, 5);
	}
	{
		shared_ptr<hashtable<int>> ht = make_shared<hashtable<int>>(10);
		int test_values[5] = { 1,2,3,4,5 };
		test_equal_operator(ht, ht2, test_values, 5);
	}
	return 0;
}