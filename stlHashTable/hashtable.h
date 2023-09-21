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
			return const_iterator(&data, data.begin());						
		}

		const_iterator end() const{						
			return const_iterator(&data, data.end());						
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
		typename std::vector<std::list<V>>::const_iterator data_iterator; // Iterator of data vector for accessing lists
		typename std::list<V>::const_iterator list_iterator; // iterator of list for accessing values
		typename const typename std::vector<std::list<V>>* data_ptr; //pointer to container for comparisons

	public:
		
		const_iterator(	const typename std::vector<std::list<V>>* data,
										const typename std::vector<std::list<V>>::const_iterator data_it)
										: data_iterator(data_it), data_ptr(data){												
												if(data_iterator == data_ptr->begin()){												
													//begin() called -> find first non-empty list or end
													while(data_iterator->empty() && data_iterator != data_ptr->end()){
														data_iterator++;
													}													
													if(data_iterator == data_ptr->end()){
														//container is empty -> go to end state
														data_iterator = data_ptr->end();
														data_iterator--;
														list_iterator = data_iterator->end();
														data_iterator++;
													}
													list_iterator = data_iterator->begin();													
												}else {
													// end() called -> list_iterator to end of end of last list
													data_iterator--;
													list_iterator = data_iterator->end();
													data_iterator++;
												}

											}

		typename std::vector<std::list<V>>::const_iterator get_data_iterator() const {
			return data_iterator;
		}

		typename std::list<V>::const_iterator get_list_iterator() const{
			return list_iterator;
		}		

		bool hasNext(){
			auto data_iterator_copy = data_iterator;
			auto list_iterator_copy = list_iterator;

			if(data_iterator_copy == data_ptr->end()){
				return false;
			}

			++list_iterator_copy; // Move to the next element in the current list

			if (list_iterator_copy == data_iterator_copy->end()) {
				// Iterator is at the end of the current list, check for the next non-empty list
				while (data_iterator_copy != data_ptr->end()) {
					++data_iterator_copy; // Move to the next list
					if (data_iterator_copy != data_ptr->end() && !data_iterator_copy->empty()) {
						// Found a non-empty list, reset the list_iterator_copy
						list_iterator_copy = data_iterator_copy->begin();
						break;
					}
				}
			}

			return data_iterator_copy != data_ptr->end() && list_iterator_copy != data_iterator_copy->end();
		}

		bool operator==(const_iterator const& rhs) const{			
			return (data_iterator == rhs.get_data_iterator() && list_iterator == rhs.get_list_iterator());
		}

		bool operator!=(const_iterator const& rhs) const{
			return data_iterator != rhs.get_data_iterator() || list_iterator != rhs.get_list_iterator();
		}

		const_reference operator*() const{		
			return *list_iterator;
		}

		const_pointer operator->() const{			
			return &(*list_iterator);
		}

		const_iterator& operator++(){					
			if (data_iterator != data_ptr->end()) {
				if (!hasNext()) {
					//end reached -> set to end state
					data_iterator = data_ptr->end();

					//set list iterator to end of last list
					data_iterator--;
					list_iterator = data_iterator->end();

					//go back to data_ptr->end
					data_iterator++;

					return *this;
				}
				//at this point there is definitely a next value
				list_iterator++;

				if (list_iterator == data_iterator->end()) {
					// end of list reached -> find next non-empty one
					do {
						++data_iterator;
					} while (data_iterator != data_ptr->end() && data_iterator->empty());
					// Found a non-empty list, reset value_iterator
					list_iterator = data_iterator->begin();
				}
			}
			return *this;
		}

		const_iterator& operator--(){			
			if (data_iterator == data_ptr->end()) {
				//iterator is at end position -> find last non-empty list in container
				do {
					data_iterator--;
				} while (data_iterator != data_ptr->begin() && data_iterator->empty());
				list_iterator = data_iterator->begin();
			}
			else if (list_iterator == data_iterator->begin()) {
				//iterator is at start of list (could also be begin) -> find next non-empty list or begin
				do {
					data_iterator--;
				} while (data_iterator != data_ptr->begin() && data_iterator->empty());
				list_iterator = data_iterator->end();
				list_iterator--;
			}
			else {
				//iterator must be at position > 0 in the list -> decrement within list
				list_iterator--;
			}
			return *this;
		}

		const_iterator operator++(int) {
		//std::cout << "++"
			const_iterator temp = *this; // Preserve state because of post-increment

			if (data_iterator != data_ptr->end()) {
				if(!hasNext()){
					//end reached -> set to end state
					data_iterator = data_ptr->end();

					//set list iterator to end of last list
					data_iterator--;
					list_iterator = data_iterator->end();

					//go back to data.end
					data_iterator++;

					return temp;
				}
				//at this point there is definitely a next value
				list_iterator++;

				if (list_iterator == data_iterator->end()) {
					// end of list reached -> find next non-empty one
					do {
						++data_iterator;
					} while (data_iterator != data_ptr->end() && data_iterator->empty());
					// Found a non-empty list, reset value_iterator
					list_iterator = data_iterator->begin();				
				}
			}			
			return temp;
		}

		const_iterator operator--(int){
			//std::cout << "-- ";
			const_iterator temp = *this; //preserve state because post increment
			//update state
			if (data_iterator == data_ptr->end()) {
				//iterator is at end position -> find last non-empty list in container
				do {
					data_iterator--;
				} while (data_iterator != data_ptr->begin() && data_iterator->empty());
				list_iterator = data_iterator->begin();
			}
			else if (list_iterator == data_iterator->begin()) {
				//iterator is at start of list (could also be begin) -> find next non-empty list or begin
				do{
					data_iterator--;
				} while (data_iterator != data_ptr->begin() && data_iterator->empty());
				list_iterator = data_iterator->end();
				list_iterator--;
			}
			else {
				//iterator must be at position > 0 in the list -> decrement within list
				list_iterator--;
			}
			return temp;
		}
	
};



