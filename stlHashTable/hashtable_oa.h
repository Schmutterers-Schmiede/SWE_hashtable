#pragma once
#include <algorithm>
#include <functional>
#include <iterator>
#include <list>
#include <ostream>
#include <vector>



template<	typename V,
	typename H = std::hash<V>,
	typename C = std::equal_to<V>>
	class hashtable_oa : hashtable<V,H,C>{//sc-> separate chaining
	
	public:

		hashtable_oa(size_t capacity) {			
			cap = capacity;
			data = std::vector<Entry>(cap);
			for(Entry item : data){
				item.state = 0;
			}
			count = 0;
		}

		~hashtable_oa() {}
		void insert(const V& value)override;
		void erase(const V& value)override;
		bool contains(const V& value)override;
		void rehash(size_t new_n_buckets)override;
		void clear()override;

		double load_factor() const override;
		size_t size() const override;
		size_t capacity() const override;
		bool empty() const override;

		friend std::ostream& operator<<(std::ostream& os, const hashtable_oa<V, H, C>& ht) {						
			for (const Entry item : ht.data) {
				if(item.state == 2){
					os << item.value << " ";
				}
			}
			os << "\n";
			
			return os;
		}

		class const_iterator;

		const_iterator begin() const {
			return const_iterator(&data, data.begin());
		}

		const_iterator end() const {
			return const_iterator(&data, data.end());
		}

		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = V const;
		using difference_type = std::ptrdiff_t;
		using const_pointer = V const*;
		using const_reference = V const&;

		typedef std::iterator<std::bidirectional_iterator_tag, value_type, difference_type, const_pointer,
			const_reference> iterator_base;


		//state = 0 -> empty
		//state = 1 -> deleted
		//state = 2 -> occupied
		struct Entry {
			V value;
			int state; 
		};

	private:
		std::vector<Entry> data;
		size_t count;
		size_t cap;
		int get_hash_index(const V& value) const;
		size_t indexOf(const V& value) const;
		
};

//=========  PUBLIC FUNCITONS  =============

template<	typename V, typename H, typename C>
void hashtable_oa<V, H, C>::insert(const V& value) {
	if (!contains(value)) {
		int index = get_hash_index(value);
		//find empty slot
		while(data.at(index).state == 2){
			index++;
			//reset if end is reached. Because rehash is triggered 
			//when the load factor is too high, there will always be a free slot, 
			//so there cannot be an endless loop
			if(index == cap)
				index = 0;
		}
		data.at(index).value = value;
		data.at(index).state = 2;

		count++;
		if (load_factor() > 0.75)
			rehash(cap * 2);
	}
}

template<	typename V, typename H, typename C>
void hashtable_oa<V, H, C>::erase(const V& value) {
	if (contains(value)) {
		size_t index = indexOf(value);
		data.at(index).state = 1;
		count--;
	}
}

template<	typename V, typename H, typename C>
bool hashtable_oa<V, H, C>::contains(const V& value) {
	return indexOf(value) != -1;
}

template<	typename V, typename H, typename C>
void hashtable_oa<V, H, C>::rehash(size_t new_n_buckets) {
	auto old_data = data;

	cap = new_n_buckets;
	data = std::vector<hashtable_oa::Entry>(new_n_buckets, { V{}, 0 });
	count = 0;

	for (hashtable_oa::Entry& item : old_data) {		
		if(item.state == 2){			
			insert(item.value);		
		}
	}
}

template<	typename V, typename H, typename C>
int hashtable_oa<V, H, C>::get_hash_index(const V& value) const{
	return H()(value) % (int)cap;
}

template<	typename V, typename H, typename C>
void hashtable_oa<V, H, C>::clear() {
	for (Entry item : data) {
		item.state = 0;
	}
	count = 0;
}

template<typename V, typename H, typename C>
double hashtable_oa<V, H, C>::load_factor() const {
	return (double)count / cap;
}

template<typename V, typename H, typename C>
size_t hashtable_oa<V, H, C>::size() const {
	return count;
}

template<typename V, typename H, typename C>
size_t hashtable_oa<V, H, C>::capacity() const {
	return cap;
}

template<typename V, typename H, typename C>
bool hashtable_oa<V, H, C>::empty() const {
	return count == 0;
}

template<typename V, typename H, typename C>
size_t hashtable_oa<V, H, C>::indexOf(const V& value) const {
	size_t index = 0;
	while (index < cap){
		if(data.at(index).state == 2 && C()(data.at(index).value, value)){
			return index;
		}
		index++;		
	}
	return -1;
}

//==========  DEFINITION OF ITERATOR CLASS ==============


template<typename V, typename H, typename C>
class hashtable_oa<V, H, C>::const_iterator : public iterator_base {

	private:
		typename std::vector<Entry>::const_iterator data_iterator; // Iterator of data vector for accessing entries
		typename const typename std::vector<Entry>* data_ptr; //pointer to data vector for comparisons

	public:

		const_iterator(	const typename std::vector<Entry>* data,
										const typename std::vector<Entry>::const_iterator data_it)
										: data_iterator(data_it), data_ptr(data) {		
			if(data_iterator == data_ptr->begin()){//begin() called -> find first element
				while(data_iterator != data_ptr->end() && data_iterator->state != 2 )
					data_iterator++;
			} 
			//no action needed for end()
		}

		typename std::vector<Entry>::const_iterator get_data_iterator() const {
			return data_iterator;
		}

		bool hasNext(){			
			auto data_iterator_copy = data_iterator;
			while(data_iterator_copy != data_ptr->end() && data_iterator_copy->state < 2){
				data_iterator_copy++;
			}
			return data_iterator_copy != data_ptr->end();
		}

		bool operator==(const_iterator const& rhs) const {
			return data_iterator == rhs.get_data_iterator();
		}
	
		bool operator!=(const_iterator const& rhs) const {
			return data_iterator != rhs.get_data_iterator();
		}	

		const_reference operator*() const {
			return data_iterator->value;
		}

		const_iterator& operator++() {
			if(data_iterator != data_ptr->end()){
				do{
					data_iterator++;
				} while(data_iterator != data_ptr->end() && data_iterator->state != 2);		
			}
			return *this;
		}

		const_iterator& operator--() {
			if (data_iterator != data_ptr->begin()) {
				do {
					data_iterator--;
				} while (data_iterator != data_ptr->begin() && data_iterator->state != 2);
			}			
			return *this;
		}

		const_iterator operator++(int) {
			const_iterator temp = *this; // Preserve state because of post-increment
			if (data_iterator != data_ptr->end()) {
				do {
					data_iterator++;
				} while (data_iterator != data_ptr->end() && data_iterator->state != 2);
			}
			return temp;
		}

		const_iterator operator--(int) {
			const_iterator temp = *this; //preserve state because post increment
			//update state
			if (data_iterator != data_ptr->begin()) {
				do {
					data_iterator--;
				} while (data_iterator != data_ptr->begin() && data_iterator->state != 2);
			}
			return temp;
		}
};

template<typename V, typename H, typename C>
bool operator==(const hashtable_oa<V, H, C> lhs, const hashtable_oa<V, H, C> rhs) {
	if (lhs.size() != rhs.size()) return false;

	auto iter_lhs = lhs.begin();
	auto iter_rhs = rhs.begin();

	while (iter_lhs != lhs.end()) {
		if (!(C()(*iter_lhs, *iter_rhs))) return false;
		iter_lhs++;
		iter_rhs++;
	}

	return true;
}