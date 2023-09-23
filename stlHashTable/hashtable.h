#pragma once

template<typename V, typename H, typename C>
class hashtable {
public:
  virtual ~hashtable() {}

  virtual void insert(const V& value) = 0;
  virtual void erase(const V& value) = 0;
  virtual bool contains(const V& value) = 0;
  virtual void rehash(size_t new_n_buckets) = 0;
  virtual void clear() = 0;

  virtual double load_factor() const = 0;
  virtual size_t size() const = 0;
  virtual size_t capacity() const = 0;
  virtual bool empty() const = 0;
};

template<typename V, typename H, typename C>
class const_iterator_base {
public:
  virtual ~const_iterator_base() {}

  virtual bool hasNext() = 0;

  virtual bool operator==(const const_iterator_base& rhs) const = 0;
  virtual bool operator!=(const const_iterator_base& rhs) const = 0;

  virtual const V& operator*() const = 0;
  virtual const V* operator->() const = 0;

  virtual const_iterator_base& operator++() = 0;
  virtual const_iterator_base& operator--() = 0;
  virtual const_iterator_base operator++(int) = 0;
  virtual const_iterator_base operator--(int) = 0;
};


