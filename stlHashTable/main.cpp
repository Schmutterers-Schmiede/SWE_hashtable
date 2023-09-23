#include "hashtable_sc.h"
#include "hashtable_oa.h"
#include <iostream>
#include <memory>
#include <string>
#include <iomanip>
using namespace std;

//custom functor for testing collision handling
template<typename T>
struct custom_hash {
	std::size_t operator()(const T& value) const {
		return 5;
	}
};

struct custom_int_compare{
	bool operator()(const int& v1, const int& v2) const {
		return v1 == v2;
	}
};

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


// SEPARATE CHAINING UNIT TESTS =====================================================================
template<	typename V, typename H, typename C>
void test_queries_sc(shared_ptr<hashtable_sc<V, H, C>> ht, int expected_capacity, const V& test_value){
	cout << "====  Test case: queries  ====\n" << endl;
	cout << "create a hashtable and test all queries \nbefore and after inserting a value." << endl;
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
void test_insert_sc(shared_ptr<hashtable_sc<V,H,C>> ht, const V& value) {
	cout << "====  Test case: insert a vlaue (includes contains())  ====\n" << endl;	
	cout << "inserting: " << value << endl;
	ht->insert(value);
	cout << "contents of hashtable: " << endl;
	cout << *ht << endl;	
	if(ht->contains(value)) cout << "SUCCESS\n" << endl;
	else cout << "FAILED\n" << endl;
	ht->clear();
}

template<	typename V, typename H, typename C>
void test_insert_duplicates_sc(shared_ptr<hashtable_sc<V, H, C>> ht, const V& value){
	cout << "==== Test case: insert duplicate values ====\n" << endl;
	cout << "hashtable should not accept duplicate \nvalues and not add anything." << endl;
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
void test_clear_sc(shared_ptr<hashtable_sc<V, H, C>> ht, const V* test_values){
	cout << "====  Test case: clear the hashtable (includes empty())  ====\n" << endl;
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
void test_erase_sc(shared_ptr<hashtable_sc<V,H,C>> ht, const V& value1, const V& value2, const V& value3){
	cout << "====  Test case: erasing a value (includes contains()) ====\n" << endl;
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
void test_erase_nonexistent_value_sc(shared_ptr<hashtable_sc<V, H, C>> ht, const V& value1, const V& value2, const V& value3){
	if(value1 == value2 || value1 == value3 || value2 == value3){
		cout << "invalid test values." << endl;
	}
	else{
		cout << "====  Test case: erasing a nonexistent value (includes contains()) ====\n" << endl;
		cout << "when the value is not found the hashtable should do nothing." << endl;
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
void test_rehash_sc(shared_ptr<hashtable_sc<V, H, C>> ht, V* test_values, int test_values_size)	{
	cout << "====  Test case: overload triggering rehash ====\n" << endl;
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
void test_iterator_sc(shared_ptr<hashtable_sc<V, H, C>> ht, V* test_values, int test_values_size){
	cout << "====  test case: iterating and dereferencing using iterator ====\n" << endl;
	cout << "inserting test values: ";

	for(int i = 0; i < test_values_size; ++i)
		cout << test_values[i] << " ";
	cout << endl;

	for(int i = 0; i < test_values_size; i++){
		ht->insert(test_values[i]);
	}	

	cout << "Operator ++ post increment: ";
	auto iter1 = ht->begin();
	while(iter1 != ht->end()){		
		cout << *iter1 << " ";
		iter1++;		
	}		

	cout << endl;

	cout << "Operator ++ pre increment: ";
	auto iter2 = ht->begin();
	while (iter2 != ht->end()) {
		cout << *iter2 << " ";
		++iter2;
	}

	cout << endl;

	cout << "Operator -- post decrement: ";
	auto iter3 = ht->end();
	while (iter3 != ht->begin()) {
		iter3--;
		cout << *iter3 << " ";
	}

	cout << endl;

	cout << "Operator -- pre decrement: ";
	auto iter4 = ht->end();
	while (iter4 != ht->begin()) {
		--iter4;
		cout << *iter4 << " ";
	}
	cout << endl << endl;
	
	ht->clear();
}

template<typename V, typename H, typename C>
void test_equal_operator_sc(shared_ptr<hashtable_sc<V,H,C>> ht1, shared_ptr<hashtable_sc<V, H, C>> ht2, V* test_values, int test_values_size){
	cout << "====  test case: comparing two hashtables using the == operator ====\n" << endl;
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

void test_custom_hash_functor_sc(){
	cout << "====  test case: creating a hashtable with a custom hash functor and inserting a value ====\n" << endl;	
	shared_ptr<hashtable_sc<int, custom_hash<int>>> ht = make_shared<hashtable_sc<int, custom_hash<int>>>(10);
	cout << "inserting 5...\n" << endl;
	ht->insert(5);	
	if(ht->size() == 1){
		cout << "SUCCESS\n" << endl;
	} else {
		cout << "FAILED\n" << endl;
	}
}

void test_collision_handling_sc(){
	cout << "====  test case: causing a collision with a custom hash functor ====\n" << endl;
	cout << "the hashtable shoudl handle the collision and add the second value." << endl;
	cout << endl;
	shared_ptr<hashtable_sc<int, custom_hash<int>>> ht = make_shared<hashtable_sc<int, custom_hash<int>>>(10);
	cout << "inserting a value..." << endl;
	ht->insert(5);
	ht->insert(6);
	if(!(ht->size() == 2)){
		cout << "SUCCESS\n" << endl;
	} else{
		cout << "FAILED\n" << endl;
	}
	
}

void test_custom_comp_functor_sc(){
	cout << "====  test case: inserting a value into a hashtable with a custom compare functor ====\n" << endl;
	cout << "The hashtable should add distinct values and \nreject already contained values\n" << endl;
	shared_ptr<hashtable_sc<int, custom_hash<int>>> ht = make_shared<hashtable_sc<int, custom_hash<int>>>(10);
	cout << "inserting 5 and 6..." << endl;
	ht->insert(5);
	ht->insert(6);
	cout << "inserting 5 again...\n" << endl;
	ht->insert(5);
	cout << "expected size: 2" << endl;
	cout << "size: " << ht->size() << endl << endl;

	if(ht->size() == 2){
		cout << "SUCCESS\n" << endl;
	}else{
		cout << "FAILED\n" << endl;
	}


}


// OPEN ADDRESSING UNIT TESTS ============================================================================

template<	typename V, typename H, typename C>
void test_queries_oa(shared_ptr<hashtable_oa<V, H, C>> ht, int expected_capacity, const V& test_value) {
	cout << "====  Test case: queries  ====\n" << endl;
	cout << "create a hashtable and test all queries \nbefore and after inserting a value." << endl;
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

	if (!ht->empty() &&
		ht->size() == 1 &&
		ht->capacity() == expected_capacity &&
		ht->load_factor() >= 0.09)
		cout << "SUCCESS\n" << endl;
	else
		cout << "FAILED\n" << endl;

	ht->clear();
}

template<	typename V, typename H, typename C>
void test_insert_oa(shared_ptr<hashtable_oa<V, H, C>> ht, const V& value) {
	cout << "====  Test case: insert a vlaue (includes contains())  ====\n" << endl;
	cout << "inserting: " << value << endl;
	ht->insert(value);
	cout << "contents of hashtable: " << endl;
	cout << *ht << endl;
	if (ht->contains(value)) cout << "SUCCESS\n" << endl;
	else cout << "FAILED\n" << endl;
	ht->clear();
}

template<	typename V, typename H, typename C>
void test_insert_duplicates_oa(shared_ptr<hashtable_oa<V, H, C>> ht, const V& value) {
	cout << "==== Test case: insert duplicate values ====\n" << endl;
	cout << "hashtable should not accept duplicate \nvalues and not add anything." << endl;
	cout << "inserting " << value << " 5 times..." << endl;
	for (int i = 0; i < 5; i++) {
		ht->insert(value);
	}
	cout << "contets of hashtable:" << endl;
	cout << *ht << endl;
	ht->erase(5);
	if (!ht->contains(5))
		cout << "SUCCESS\n" << endl;
	else
		cout << "FAILED\n" << endl;
	ht->clear();
}

template<	typename V, typename H, typename C>
void test_clear_oa(shared_ptr<hashtable_oa<V, H, C>> ht, const V* test_values) {
	cout << "====  Test case: clear the hashtable (includes empty())  ====\n" << endl;
	cout << "inserting values..." << endl;
	for (int i = 0; i < 5; i++) {
		ht->insert(test_values[i]);
	}
	cout << "contents of hashtable: " << endl;
	cout << *ht << endl;
	cout << "clearing...\n" << endl;
	ht->clear();
	if (ht->empty()) cout << "SUCCESS\n" << endl;
	else cout << "FAILED\n";
}

template<	typename V, typename H, typename C>
void test_erase_oa(shared_ptr<hashtable_oa<V, H, C>> ht, const V& value1, const V& value2, const V& value3) {
	cout << "====  Test case: erasing a value (includes contains()) ====\n" << endl;
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

	if (!ht->contains(value1))
		cout << "SUCCESS\n" << endl;
	else
		cout << "FAILED\n" << endl;

	ht->clear();
}

template<	typename V, typename H, typename C>
void test_erase_nonexistent_value_oa(shared_ptr<hashtable_oa<V, H, C>> ht, const V& value1, const V& value2, const V& value3) {
	if (value1 == value2 || value1 == value3 || value2 == value3) {
		cout << "invalid test values." << endl;
	}
	else {
		cout << "====  Test case: erasing a nonexistent value (includes contains()) ====\n" << endl;
		cout << "when the value is not found the hashtable should do nothing." << endl;
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

		if (!ht->contains(value1) || !ht->contains(value2))
			test_success = false;

		ht->erase(value1);
		ht->erase(value2);
		if (!ht->empty())
			test_success = false;

		if (test_success)
			cout << "SUCCESS\n" << endl;
		else
			cout << "FAILED\n" << endl;
	}
}

template<	typename V, typename H, typename C>
void test_rehash_oa(shared_ptr<hashtable_oa<V, H, C>> ht, V* test_values, int test_values_size) {
	cout << "====  Test case: overload triggering rehash ====\n" << endl;
	cout << "capacity before rehash: " << ht->capacity() << endl;
	cout << "inserting values..." << endl;

	for (int i = 0; i < test_values_size; i++) {
		ht->insert(test_values[i]);
	}

	cout << "capacity after rehash: " << ht->capacity() << endl;
	cout << "size: " << ht->size() << endl << endl;
	if (ht->capacity() == 20)
		cout << "SUCCESS\n" << endl;
	else
		cout << "FAILED\n" << endl;
	ht->clear();
}

template<	typename V, typename H, typename C>
void test_iterator_oa(shared_ptr<hashtable_oa<V, H, C>> ht, V* test_values, int test_values_size) {
	cout << "====  test case: iterating and dereferencing using iterator ====\n" << endl;
	cout << "inserting test values: ";

	for (int i = 0; i < test_values_size; ++i)
		cout << test_values[i] << " ";
	cout << endl;

	for (int i = 0; i < test_values_size; i++) {
		ht->insert(test_values[i]);
	}

	cout << "Operator ++ post increment: ";
	auto iter1 = ht->begin();
	while (iter1 != ht->end()) {
		cout << *iter1 << " ";
		iter1++;
	}

	cout << endl;

	cout << "Operator ++ pre increment: ";
	auto iter2 = ht->begin();
	while (iter2 != ht->end()) {
		cout << *iter2 << " ";
		++iter2;
	}

	cout << endl;

	cout << "Operator -- post decrement: ";
	auto iter3 = ht->end();
	while (iter3 != ht->begin()) {
		iter3--;
		cout << *iter3 << " ";
	}

	cout << endl;

	cout << "Operator -- pre decrement: ";
	auto iter4 = ht->end();
	while (iter4 != ht->begin()) {
		--iter4;
		cout << *iter4 << " ";
	}
	cout << endl << endl;

	ht->clear();
}

template<typename V, typename H, typename C>
void test_equal_operator_oa(shared_ptr<hashtable_oa<V, H, C>> ht1, shared_ptr<hashtable_oa<V, H, C>> ht2, V* test_values, int test_values_size) {
	cout << "====  test case: comparing two hashtables using the == operator ====\n" << endl;
	cout << "inserting values [";
	for (int i = 0; i < test_values_size; i++) {
		cout << test_values[i];
		if (i < 4) cout << ", ";
	}
	cout << "] into both hashtables..." << endl;

	for (int i = 0; i < test_values_size; i++) {
		ht1->insert(test_values[i]);
		ht2->insert(test_values[i]);
	}

	cout << "comparing...\n" << endl;

	if (*ht1 == *ht2)
		cout << "SUCCESS\n" << endl;
	else cout << "FAILED\n" << endl;


	ht1->clear();
	ht2->clear();
}

void test_custom_hash_functor_oa() {
	cout << "====  test case: creating a hashtable with a custom hash functor and inserting a value ====\n" << endl;
	shared_ptr<hashtable_oa<int, custom_hash<int>>> ht = make_shared<hashtable_oa<int, custom_hash<int>>>(10);
	cout << "inserting 5...\n" << endl;
	ht->insert(5);
	if (ht->size() == 1) {
		cout << "SUCCESS\n" << endl;
	}
	else {
		cout << "FAILED\n" << endl;
	}
}

void test_collision_handling_oa() {
	cout << "====  test case: causing collisions with a custom hash functor ====\n" << endl;
	cout << endl;
	shared_ptr<hashtable_oa<int, custom_hash<int>>> ht = make_shared<hashtable_oa<int, custom_hash<int>>>(10);
	cout << "inserting a value..." << endl;
	if (!ht->empty()) {
		cout << "SUCCESS\n" << endl;
	}
	else {
		cout << "FAILED\n" << endl;
	}

}

void test_custom_comp_functor_oa() {
	cout << "====  test case: inserting a value into a hashtable with a custom compare functor ====\n" << endl;
	cout << "The hashtable should add distinct values and \nreject already contained values\n" << endl;
	shared_ptr<hashtable_oa<int, custom_hash<int>>> ht = make_shared<hashtable_oa<int, custom_hash<int>>>(10);
	cout << "inserting 5 and 6..." << endl;
	ht->insert(5);
	ht->insert(6);
	cout << "inserting 5 again...\n" << endl;
	ht->insert(5);
	cout << "expected size: 2" << endl;
	cout << "size: " << ht->size() << endl << endl;

	if(ht->size() == 2){
		cout << "SUCCESS\n" << endl;
	}else{
		cout << "FAILED\n" << endl;
	}

}

void test_skip_deleted_value_oa(){
	cout << "====  test case: searching for a value that caused a collision and is located after a deleted value ====\n" << endl;
	cout << "The hashtable should iterate past the deleted value and find the value it is looking for\n" << endl;

	//using custom hash functor to cause collisions
	shared_ptr<hashtable_oa<int, custom_hash<int>>> ht = make_shared<hashtable_oa<int, custom_hash<int>>>(10);
	
	cout << "adding values..." << endl;
	ht->insert(5);
	ht->insert(6);
	ht->insert(7);

	cout << "hashtable before deleting: " << *ht << endl;

	ht->erase(6);
	cout << "hashtable after deleting: " << *ht << endl;

	cout << "calling contains() to check if 7 exists and 6 does not..." << endl;

	if(ht->contains(7) && !ht->contains(6))
		cout << "SUCCESS\n" << endl;
	else
		cout << "FAILED\n" << endl;
}




int main() {		

	print_header("SEPARATE CHAINING");
	{
		shared_ptr<hashtable_sc<int>> ht = make_shared<hashtable_sc<int>>(10);
		test_queries_sc(ht, 10, 5);
	}
	{
		shared_ptr<hashtable_sc<int>> ht = make_shared<hashtable_sc<int>>(10);
		test_insert_sc(ht, 5);
	}

	{
		shared_ptr<hashtable_sc<int>> ht = make_shared<hashtable_sc<int>>(10);
		test_insert_duplicates_sc(ht, 42);
	}
	{
		shared_ptr<hashtable_sc<int>> ht = make_shared<hashtable_sc<int>>(10);
		int test_values[5] = { 1,2,3,4,5 };
		test_clear_sc(ht, test_values);
	}
	{
		shared_ptr<hashtable_sc<int>> ht = make_shared<hashtable_sc<int>>(10);
		test_queries_sc(ht, 10, 4);
	}
	{
		shared_ptr<hashtable_sc<int>> ht = make_shared<hashtable_sc<int>>(10);
		test_erase_sc(ht, 5, 6, 7);
	}
	{
		shared_ptr<hashtable_sc<int>> ht = make_shared<hashtable_sc<int>>(10);
		test_erase_nonexistent_value_sc(ht, 1,2,3);
	}
	{
		shared_ptr<hashtable_sc<int>> ht = make_shared<hashtable_sc<int>>(10);
		int rehash_test_values[10] = { 0,1,2,3,4,5,6,7,8,9 };
		test_rehash_sc(ht, rehash_test_values, 10);
	}
	{
		shared_ptr<hashtable_sc<int>> ht = make_shared<hashtable_sc<int>>(10);
		int test_values[5] = { 1,2,3,4,5 };
		test_iterator_sc(ht, test_values, 5);
	}
	{
		shared_ptr<hashtable_sc<int>> ht = make_shared<hashtable_sc<int>>(10);
		shared_ptr<hashtable_sc<int>> ht2 = make_shared<hashtable_sc<int>>(10);
		int test_values[5] = { 1,2,3,4,5 };
		test_equal_operator_sc(ht, ht2, test_values, 5);
	}
	test_custom_hash_functor_sc();
	test_custom_comp_functor_sc();



	//----------------------------------------------------------------------

	print_header("OPEN ADDRESSING");
	{
		shared_ptr<hashtable_oa<int>> ht = make_shared<hashtable_oa<int>>(10);
		test_queries_oa(ht, 10, 5);
	}
	{
		shared_ptr<hashtable_oa<int>> ht = make_shared<hashtable_oa<int>>(10);
		test_insert_oa(ht, 5);
	}

	{
		shared_ptr<hashtable_oa<int>> ht = make_shared<hashtable_oa<int>>(10);
		test_insert_duplicates_oa(ht, 42);
	}
	{
		shared_ptr<hashtable_oa<int>> ht = make_shared<hashtable_oa<int>>(10);
		int test_values[5] = { 1,2,3,4,5 };
		test_clear_oa(ht, test_values);
	}
	{
		shared_ptr<hashtable_oa<int>> ht = make_shared<hashtable_oa<int>>(10);
		test_erase_oa(ht, 5, 6, 7);
	}
	{
		shared_ptr<hashtable_oa<int>> ht = make_shared<hashtable_oa<int>>(10);
		test_erase_nonexistent_value_oa(ht, 1, 2, 3);
	}
	{
		shared_ptr<hashtable_oa<int>> ht = make_shared<hashtable_oa<int>>(10);
		int rehash_test_values[10] = { 0,1,2,3,4,5,6,7,8,9 };
		test_rehash_oa(ht, rehash_test_values, 10);
	}
	
	{
		shared_ptr<hashtable_oa<int>> ht = make_shared<hashtable_oa<int>>(10);
		int test_values[5] = { 1,2,3,4,5 };
		test_iterator_oa(ht, test_values, 5);
	}
	{
		shared_ptr<hashtable_oa<int>> ht = make_shared<hashtable_oa<int>>(10);
		shared_ptr<hashtable_oa<int>> ht2 = make_shared<hashtable_oa<int>>(10);
		int test_values[5] = { 1,2,3,4,5 };
		test_equal_operator_oa(ht, ht2, test_values, 5);
	}
	test_custom_hash_functor_oa();
	test_custom_comp_functor_oa();
	test_skip_deleted_value_oa();

	shared_ptr<hashtable_sc<int>> ht = make_shared<hashtable_sc<int>>(10);
	
	return 0;
}