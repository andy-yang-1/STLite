#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>

namespace sjtu {
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector {
private:

    T **all_data = nullptr ;
    int length = 0 ; // 数组长度
    int up_bound = 0 ; // 数组上限
public:
    bool is_full() // 判满
    {
        if ( length == up_bound ) return true ;
        return false ;
    }

    void split() // 加倍
    {
        T **new_array ;
        new_array = new T* [2*up_bound] ;
        for ( int i = 0 ; i < up_bound ; i++ ){
            new_array[i] = all_data[i] ; // 转移指针，不转移内容
        }
        up_bound *= 2 ;
        delete [] all_data ; // todo 查询是否需要更改 delete 模式
        all_data = new_array ;
        new_array = nullptr ;
    }


	/**
	 * TODO
	 * a type for actions of the elements of a vector, and you should write
	 *   a class named const_iterator with same interfaces.
	 */
	/**
	 * you can see RandomAccessIterator at CppReference for help.
	 */
	class const_iterator;
	class iterator {
    public:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */

		int real_pos = 0 ; // 若为 length 则说明是 end
		vector<T> *real_pointer = nullptr ; // 指向vector

	public:

	    iterator() {}

	    iterator( int pos , vector<T> *p ) : real_pos(pos) , real_pointer(p) {}

	    iterator( const iterator &other ) : real_pos(other.real_pos) , real_pointer(other.real_pointer) {}

	    iterator &operator= ( iterator && other )
        {
	        if ( this == &other ) return *this ;
	        real_pos = other.real_pos ;
	        real_pointer = other.real_pointer ;
        }

		/**
		 * return a new iterator which pointer n-next elements
		 * as well as operator-
		 */
		iterator operator+(const int &n) const {
			//TODO
			if ( real_pos + n > real_pointer->length || real_pos + n < 0 ) throw runtime_error() ;
			iterator temp_iterator = *this ;
			temp_iterator.real_pos += n ;
			return temp_iterator ;
		}
		iterator operator-(const int &n) const {
			//TODO
            if ( real_pos - n > real_pointer->length || real_pos - n < 0 ) throw runtime_error() ;
            iterator temp_iterator = *this ;
            temp_iterator.real_pos -= n ;
            return temp_iterator ;
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const {
			//TODO
            if ( real_pointer != rhs.real_pointer ) throw invalid_iterator() ;
            return real_pos - rhs.real_pos ;
		}
		iterator& operator+=(const int &n) {
			//TODO
			if ( real_pos + n > real_pointer->length || real_pos + n < 0 ) throw runtime_error() ;
			real_pos += n ;
			return *this ;
		}
		iterator& operator-=(const int &n) {
			//TODO
			if ( real_pos - n > real_pointer->length || real_pos - n < 0 ) throw runtime_error() ;
			real_pos -= n ;
			return *this ;
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
		    if ( real_pos + 1 > real_pointer->length ) throw runtime_error() ;
		    iterator temp_iterator = *this ;
		    real_pos++ ;
		    return temp_iterator ;
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++() {
		    if ( real_pos + 1 > real_pointer->length ) throw runtime_error() ;
		    real_pos++ ;
		    return *this ;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
		    if ( real_pos - 1 < 0 ) throw runtime_error() ;
		    iterator temp_iterator = *this ;
		    real_pos-- ;
		    return temp_iterator ;
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--() {
		    if ( real_pos - 1 < 0 ) throw runtime_error() ;
		    real_pos-- ;
		    return *this ;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const{
		    return *(real_pointer->all_data[real_pos]) ; // 返回 T 元素而非指针
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const {
		    if ( this->real_pointer == rhs.real_pointer && real_pos == rhs.real_pos ) return true ;
		    return false ;
		}
		bool operator==(const const_iterator &rhs) const {
		    if ( this->real_pointer == rhs.real_pointer && real_pos == rhs.real_pos ) return true ;
		    return false ;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
		    if ( *this == rhs ) return false ;
		    return true ;
		}
		bool operator!=(const const_iterator &rhs) const {
		    if ( *this == rhs ) return false ;
		    return true ;
		}
	};
	/**
	 * TODO
	 * has same function as iterator, just for a const object.
	 */
	class const_iterator {

    public:
        /**
         * TODO add data members
         *   just add whatever you want.
         */

        int real_pos = 0 ; // 若为 length 则说明是 end
        const vector<T> *real_pointer = nullptr ; // 指向vector

    public:

        const_iterator() {}

        const_iterator( int pos , const vector<T> *p ) : real_pos(pos) , real_pointer(p) {}

        const_iterator( const const_iterator &other ) : real_pos(other.real_pos) , real_pointer(other.real_pointer) {}

//        const_iterator &operator= ( const_iterator && other )
//        {
//            if ( this == &other ) return *this ;
//            real_pos = other.real_pos ;
//            real_pointer = other.real_pointer ;
//        }

/**
 * return a new const_iterator which pointer n-next elements
 * as well as operator-
 */
        const_iterator operator+(const int &n) const {
            //TODO
            if ( real_pos + n > real_pointer->length || real_pos + n < 0 ) throw runtime_error() ;
            const_iterator temp_const_iterator = *this ;
            temp_const_iterator.real_pos += n ;
            return temp_const_iterator ;
        }
        const_iterator operator-(const int &n) const {
            //TODO
            if ( real_pos - n > real_pointer->length || real_pos - n < 0 ) throw runtime_error() ;
            const_iterator temp_const_iterator = *this ;
            temp_const_iterator.real_pos -= n ;
            return temp_const_iterator ;
        }
// return the distance between two const_iterators,
// if these two const_iterators point to different vectors, throw invaild_const_iterator.
        int operator-(const const_iterator &rhs) const {
            //TODO
            if ( real_pointer != rhs.real_pointer ) throw invalid_iterator() ;
            return real_pos - rhs.real_pos ;
        }
        const_iterator& operator+=(const int &n) {
            //TODO
            if ( real_pos + n > real_pointer->length || real_pos + n < 0 ) throw runtime_error() ;
            real_pos += n ;
            return *this ;
        }
        const_iterator& operator-=(const int &n) {
            //TODO
            if ( real_pos - n > real_pointer->length || real_pos - n < 0 ) throw runtime_error() ;
            real_pos -= n ;
            return *this ;
        }
/**
 * TODO iter++
 */
        const_iterator operator++(int) {
            if ( real_pos + 1 > real_pointer->length ) throw runtime_error() ;
            const_iterator temp_const_iterator = *this ;
            real_pos++ ;
            return temp_const_iterator ;
        }
/**
 * TODO ++iter
 */
        const_iterator& operator++() {
            if ( real_pos + 1 > real_pointer->length ) throw runtime_error() ;
            real_pos++ ;
            return *this ;
        }
/**
 * TODO iter--
 */
        const_iterator operator--(int) {
            if ( real_pos - 1 < 0 ) throw runtime_error() ;
            const_iterator temp_const_iterator = *this ;
            real_pos-- ;
            return temp_const_iterator ;
        }
/**
 * TODO --iter
 */
        const_iterator& operator--() {
            if ( real_pos - 1 < 0 ) throw runtime_error() ;
            real_pos-- ;
            return *this ;
        }
/**
 * TODO *it
 */
        const T& operator*() const{
            return *(real_pointer->all_data[real_pos]) ;
        }
/**
 * a operator to check whether two const_iterators are same (pointing to the same memory address).
 */
        bool operator==(const const_iterator &rhs) const {
            if ( this->real_pointer == rhs.real_pointer && real_pos == rhs.real_pos ) return true ;
            return false ;
        }
        bool operator==(const iterator &rhs) const {
            if ( this->real_pointer == rhs.real_pointer && real_pos == rhs.real_pos ) return true ;
            return false ;
        }
/**
 * some other operator for const_iterator.
 */
        bool operator!=(const const_iterator &rhs) const {
            if ( *this == rhs ) return false ;
            return true ;
        }
        bool operator!=(const iterator &rhs) const {
            if ( *this == rhs ) return false ;
            return true ;
        }


    };
	/**
	 * TODO Constructs
	 * Atleast two: default constructor, copy constructor
	 */
	vector() {
	    length = 0 ;
	    up_bound = 50 ;
	    all_data = new T* [50] ;
	    for ( int i = 0 ; i < up_bound ; i++ ){
	        all_data[i] = nullptr ;
	    }
	}
	vector(const vector &other) : length(other.length) , up_bound(other.up_bound) {
	    all_data = new T* [up_bound] ;
	    for ( int i = 0 ; i < length ; i++ ){
	        all_data[i] = new T(*(other.all_data[i])) ; // new 拷贝构造
	    }
	    for ( int i = length ; i < up_bound ; i++ ){
	        all_data[i] = nullptr ;
	    }
	}
	/**
	 * TODO Destructor
	 */
	~vector() {
	    for ( int i = 0 ; i < length ; i++ ){ // 防止内存泄漏，不考虑 delete 空指针
	        if ( all_data[i] != nullptr )
	            delete all_data[i] ;
	        all_data[i] = nullptr ;
	    }
	    delete [] all_data ;
	}
	/**
	 * TODO Assignment operator
	 */
	vector &operator=(const vector &other) {
	    if ( this == &other ) return *this ;
	    if ( all_data != nullptr ) this->clear() ;
	    length = other.length ;
	    up_bound = other.up_bound ;
	    all_data = new T* [up_bound] ;
	    for ( int i = 0 ; i < length ; i++ ){
	        all_data[i] = new T (*(other.all_data[i])) ;
	    }
	    return *this ;
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 */
	T & at(const size_t &pos) {
	    if ( pos >= length || pos < 0 ) throw index_out_of_bound() ;
	    return *(all_data[pos]) ;
	}
	const T & at(const size_t &pos) const {
	    if ( pos >= length || pos < 0 ) throw index_out_of_bound() ;
	    return *(all_data[pos]) ;
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos) {
	    if ( pos >= length || pos < 0 ) throw index_out_of_bound() ;
	    return *(all_data[pos]) ;
	}
	const T & operator[](const size_t &pos) const {
	    if ( pos >= length || pos < 0 ) throw index_out_of_bound() ;
	    return *(all_data[pos]) ;
	}
	/**
	 * access the first element.
	 * throw container_is_empty if size == 0
	 */
	const T & front() const {
	    if ( length == 0 ) throw container_is_empty() ;
	    return *(all_data[0]) ;
	}
	/**
	 * access the last element.
	 * throw container_is_empty if size == 0
	 */
	const T & back() const {
	    if ( length == 0 ) throw container_is_empty() ;
	    return *(all_data[length-1]) ;
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {
	    iterator temp ;
	    temp.real_pos = 0 ;
	    temp.real_pointer = this ;
	    return temp ;
	}
	const_iterator cbegin() const {
	    const_iterator temp(0,this) ;
	    return temp ;
	}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {
	    iterator temp ;
        temp.real_pos = length ;
        temp.real_pointer = this ;
	    return temp ;
	}
	const_iterator cend() const {
	    const_iterator temp(length,this) ;
	    return temp ;
	}
	/**
	 * checks whether the container is empty
	 */
	bool empty() const {
	    return length == 0 ;
	}
	/**
	 * returns the number of elements
	 */
	size_t size() const {
	    return length ;
	}
	/**
	 * clears the contents
	 */
	void clear() {
	    for ( int i = 0 ; i < up_bound ; i++ ){
            if ( all_data[i] != nullptr )
	            delete all_data[i] ;
            all_data[i] = nullptr ;
	    }
	    delete [] all_data ;
	    all_data = nullptr ;
	    length = 0 ;
	    up_bound = 0 ;
	}
	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */
	iterator insert(iterator pos, const T &value) {
	    for ( int i = length ; i > pos.real_pos ; i-- ){
	        all_data[i] = all_data[i-1] ;
	    }
        all_data[pos.real_pos] = new T (value) ;
	    length++ ;
        if (is_full()) this->split() ;
	    return pos ;
	}
	/**
	 * inserts value at index ind.
	 * after inserting, this->at(ind) == value
	 * returns an iterator pointing to the inserted value.
	 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
	 */
	iterator insert(const size_t &ind, const T &value) {
	    if ( ind < 0 || ind > length ) throw index_out_of_bound() ;
	    for ( int i = length ; i > ind ; i-- ){
	        all_data[i] = all_data[i-1] ;
	    }
        all_data[ind] = new T (value) ;
	    length++ ;
	    if (is_full()) this->split() ;
	    iterator temp ;
        temp.real_pos = ind ;
        temp.real_pointer = this ;
	    return temp ;
	}
	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the end() iterator is returned.
	 */
	iterator erase(iterator pos) {
	    delete all_data[pos.real_pos] ;
	    for ( int i = pos.real_pos ; i < length - 1 ; i++ ){
	        all_data[i] = all_data[i+1] ;
	    }
	    all_data[length-1] = nullptr ;
	    length-- ; // 不能 delete ，否则会删去目标空间
	    return pos ;
	}
	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound if ind >= size
	 */
	iterator erase(const size_t &ind) {
	    if ( ind < 0 || ind >= length ) throw index_out_of_bound() ;
	    delete all_data[ind] ;
	    for ( int i = ind ; i < length - 1 ; i++ ){
	        all_data[i] = all_data[i+1] ;
	    }
	    all_data[length-1] = nullptr ;
	    length-- ;
	    iterator temp ;
        temp.real_pos = ind ;
        temp.real_pointer = this ;
	    return temp ;
	}
	/**
	 * adds an element to the end.
	 */
	void push_back(const T &value) {
	    all_data[length] = new T (value) ;
	    length++ ;
        if (is_full()) this->split() ;
	}
	/**
	 * remove the last element from the end.
	 * throw container_is_empty if size() == 0
	 */
	void pop_back() {
	    delete all_data[length-1] ;
	    length-- ;
	}
};


}

#endif
