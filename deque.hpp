#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"

#include <cstddef>

#define NODE_SIZE 1000

namespace sjtu { 

template<class T>
class deque {

public:

    class node{

    public:

        T *  all_data[NODE_SIZE] = {0} ;
        node *left_son = nullptr ;
        node *right_son = nullptr ;
        int node_length = 0 ;

    public:

        node(){}

        node( const node &other ) : node_length(other.node_length) {
            for ( int i = 0 ; i < node_length ; i++ ){
                all_data[i] = new T ( *(other.all_data[i]) ) ;
            }
        }

        void all_clear(){
            for ( int i = 0 ; i < node_length ; i++ ){
                delete all_data[i] ;
            }
        }

        node &operator= ( const node &other ){
            if ( this == &other ) return *this ;
            for ( int i = 0 ; i < node_length ; i++ ){
                delete all_data[i] ;
            }
            for ( int i = 0 ; i < other.node_length ; i++ ){
                all_data[i] = new T ( *(other.all_data[i]) ) ;
            }
            node_length = other.node_length ;
            return *this ;
        }

        void add_element( int pos , const T & value ){
            for ( int i = node_length ; i > pos ; i-- ){
                all_data[i] = all_data[i-1] ;
            }
            all_data[pos] = new T (value) ;
            node_length++ ;
        }

        void del_element( int pos ){
            delete all_data[pos] ;
            for ( int i = pos ; i < node_length - 1 ; i++ ){
                all_data[i] = all_data[i+1] ;
            }
            all_data[node_length-1] = nullptr ;
            node_length-- ;
        }

        bool has_left(){
            return left_son != nullptr ;
        }

        bool has_right(){
            return right_son != nullptr ;
        }

        bool is_full(){
            return node_length == NODE_SIZE ;
        }

        bool is_empty(){
            return node_length == 0 ;
        }

        node* split(){
            if ( !this->has_right() ){
                right_son = new node ;
                right_son->left_son = this ;
            } else{
                node *temp = new node , *r_n = right_son ;
                temp->left_son = this ;
                temp->right_son = right_son ;
                r_n->left_son = temp ;
                right_son = temp ;
            }
            return right_son ;
        }

        void self_erase(){
            node *l_n = left_son , *r_n = right_son ;
            if (this->has_left())
                l_n->right_son = right_son ;
            if (this->has_right())
                r_n->left_son = left_son ;
            this->all_clear() ;
        }

    };

public:

    node *head = nullptr ; // 指向第一个块
    node *rear = nullptr ; // 指向最后一个块
    int length = 0 ; // 总长度


public:
	class const_iterator;
	class iterator {
	    friend deque<T> ;
	    friend const_iterator ;
	public:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */

		deque<T> *real_pointer ;
		node *node_pointer ;
		int real_pos ;

	public:

	    iterator( const iterator &other ) : real_pointer(other.real_pointer) , node_pointer(other.node_pointer) , real_pos(other.real_pos) {}
	    iterator() : real_pointer(nullptr) , node_pointer(nullptr) , real_pos(0) {}
	    iterator( deque<T> *r_p , node *n_p , int pos ) : real_pointer(r_p) , node_pointer(n_p) , real_pos(pos) {}
	    // todo 添加一个赋值函数？

