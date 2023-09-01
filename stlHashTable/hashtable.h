#pragma once
#include <algorithm>
#include <functional>
#include <iterator>
#include <list>
#include <ostream>
#include <vector>
#include <iostream>




template<	typename V, 
					typename H = std::hash<V>, 
					typename C = std::equal_to<V>>
class hashtable {
	public: 	

		hashtable(size_t capacity){
			cap = capacity;
			count = 0;
			data = std::vector<std::list<V>>(cap);
		}

		~hashtable(){}
		void insert(const V& value); 
		void erase(const V& value); 
		bool contains(const V& value); 
		void rehash(size_t new_n_buckets);
		void clear();
	
		double load_factor() const; 
		size_t size() const; 
		size_t capacity() const; 
		bool empty() const;

		friend std::ostream& operator<<(std::ostream& os, const hashtable<V,H,C>& ht){
			for (const std::list<V>& list : ht.data) {
				if (!list.empty()) {
					for (const V& item : list) {
						os << item << " ";
					}
					os << "\n";
				}
			}
			return os;
		}

		class const_iterator;				

		const_iterator begin() const{
			auto list_iterator = data.begin();
			while(list_iterator->empty() && list_iterator != data.end()) {list_iterator++;}	//find first nonempty list						
			return const_iterator(data, list_iterator, list_iterator->begin());						
		}

		const_iterator end() const{
			// Create and return an iterator pointing beyond the end of the data
			auto last_list = data.end();
			while (last_list->empty() && last_list != data.begin()) {
				--last_list; // Adjust to the last non-empty list
			}
			return const_iterator(data, last_list, last_list->end());
		}

		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = V const;
		using difference_type = std::ptrdiff_t;
		using const_pointer = V const*;
		using const_reference = V const&;
		
		typedef std::iterator<std::bidirectional_iterator_tag, value_type, difference_type, const_pointer,
			const_reference> iterator_base;

	private:
		std::vector<std::list<V>> data; 
		size_t count;
		size_t cap;
		bool isEmpty = true;		
		int get_hash_index(const V& value);
};

//=========  PUBLIC FUNCITONS  =============

template<	typename V, typename H, typename C>
void hashtable<V, H, C>::insert(const V& value) {
	if(!contains(value)){
		int index = get_hash_index(value);
		data.at(index).push_back(value);	

		count++;
		if (load_factor() > 0.75) 
			rehash(cap * 2);
	}
}

template<	typename V, typename H, typename C>
void hashtable<V, H, C>::erase(const V& value) {
	if(contains(value)) {
		count--;
		data.at(get_hash_index(value)).remove_if([value](V& element) {
			return C()(element, value);
		});
	}
}

template<	typename V, typename H, typename C>
bool hashtable<V, H, C>::contains(const V& value) {
	int index = get_hash_index(value);
	if(data.at(index).empty()) return false; //list empty

	for (V item : data.at(index)) {		
		if (C()(item, value)) return true; // value found
	}
	return false; //value not found
}

template<	typename V, typename H, typename C>
void hashtable<V, H, C>::rehash(size_t new_n_buckets) {
	auto old_data = data;
	cap = new_n_buckets;
	data = std::vector<std::list<V>>(cap);
	for (auto list : old_data) {
		for(const V& item : list){
			data.at(get_hash_index(item)).push_back(item);
		}
	}	
}

template<	typename V, typename H, typename C>
int hashtable<V, H, C>::get_hash_index(const V& value) {
	return H()(value) % (int)cap;
}

template<	typename V, typename H, typename C>
void hashtable<V,H,C>::clear(){
	for(std::list<V>& l : data){
		l.clear();
	}
	count = 0;
}

//template<	typename V, typename H, typename C>
//hashtable<V, H, C>::const_iterator hashtable<V, H, C>::begin() const {	
//	return const_iterator(data, data.begin(), data.begin()->begin());
//}

//template<typename V, typename H, typename C>
//hashtable<V, H, C>::const_iterator hashtable<V, H, C>::end() const {
//	// Create and return an iterator pointing beyond the end of the data
//	auto last_list = data.end();
//	while(last_list->empty()){
//		--last_list; // Adjust to the last non-empty list
//	}
//	return const_iterator(data, last_list, last_list->end());
//}

//=========  OPERATORS  ===========

