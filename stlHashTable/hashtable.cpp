//#include "hashtable.h"
//
//template<	typename V, typename H, typename C>
//void hashtable<V, H, C>::insert(const V& value) {
//	int index = get_hash_index(value);
//	count++;
//
//	if (data[index].empty())
//		data[index].push_back(value);
//
//	else
//		handle_collision(index, value);
//
//	if (load_factor() > 0.75) rehash(cap * 2);
//}
//
//template<	typename V, typename H, typename C>
//void hashtable<V, H, C>::erase(const V& value){
//	data[get_hash_index(value)].remove_if([](V& element){
//		return C()(element, value);
//	});
//}
//
//template<	typename V, typename H, typename C>
//bool hashtable<V, H, C>::contains(const V& value){
//	for(V item : data[get_hash_index(value)]){
//		if(C()(item, value)) return true;
//	}
//}
//
//template<	typename V, typename H, typename C>
//void hashtable<V, H, C>::rehash(size_t new_n_buckets){
//	cap = new_n_buckets;
//	std::vector<std::list<V>> new_data = std::vector<std::list<V>>(cap);
//	for(auto list : data){		
//			new_data[get_hash_index(list.begin())] = list;
//	}
//	data = new_data;
//}
//
//template<	typename V, typename H, typename C>
//void hashtable<V, H, C>::handle_collision(const int collision_index, const V& value){
//	data[collision_index].push_back(value);
//}
//
//template<	typename V, typename H, typename C>
//int hashtable<V, H, C>::get_hash_index(const V& value){
//	return H()(value) % cap;
//}
//
//
//
//template<typename V, typename H, typename C>
//double hashtable<V, H, C>::load_factor() const {
//	return size / cap;
//}
//
//template<typename V, typename H, typename C>
//size_t hashtable<V, H, C>::size() const {
//	return count;
//}
//
//template<typename V, typename H, typename C>
//size_t hashtable<V, H, C>::capacity() const {
//	return cap;
//}
//
//template<typename V, typename H, typename C>
//bool hashtable<V, H, C>::empty() const {
//	return isEmpty;
//}
//
//template<typename V, typename H, typename C>
//std::ostream& operator<<(std::ostream& os,
//	const hashtable<V, H, C>& ht){
//		for(auto list : hashtable::data){
//			for (V item : list){
//				os << item;
//			}
//		}
//	}