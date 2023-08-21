#pragma once
#include <functional>
#include <ostream>
#include <vector>
#include <list>
#include <algorithm>

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

		friend std::ostream& operator<<(std::ostream& os, const hashtable<V,H,C>& ht) {
			for (std::list<V> list : ht.data) {
				if(!list.empty()){
					for (V& item : list) {
						os << item << " ";
					}
					os << "\n";
				}
			}
			return os;
		}


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
	data.at(get_hash_index(value)).remove_if([value](V& element) {
		return C()(element, value);
	});
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
	cap = new_n_buckets;
	std::vector<std::list<V>> new_data = std::vector<std::list<V>>(cap);
	for (auto list : data) {
		new_data.at(get_hash_index(list.front())) = list;
	}
	data = new_data;
}

template<	typename V, typename H, typename C>
int hashtable<V, H, C>::get_hash_index(const V& value) {
	return H()(value) % (int)cap;
}

template<	typename V, typename H, typename C>
void hashtable<V,H,C>::clear(){
	for(std::list<V> l : data){
		l.clear();
	}
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