//template<typename V, typename H, typename C>
//std::ostream& operator<<(std::ostream& os, const hashtable<V, H, C>& ht) {
//	for (const std::list<V>& list : ht.data) {
//		if (!list.empty()) {
//			for (const V& item : list) {
//				os << item << " ";
//			}
//			os << "\n";
//		}
//	}
//	return os;
//}

template<typename V, typename H, typename C>
bool operator==(const hashtable<V, H, C>& lhs, const hashtable<V, H, C>& rhs) {
	if (lhs->size() == rhs->size()) {
		auto it_lhs = lhs->begin();
		auto it_rhs = rhs->begin();
		for (; it_lhs != lhs->end(); ++it_lhs, ++it_rhs) {
			if (*it_lhs != *it_rhs) return false;			
		}
		return true;
	}
	else return false;
}

//=========  PRIVATE FUNCITONS  =============

template<typename V, typename H, typename C>
double hashtable<V, H, C>::load_factor() const {
	return (double)count / cap;
}

template<typename V, typename H, typename C>
size_t hashtable<V, H, C>::size() const {
	return count;
}

template<typename V, typename H, typename C>
size_t hashtable<V, H, C>::capacity() const {
	return cap;
}

template<typename V, typename H, typename C>
bool hashtable<V, H, C>::empty() const {
	return isEmpty;
}



//==========  DEFINITION OF ITERATOR CLASS ==============


template<typename V, typename H, typename C>
class hashtable<V, H, C>::const_iterator : public iterator_base {

	private:
		const std::vector<std::list<V>>* data_ptr; // pointer to data vector of hashtable class
		typename std::vector<std::list<V>>::const_iterator list_iterator; // Iterator of data vector for accessing lists
		typename std::list<V>::const_iterator value_iterator; // iterator of list for accessing values

	public:
		
		const_iterator(	const std::vector<std::list<V>>& data,
										typename std::vector<std::list<V>>::const_iterator list_it,
										typename std::list<V>::const_iterator value_it)
										: data_ptr(&data), 
											list_iterator(list_it), 
											value_iterator(value_it) {std::cout << "constructor ";}				

		typename std::list<V>::const_iterator get_value_iterator() const {
			return value_iterator;
		}

		bool operator==(const_iterator const& rhs) const{
			std::cout << "== ";
			return (value_iterator == rhs.get_value_iterator());
		}

		bool operator!=(const_iterator const& rhs) const{
			std::cout << "!= ";
			return (value_iterator != rhs.get_value_iterator());
		}

		const_reference operator*() const{
			std::cout << "* ";
			return *value_iterator;
		}

		const_pointer operator->() const{
			std::cout << "-> ";
			return &(*value_iterator);
		}

		const_iterator& operator++(){		
			std::cout << "&++ ";
			//update state
			if (value_iterator == list_iterator->end() && list_iterator != data_ptr->end()) {
				do {
					++list_iterator;
				} while (list_iterator != data_ptr->end() && list_iterator->empty());
				if (list_iterator != data_ptr->end()) {
					value_iterator = list_iterator->begin();
				}
			}
			else {
				value_iterator++;
			}
			return *this;
		}

		const_iterator& operator--(){
			std::cout << "&-- ";
			//update state
			if (value_iterator == list_iterator->begin() && list_iterator != data_ptr->begin()) {
				do {
					--list_iterator;
				} while (list_iterator->empty() && list_iterator != data_ptr->end());
				value_iterator = list_iterator->end();
			}
			else {
				value_iterator--;
			}
			return *this;
		}

		const_iterator operator++(int){		
			std::cout << "++ ";
			const_iterator temp = *this; //preserve state because post increment
			//update state
			if(value_iterator == list_iterator->end() && list_iterator != data_ptr->end()) {
				do{
					++list_iterator;
				} while(list_iterator->empty() && list_iterator != data_ptr->end());
				if(list_iterator != data_ptr->end()){
					value_iterator = list_iterator->begin();
				}
			}
			else{
				value_iterator++;			
			}			
			return temp;
		}

		const_iterator operator--(int){
			std::cout << "-- ";
			const_iterator temp = *this; //preserve state because post increment
			//update state
			if (value_iterator == list_iterator->begin() && list_iterator != data_ptr->begin()) {
				do {
					--list_iterator;
				} while (list_iterator->empty() && list_iterator != data_ptr->end());
				value_iterator = list_iterator->end();
			}
			else {
				value_iterator--;
			}
			return temp;
		}
	
};