		/**
		 * return a new iterator which pointer n-next elements
		 *   if there are not enough elements, iterator becomes invalid
		 * as well as operator-
		 */
		iterator operator+(const int &n) const { // todo 通过 node_length for减少来进行寻找
			//TODO
			iterator temp_it(*this) ;
			temp_it += n ;
			return temp_it ;
		}
		iterator operator-(const int &n) const {
			//TODO
			iterator temp_it(*this) ;
			temp_it -= n ;
            return temp_it ;
		}
		// return th distance between two iterator,
		// if these two iterators points to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const {
			//TODO
			// todo 反向减回来 添加
            if ( real_pointer != rhs.real_pointer ) throw invalid_iterator() ;
            iterator temp_it(*this) ;
            int dist = 0 ; // distance
            while ( temp_it.node_pointer != rhs.node_pointer ){ // 指向不同的 node
                dist += temp_it.node_pointer->node_length - temp_it.real_pos ;
                if ( !temp_it.node_pointer->has_right() ) {return -(rhs - *this) ;}
                temp_it.node_pointer = temp_it.node_pointer->right_son ;
                temp_it.real_pos = 0 ;
            }
            dist += rhs.real_pos - temp_it.real_pos ;
            return -dist ;
		}
		iterator& operator+=(const int &n) {
			//TODO
			int k = n ; // 避免破坏引用
			if ( k < 0 ){ *this -= -k ; return *this ; }
			while ( node_pointer->node_length <= real_pos + k ){
			    if ( !node_pointer->has_right() && node_pointer->node_length == real_pos + k ){ break ; }
                if ( !node_pointer->has_right() ) throw invalid_iterator() ;
                k -= node_pointer->node_length - real_pos ;  // 成为下一个 node 第一个元素
                node_pointer = node_pointer->right_son ;
                real_pos = 0 ;
			}
			real_pos += k ;
			return *this ;
		}
		iterator& operator-=(const int &n) {
			//TODO
			int k = n ;
			if ( k < 0 ) { *this += -k ; return *this ; }
			while ( real_pos < k ){
			    if ( !node_pointer->has_left() ) throw invalid_iterator() ;
			    k -= real_pos + 1 ;
			    node_pointer = node_pointer->left_son ;
			    real_pos = node_pointer->node_length - 1 ;
			}
			real_pos -= k ;
			return *this ;
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
		    iterator temp_it(*this) ;
		    (*this) += 1 ;
		    return temp_it ;
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++() {
            (*this)++ ;
            return *this ;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
		    iterator temp_it(*this) ;
		    *this -= 1 ;
            return temp_it ;
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--() {
            (*this)-- ;
            return *this ;
		}
		/**
		 * TODO *it
		 * 		throw if iterator is invalid
		 */
		T& operator*() const {
		    if ( this->real_pos >= this->node_pointer->node_length ) throw invalid_iterator() ; // 越界
		    return *(this->node_pointer->all_data[this->real_pos]) ;
		}
		/**
		 * TODO it->field
		 * 		throw if iterator is invalid
		 */
		T* operator->() const noexcept {
		    if ( this->real_pos >= this->node_pointer->node_length ) throw invalid_iterator() ;
		    return this->node_pointer->all_data[this->real_pos] ;
		} // todo 什么东西
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		bool operator==(const iterator &rhs) const {
		    if ( real_pointer == rhs.real_pointer && node_pointer == rhs.node_pointer && real_pos == rhs.real_pos ) return true ;
		    return false ;
		}
		bool operator==(const const_iterator &rhs) const {
		    if ( real_pointer == rhs.real_pointer && node_pointer == rhs.real_pointer && real_pos == rhs.real_pos ) return true ;
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
	class const_iterator {
	    friend deque<T> ;
	    friend iterator ;
		// it should has similar member method as iterator.
		//  and it should be able to construct from an iterator.
		private:
			// data members.
			const deque<T> *real_pointer = nullptr ;
			node *node_pointer = nullptr ;
			int real_pos = 0 ;

		public:
			const_iterator() {
				// TODO
			}
			const_iterator(const const_iterator &other) : real_pointer(other.real_pointer) , node_pointer(other.node_pointer) , real_pos(other.real_pos) {
				// TODO
			}
			const_iterator(const iterator &other) {
				// TODO
				real_pointer = other.real_pointer ;
				node_pointer = other.real_pointer ;
				real_pos = other.real_pos ;
			}
			const_iterator( const deque<T> *r_p , node *n_p , int pos ) : real_pointer(r_p) , node_pointer(n_p) , real_pos(pos) {}
			// And other methods in iterator.
			// And other methods in iterator.
			// And other methods in iterator.
        /**
     * return a new const_iterator which pointer n-next elements
     *   if there are not enough elements, const_iterator becomes invalid
     * as well as operator-
     */
        const_iterator operator+(const int &n) const { // todo 通过 node_length for减少来进行寻找
            //TODO
            const_iterator temp_it(*this) ;
            temp_it += n ;
            return temp_it ;
        }
        const_iterator operator-(const int &n) const {
            //TODO
            const_iterator temp_it(*this) ;
            temp_it -= n ;
            return temp_it ;
        }
// return th distance between two const_iterator,
// if these two const_iterators points to different vectors, throw invaild_iterator.
        int operator-(const const_iterator &rhs) const {
            //TODO
            if ( real_pointer != rhs.real_pointer ) throw invalid_iterator() ;
            const_iterator temp_it(*this) ;
            int dist = 0 ; // distance
            while ( temp_it.node_pointer != rhs.node_pointer ){ // 指向不同的 node
                dist += temp_it.node_pointer->node_length - temp_it.real_pos ;
                if ( !temp_it.node_pointer->has_right() ) {return -(rhs - *this) ;}
                temp_it.node_pointer = temp_it.node_pointer->right_son ;
                temp_it.real_pos = 0 ;
            }
            dist += rhs.real_pos - temp_it.real_pos ;
            return -dist ;
        }
        const_iterator& operator+=(const int &n) {
            //TODO
            int k = n ; // 避免破坏引用
            while ( node_pointer->node_length <= real_pos + k ){
                if ( !node_pointer->has_right() && node_pointer->node_length == real_pos + k ) break ;
                if ( !node_pointer->has_right() ) throw invalid_iterator() ;
                k -= node_pointer->node_length - real_pos ;  // 成为下一个 node 第一个元素
                node_pointer = node_pointer->right_son ;
                real_pos = 0 ;
            }
            real_pos += k ;
            return *this ;
        }
        const_iterator& operator-=(const int &n) {
            //TODO
            int k = n ;
            while ( real_pos < k ){
                if ( !node_pointer->has_left() ) throw invalid_iterator() ;
                k -= real_pos + 1 ;
                node_pointer = node_pointer->left_son ;
                real_pos = node_pointer->node_length - 1 ;
            }
            real_pos -= k ;
            return *this ;
        }
/**
 * TODO iter++
 */
        const_iterator operator++(int) {
            const_iterator temp_it(*this) ;
            *this += 1 ;
            return temp_it ;
        }
/**
 * TODO ++iter
 */
        const_iterator& operator++() {
            (*this)++ ;
            return *this ;
        }
/**
 * TODO iter--
 */
        const_iterator operator--(int) {
            const_iterator temp_it(*this) ;
            *this -= 1 ;
            return temp_it ;
        }
/**
 * TODO --iter
 */
        const_iterator& operator--() {
            (*this)-- ;
            return *this ;
        }
/**
 * TODO *it
 * 		throw if const_iterator is invalid
 */
        T& operator*() const {
            if ( this->real_pos >= this->node_pointer->node_length ) throw invalid_iterator() ; // 越界
            return *(this->node_pointer->all_data[this->real_pos]) ;
        }
/**
 * TODO it->field
 * 		throw if const_iterator is invalid
 */
        T* operator->() const noexcept {
            if ( this->real_pos >= this->node_pointer->node_length ) throw invalid_iterator() ;
            return this->node_pointer->all_data[this->real_pos] ;
        } // todo 什么东西
/**
 * a operator to check whether two const_iterators are same (pointing to the same memory).
 */
        bool operator==(const const_iterator &rhs) const {
            if ( real_pointer == rhs.real_pointer && node_pointer == rhs.node_pointer && real_pos == rhs.real_pos ) return true ;
            return false ;
        }
        bool operator==(const iterator &rhs) const {
            if ( real_pointer == rhs.real_pointer && node_pointer == rhs.real_pointer && real_pos == rhs.real_pos ) return true ;
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
	 * TODO Constructors
	 */
	deque() {
	    head = new node ;
	    rear = head ;
	    length = 0 ;
	}
	deque(const deque &other) {
	    head = new node (*(other.head)) ;
	    length = other.length ;
	    node *mover = other.head->right_son , *catcher = head ;
        while ( mover != nullptr ){
            catcher->right_son = new node (*mover) ; // 创建新的 node
            catcher->right_son->left_son = catcher ; // 连接旧的 node
            catcher = catcher->right_son ;
            mover = mover->right_son ;
        }
        rear = catcher ;
	}
	/**
	 * TODO Deconstructor
	 */
	~deque() {
	    node *mover = head->right_son ;
	    while ( mover != nullptr ){
	        head->self_erase() ;
	        delete head ;
	        head = mover ;
	        mover = mover->right_son ;
	    }
	    head->self_erase() ;
	    delete head ;
	}
	/**
	 * TODO assignment operator
	 */
	deque &operator=(const deque &other) {
	    if ( this == &other ) return *this ;
        this->clear() ;
        delete head ;
        head = new node (*(other.head)) ;
        length = other.length ;
        node *mover = other.head->right_son , *catcher = head ;
        rear = head ;
        while ( mover != nullptr ){
            catcher->right_son = new node (*mover) ; // 创建新的 node
            catcher->right_son->left_son = catcher ; // 连接旧的 node
            catcher = catcher->right_son ;
            rear = catcher ;
            mover = mover->right_son ;
        }
        return *this ;
	} // todo 出问题了，重新处理
	/**
	 * access specified element with bounds checking
	 * throw index_out_of_bound if out of bound.
	 */
	T & at(const size_t &pos) {
	    if ( pos >= length || pos < 0 ) throw index_out_of_bound() ;
	    iterator temp_it = this->begin() ;
	    temp_it += pos ;
	    return *(temp_it.node_pointer->all_data[temp_it.real_pos]) ;
	}
	const T & at(const size_t &pos) const {
	    if ( pos >= length || pos < 0 ) throw index_out_of_bound() ;
	    const_iterator temp_it = this->cbegin() ;
	    temp_it += pos ;
	    return *(temp_it.node_pointer->all_data[temp_it.real_pos]) ;
	} // todo 用 iterator 来查找
	T & operator[](const size_t &pos) {
	    return this->at(pos) ;
	}
	const T & operator[](const size_t &pos) const {
	    return this->at(pos) ;
	}
	/**
	 * access the first element
	 * throw container_is_empty when the container is empty.
	 */
	const T & front() const {
	    if ( length == 0 ) throw container_is_empty() ;
	    return *(head->all_data[0]) ;
	}
	/**
	 * access the last element
	 * throw container_is_empty when the container is empty.
	 */
	const T & back() const {
	    if ( length == 0 ) throw container_is_empty() ;
	    return *(rear->all_data[rear->node_length-1]) ;
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {
	    iterator temp_it(this,head,0) ;
        return temp_it ;
	}
	const_iterator cbegin() const {
	    const_iterator temp_it(this,head,0) ;
	    return temp_it ;
	}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {
	    iterator temp_it(this,rear,rear->node_length) ;
	    return temp_it ;
	}
	const_iterator cend() const {
	    const_iterator temp_it(this,rear,rear->node_length) ;
	    return temp_it ;
	}
	/**
	 * checks whether the container is empty.
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
	    if (length == 0) return ;
        node *mover = head->right_son ;
        while ( mover != nullptr ){
            head->self_erase() ;
            delete head ;
            head = mover ;
            mover = mover->right_son ;
        }
        head->self_erase() ;
        delete head ;
        head = new node ;
        rear = head ;
        length = 0 ;
	}
	/**
	 * inserts elements at the specified locat on in the container.
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value
	 *     throw if the iterator is invalid or it point to a wrong place.
	 */
	iterator insert(iterator pos, const T &value) { // todo 调用 add_element
        if ( pos.node_pointer->node_length < pos.real_pos || pos.real_pointer != this ) throw invalid_iterator() ;
        if ( pos.node_pointer->is_full() ){ // 开辟新空间
            if ( !pos.node_pointer->has_right() || pos.node_pointer->right_son->is_full() )
                pos.node_pointer->split() ;
            if ( pos.real_pos == NODE_SIZE ){ // 当通过 push_back 添加元素
                pos.node_pointer->right_son->add_element(0,value) ;
                length++ ;
                rear = rear->right_son ;
                iterator temp_it(this,pos.node_pointer->right_son,0) ;
                return temp_it ;
            }
            pos.node_pointer->right_son->add_element(0,*(pos.node_pointer->all_data[NODE_SIZE-1])) ; // 添加元素
            pos.node_pointer->del_element(NODE_SIZE-1) ; // 删除最后一个元素
            pos.node_pointer->add_element(pos.real_pos,value) ; // 添加新元素
            length++ ;
            if ( pos.node_pointer == rear ) rear = rear->right_son ;
            return pos ;
        }
        pos.node_pointer->add_element(pos.real_pos,value) ; // 不开辟新空间
        length++ ;
        return pos ;
	}
	/**
	 * removes specified element at pos.
	 * removes the element at pos.
	 * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
	 * throw if the container is empty, the iterator is invalid or it points to a wrong place.
	 */
	iterator erase(iterator pos) {
	    if ( pos.node_pointer->node_length <= pos.real_pos || pos.real_pointer != this ) throw invalid_iterator() ;
	    pos.node_pointer->del_element(pos.real_pos) ;
	    length-- ;
	    if ( pos.node_pointer->is_empty() ) { // 删除空 node
	        node *catcher , *mover ;
            if ( head == rear ) return pos ; // 全部删除，只保留一个 node
	        if ( pos.node_pointer == head ){ // 删除头
	            catcher = head ;
	            head = head->right_son ; // todo 特判删光情况
	            head->left_son = nullptr ;
	            catcher->self_erase() ;
	            delete catcher ;
	            return this->begin() ;
	        }
	        if ( pos.node_pointer == rear ){ // 删除尾
	            catcher = rear ;
	            rear = rear->left_son ;
	            rear->right_son = nullptr ;
	            catcher->self_erase() ;
	            delete catcher ;
	            return this->end() ;
	        }
	        iterator temp_it(this,pos.node_pointer->right_son,0) ;
	        pos.node_pointer->self_erase() ;
	        delete pos.node_pointer ;
            return temp_it ;
	    }
	    if ( pos.real_pos == pos.node_pointer->node_length && pos.node_pointer != rear ){ // 指向下一个node第一个元素
	        iterator temp_it(this,pos.node_pointer->right_son,0) ;
	        return temp_it ;
	    }
	    return pos ;
	}
	/**
	 * adds an element to the end
	 */
	void push_back(const T &value) {
	    this->insert(this->end(),value) ;
	}
	/**
	 * removes the last element
	 *     throw when the container is empty.
	 */
	void pop_back() {
	    if ( length == 0 ) throw container_is_empty() ;
	    this->erase(--this->end()) ;
	}
	/**
	 * inserts an element to the beginning.
	 */
	void push_front(const T &value) {
	    this->insert(this->begin(),value) ;
	}
	/**
	 * removes the first element.
	 *     throw when the container is empty.
	 */
	void pop_front() {
	    if ( length == 0 ) throw container_is_empty() ;
	    this->erase(this->begin()) ;
	}
};

}

#endif
