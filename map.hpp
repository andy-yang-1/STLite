/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {

template<
	class Key,
	class T,
	class Compare = std::less<Key>
> class map {
public:
	/**
	 * the internal type of data.
	 * it should have a default constructor, a copy constructor.
	 * You can use sjtu::map as value_type by typedef.
	 */



	struct node ;
	node *root = nullptr ;
	int length = 0 ;

	Key *max_key = nullptr , *min_key = nullptr ;

	class iterator ;
	iterator real_end_iterator ;

	typedef pair< Key, T> value_type; // todo 把 const 去掉试试
	/**
	 * see BidirectionalIterator at CppReference for help.
	 *
	 * if there is anything wrong throw invalid_iterator.
	 *     like it = map.begin(); --it;
	 *       or it = map.end(); ++end();
	 */

    struct node{ // todo 把所有的 key 和 real_value 合并成 value_type

//        Key *key = nullptr ;
//        T *real_value = nullptr ;
        value_type data ;
        int depth = 0 ;

        node *father = nullptr ;
        node *left_son = nullptr ;
        node *right_son = nullptr ;

        node(){}
        node( const value_type &input_inf ):data(input_inf){}
        node( const Key &input_key ){ data.first = input_key ; }
        node( const Key &input_key , const T &input_value ){ data.first = input_key ; data.second = input_value ; }
        node( const node &other ){ data = other.data ; }



        ~node(){
            depth = 0 ;
        }

        bool has_right() const {
            return right_son != nullptr ;
        }

        bool has_left() const {
            return left_son != nullptr ;
        }

        bool is_root( node *&temp_root ){
            return this == temp_root ; // todo 需要传参来判别 root
        }

        bool operator<( const node &other ) const {
            Compare cmp ;
            return cmp( data.first , (other.data.first)) ;
        }
        bool operator==( const node &other ) const {
            Compare cmp ;
            return !(cmp(data.first , other.data.first)) && !(cmp(other.data.first,data.first)) ;
        }
        bool operator>( const node &other ) const {
            Compare cmp ;
            return cmp(other.data.first,data.first) ;
        }

        node* search( const Key &i ){ // todo search 增加一个 value_type 的重载
            Compare cmp ;
            if ( !cmp(this->data.first , i) && !cmp( i , this->data.first ) ) return this ;
            if ( cmp(this->data.first,i) && this->has_right() ) return right_son->search(i) ;
            if ( cmp(i , this->data.first) && this->has_left() ) return left_son->search(i) ;
            return nullptr ;
        }

        node* search( const value_type &i ){
            node temp_node(i) ;
            if ( *this == temp_node ) return this ;
            if ( temp_node > *this && this->has_right() ) return right_son->search(i) ;
            if ( temp_node < *this && this->has_left() ) return left_son->search(i) ;
            return nullptr ;
        }

        node* get_precursor(){
            node *mover , *catcher ;
            if ( this->has_left() ){ // 存在左子节点
                mover = this->left_son ;
                while (mover->has_right()){
                    mover = mover->right_son ;
                }
                return mover ;
            }
            mover = father ; catcher = this ;
            while ( mover->right_son != catcher ){ // 没有发生回跳
                mover = mover->father ;
                catcher = catcher->father ;
            }
            return mover ;
        }

        node* get_successor(){
            node *mover , *catcher ;
            if ( this->has_right() ){
                mover = right_son ;
                while (mover->has_left()){
                    mover = mover->left_son ;
                }
                return mover ;
            }
            mover = father ; catcher = this ;
            while ( mover->left_son != catcher ){
                mover = mover->father ;
                catcher = catcher->father ;
            }
            return mover ;
        }

        void get_depth(){
            if ( !this->has_left() && !this->has_right() ){
                depth = 0 ;
                return ;
            }
            if ( !this->has_left() ){
                depth = right_son->depth + 1 ;
                return ;
            }
            if ( !this->has_right() ){
                depth = left_son->depth + 1 ;
                return ;
            }
            depth = left_son->depth > right_son->depth ? left_son->depth + 1 : right_son->depth + 1 ; // 左右节点同时存在
        }

        int get_balance_factor() const { // 左减右
            if ( !this->has_left() && !this->has_right() ){
                return 0 ;
            }
            if ( !this->has_left() ){
                return (-right_son->depth-1) ;
            }
            if ( !this->has_right() ){
                return left_son->depth + 1 ;
            }
            return left_son->depth - right_son->depth ;
        }

        void left_rotate( node *&temp_root ) { // todo 在旋转时要 get_depth()
            node *temp_left = left_son ;

            temp_left->father = this->father ; // 父节点转移
            if ( !this->is_root(temp_root) ){ // 特判 root
                if ( this->father->left_son == this ){
                    this->father->left_son = temp_left ;
                }else{
                    this->father->right_son = temp_left ;
                }
            }else{
                temp_root = temp_left ;
            }

            this->left_son = temp_left->right_son ; // 右节点转移
            if ( temp_left->has_right() ){
                temp_left->right_son->father = this ;
            }

            temp_left->right_son = this ; // 原父节点作为右节点
            this->father = temp_left ; // 原左节点作为父节点

            this->get_depth() ; // 更新高度
            temp_left->get_depth() ;
        }

        void right_rotate( node *&temp_root ) { // todo 在旋转时加入根节点判断
            node *temp_right = right_son ;

            temp_right->father = this->father ;
            if ( !this->is_root(temp_root) ){
                if ( this->father->left_son == this ){
                    this->father->left_son = temp_right ;
                }else{
                    this->father->right_son = temp_right ;
                }
            }else{
                temp_root = temp_right ;
            }

            this->right_son = temp_right->left_son ;
            if ( temp_right->has_left() ){
                temp_right->left_son->father = this ;
            }

            temp_right->left_son = this ;
            this->father = temp_right ;

            this->get_depth() ;
            temp_right->get_depth() ;
        }

        void R_left_rotate( node* &temp_root ){
            node *temp_right = right_son ;
            temp_right->left_rotate(temp_root) ;
            this->right_rotate(temp_root) ;
        }
        void L_right_rotate( node* &temp_root ){
            node *temp_left = left_son ;
            temp_left->right_rotate(temp_root) ;
            this->left_rotate(temp_root) ;
        }

        node* get_max( node *temp_root ){ // 修改接口：用参数实现 node 类内 root 需求
            node *mover = temp_root ;
            while( mover->has_right() ){
                mover = mover->right_son ;
            }
            return mover ;
        }
        node* get_min( node *temp_root ){
            node *mover = temp_root ;
            while ( mover->has_left() ){
                mover = mover->left_son ;
            }
            return mover ;
        }

        node* insert( const value_type &_key , node *&temp_root ){ // todo 改 insert ，将所有的 _key 换成 value_type 的 value
            node *answer_node = nullptr ;
            node temp_node(_key) ;
            if ( temp_node > *this && !this->has_right() ){
                this->right_son = new node(_key) ;
                answer_node = this->right_son ;
                this->right_son->father = this ;
                this->get_depth() ;
                return answer_node ;
            }
            if ( temp_node < *this && !this->has_left() ){
                this->left_son = new node(_key) ;
                answer_node = this->left_son ;
                this->left_son->father = this ;
                this->get_depth() ;
                return answer_node ;
            }

            int temp_balance_factor = 0 , son_balance_factor = 0 ;

            if ( temp_node < *this ){ // 左节点插入
                answer_node = this->left_son->insert(_key,temp_root) ;
                this->get_depth() ;
                temp_balance_factor = this->get_balance_factor() ;
                if ( temp_balance_factor >= -1 && temp_balance_factor <= 1 ) return answer_node ;
                son_balance_factor = left_son->get_balance_factor() ;
                if ( son_balance_factor * temp_balance_factor < 0 ) this->L_right_rotate(temp_root) ;
                else{
                    this->left_rotate(temp_root) ;
                }
                return answer_node ;
            }

            // todo 在旋转时加入根节点判断

            if ( temp_node > *this ){ // 右节点插入
                answer_node = this->right_son->insert(_key,temp_root) ;
                this->get_depth() ;
                temp_balance_factor = this->get_balance_factor() ;
                if ( temp_balance_factor >= -1 && temp_balance_factor <= 1 ) return answer_node ;
                son_balance_factor = right_son->get_balance_factor() ;
                if ( son_balance_factor * temp_balance_factor < 0 ) this->R_left_rotate(temp_root) ;
                else{
                    this->right_rotate(temp_root) ;
                }
                return answer_node ;
            }

            return answer_node ;
        }

        void del_key( const Key &_key , node *&temp_root ){ //todo 根节点删除
            node *catcher = this->search(_key) , *mover = catcher->father , *successor ; // todo 特判 root

            if ( !catcher->has_left() && !catcher->has_right() ){ // todo 全部添加 删 root 特判
                if ( mover != nullptr ) {
                    if (mover->left_son == catcher) {
                        mover->left_son = nullptr;
                    } else {
                        mover->right_son = nullptr;
                    }
                    delete catcher;
                }else{
                    delete catcher ;
                    temp_root = nullptr ;
                    return ; // 删 root 终止
                }
            }

            else if ( !catcher->has_left() && catcher->has_right() ){
                if ( mover != nullptr ) {
                    if (mover->left_son == catcher) {
                        mover->left_son = catcher->right_son;
                    } else {
                        mover->right_son = catcher->right_son;
                    }
                    catcher->right_son->father = mover;
                    delete catcher;
                }else{
                    catcher->right_son->father = nullptr ;
                    temp_root = catcher->right_son ;
                    delete catcher ;
                    return ;
                }
            }

            else if ( !catcher->has_right() && catcher->has_left() ){
                if ( mover != nullptr ) {
                    if (mover->left_son == catcher) {
                        mover->left_son = catcher->left_son;
                    } else {
                        mover->right_son = catcher->left_son;
                    }
                    catcher->left_son->father = mover;
                    delete catcher;
                }else{
                    catcher->left_son->father = nullptr ;
                    temp_root = catcher->left_son ;
                    delete catcher ;
                    return ;
                }
            }

            else if ( catcher->has_left() && catcher->has_right() ){
                successor = catcher->get_successor() ; // 采用值移动而非位置移动
                mover = successor->father ;
                if ( mover->left_son == successor ){
                    mover->left_son = successor->right_son ;
                }else{
                    mover->right_son = successor->right_son ;
                }
                if ( successor->has_right() ){
                    successor->right_son->father = mover ;
                }

//                catcher->data.first = successor->data.first ; // todo 不允许值转移
//                catcher->data.second = successor->data.second ;

                successor->father = catcher->father ;
                successor->left_son = catcher->left_son ;
                successor->right_son = catcher->right_son ; // 采用节点转移

                if ( successor->has_left() ){
                    successor->left_son->father = successor ;
                }
                if ( successor->has_right() ){
                    successor->right_son->father = successor ;
                }
                if ( successor->father != nullptr ){
                    if ( successor->father->left_son == catcher ){
                        successor->father->left_son = successor ;
                    }else{
                        successor->father->right_son = successor ;
                    }
                }else{
                    temp_root = successor ; // 特判根节点
                }

                if ( mover == catcher ){ // 左子树或右子树仅有一个节点
                    mover = successor ;
                }

                delete catcher ;
            }

            int temp_balance_factor = 0 , son_balance_factor = 0 ;

            while ( mover != nullptr ){
                mover->get_depth() ;
                temp_balance_factor = mover->get_balance_factor() ;
                if ( temp_balance_factor >= -1 && temp_balance_factor <= 1 ){
                    mover = mover->father ;
                    continue;
                }

                son_balance_factor = temp_balance_factor > 0 ? mover->left_son->get_balance_factor() : mover->right_son->get_balance_factor() ; // 判断出问题的节点

                if ( temp_balance_factor > 0 ){
                    if ( son_balance_factor < 0 ){
                        mover->L_right_rotate(temp_root) ;
                    }else{
                        mover->left_rotate(temp_root) ;
                    }
                }else{
                    if ( son_balance_factor > 0 ){
                        mover->R_left_rotate(temp_root) ;
                    }else{
                        mover->right_rotate(temp_root) ;
                    }
                } // 旋转系列操作

            }
        }

        void del_all( node *&temp_root ){ // 递归删除
            if ( temp_root == nullptr ) return ;
            del_all( temp_root->left_son ) ;
            del_all( temp_root->right_son ) ;
            delete temp_root ;
        }

        void show_all(){
            if ( this->has_left() ) left_son->show_all() ;
            printf("%d\n",data.first) ;
            if ( this->has_right() ) right_son->show_all() ;
        }

        bool check_is_right( ){
            Compare cmp ;
            if ( this == nullptr ) return true ;
            if ( this->has_left() ){
                if ( cmp( this->data.first , this->left_son->data.first ) ) return false ;
                if ( !this->left_son->check_is_right() ) return false ;
            }
            if ( this->has_right() ){
                if ( cmp( this->right_son->data.first , this->data.first ) ) return false ;
                if ( !this->right_son->check_is_right() ) return false ;
            }
            return true ;
        }


    };

	class const_iterator;
	class iterator {
    public:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */


		map<Key,T,Compare> *real_pointer = nullptr ;
		node *node_pointer = nullptr ;

	public:
		iterator() {
			// TODO
			real_pointer = nullptr ;
			node_pointer = nullptr ;
		}
		iterator(const iterator &other) {
			// TODO
			real_pointer = other.real_pointer ;
			node_pointer = other.node_pointer ;
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
		    iterator temp_it(*this) ;
		    Compare cmp ;
		    if ( temp_it == temp_it.real_pointer->real_end_iterator ) throw invalid_iterator() ;
		    if ( !cmp((temp_it.node_pointer->data.first) , *(temp_it.real_pointer->max_key)) && !cmp(*(temp_it.real_pointer->max_key),temp_it.node_pointer->data.first) ){
		        *this = temp_it.real_pointer->real_end_iterator ;
		    }else{
		        this->node_pointer = temp_it.node_pointer->get_successor() ;
		    }
		    return temp_it ;
		}
		/**
		 * TODO ++iter
		 */
		iterator & operator++() {
            (*this)++ ;
            return *this ;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
		    iterator temp_it(*this) ;
		    Compare cmp ;
		    if ( *this == real_pointer->begin() ){
		        throw invalid_iterator() ;
		    }
		    if ( *this == this->real_pointer->real_end_iterator ){
		        this->node_pointer = this->real_pointer->root->get_max(this->real_pointer->root) ; // end -- 获得的效果
		        return temp_it ;
		    }
		    if ( !cmp((temp_it.node_pointer->data.first) , *(temp_it.real_pointer->min_key)) && !cmp(*(temp_it.real_pointer->min_key),temp_it.node_pointer->data.first) ){
		        throw invalid_iterator() ;
		    }else{
		        this->node_pointer = temp_it.node_pointer->get_precursor() ;
		    }
		    return temp_it ;
		}
		/**
		 * TODO --iter
		 */
		iterator & operator--() {
            (*this)-- ;
            return *this ;
		}
		/**
		 * an operator to check whether two iterators are same (pointing to the same memory).
		 */
		value_type & operator*() const {
            return node_pointer->data ; // todo 没有把握
		}
		bool operator==(const iterator &rhs) const {
		    return ( real_pointer == rhs.real_pointer && node_pointer == rhs.node_pointer ) ;
		}
		bool operator==(const const_iterator &rhs) const {
		    return ( real_pointer == rhs.real_pointer && node_pointer == rhs.node_pointer ) ;
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

		/**
		 * for the support of it->first. 
		 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
		 */
		value_type* operator->() const noexcept {
            return &(node_pointer->data) ;
            // todo 注意，原网址损坏，不一定正确，非真实值该如何赋值
		}
	};
	class const_iterator {
		// it should has similar member method as iterator.
		//  and it should be able to construct from an iterator.
        public:
			// data members.
			const map<Key,T,Compare> *real_pointer = nullptr ;
			node *node_pointer = nullptr ;
		public:
			const_iterator() {
				// TODO
				real_pointer = nullptr ;
				node_pointer = nullptr ;
			}
			const_iterator(const const_iterator &other) {
				// TODO
				real_pointer = other.real_pointer ;
				node_pointer = other.node_pointer ;
			}
			const_iterator(const iterator &other) {
				// TODO
				real_pointer = other.real_pointer ;
				node_pointer = other.node_pointer ;
			}
			// And other methods in iterator.
			// And other methods in iterator.
			// And other methods in iterator.
        /**
     * TODO iter++
     */
        const_iterator operator++(int) {
            const_iterator temp_it(*this) ;
            Compare cmp ;
            if ( temp_it == temp_it.real_pointer->real_end_iterator ) throw invalid_iterator() ;
            if ( !cmp((temp_it.node_pointer->data.first) , *(temp_it.real_pointer->max_key)) && !cmp(*(temp_it.real_pointer->max_key),temp_it.node_pointer->data.first) ){
                *this = temp_it.real_pointer->real_end_iterator ;
            }else{
                this->node_pointer = temp_it.node_pointer->get_successor() ;
            }
            return temp_it ;
        }
        /**
         * TODO ++iter
         */
        const_iterator & operator++() {
            (*this)++ ;
            return *this ;
        }
        /**
         * TODO iter--
         */
        const_iterator operator--(int) {
            const_iterator temp_it(*this) ;
            Compare cmp ;
            if ( *this == this->real_pointer->cbegin() ){
                throw invalid_iterator() ;
            }
            if ( *this == this->real_pointer->real_end_iterator ){
                this->node_pointer = this->real_pointer->root->get_max(this->real_pointer->root) ;
                return temp_it ;
            }
            if ( !cmp((temp_it.node_pointer->data.first) , *(temp_it.real_pointer->min_key)) && !cmp(*(temp_it.real_pointer->min_key),temp_it.node_pointer->data.first) ){
                throw invalid_iterator() ;
            }else{
                this->node_pointer = temp_it.node_pointer->get_precursor() ;
            }
            return temp_it ;
        }
        /**
         * TODO --iter
         */
        const_iterator & operator--() {
            (*this)-- ;
            return *this ;
        }
        /**
         * an operator to check whether two iterators are same (pointing to the same memory).
         */
        value_type & operator*() const {
            return node_pointer->data ; // todo 没有正确的把握
        }
        bool operator==(const iterator &rhs) const {
            return ( real_pointer == rhs.real_pointer && node_pointer == rhs.node_pointer ) ;
        }
        bool operator==(const const_iterator &rhs) const {
            return ( real_pointer == rhs.real_pointer && node_pointer == rhs.node_pointer ) ;
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

        /**
         * for the support of it->first.
         * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
         */
        value_type* operator->() const noexcept {
            return &(node_pointer->data) ;
            // todo 注意，原网址损坏，不一定正确，非真实值该如何赋值
        }
	};
	/**
	 * TODO two constructors
	 */
	map() {
	    root = nullptr ;
	    length = 0 ;
	    max_key = nullptr ;
	    min_key = nullptr ;
	    real_end_iterator.real_pointer = this ;
	    real_end_iterator.node_pointer = nullptr ;
	}

	map(const map &other) {
	    // todo 用 map 的 insert 来实现插入
	    if ( other.length != 0 ){ // 避免拷贝构造一个空 map
	        auto it = other.cbegin() ;
	        for ( ; it != other.cend() ; it++ ){
	            this->insert(it.node_pointer->data) ;
	        }
	    }
	    real_end_iterator.real_pointer = this ;
	    real_end_iterator.node_pointer = nullptr ;
	}
	/**
	 * TODO assignment operator
	 */
	map & operator=(const map &other) {
	    if ( this == &other ) return *this ;
	    this->clear() ;
	    if ( other.length == 0 ) return *this ;
	    auto it = other.cbegin() ;
	    for ( ; it != other.cend() ; it++ ){
	        this->insert(it.node_pointer->data) ;
	    }
	    return *this ;
	}
	/**
	 * TODO Destructors
	 */
	~map() {
	    root->del_all(root) ;
	    delete max_key ;
	    delete min_key ;
	}
	/**
	 * TODO
	 * access specified element with bounds checking
	 * Returns a reference to the mapped value of the element with key equivalent to key.
	 * If no such element exists, an exception of type `index_out_of_bound'
	 */
	T & at(const Key &key) {
	    if ( !this->count(key) ){
	        throw index_out_of_bound() ;
	    }
	    node *mover = root->search(key) ;
	    return mover->data.second ;
	}
	const T & at(const Key &key) const {
	    if (!this->count(key)){
	        throw index_out_of_bound() ;
	    }
	    node *mover = root->search(key) ;
	    return mover->data.second ;
	}
	/**
	 * TODO
	 * access specified element 
	 * Returns a reference to the value that is mapped to a key equivalent to key,
	 *   performing an insertion if such key does not already exist.
	 */
	T & operator[](const Key &key) {
	    if ( this->count(key) ) return this->at(key) ;
	    iterator temp_it((this->insert({key,T()})).first) ;
	    return temp_it.node_pointer->data.second ;
	}
	/**
	 * behave like at() throw index_out_of_bound if such key does not exist.
	 */
	const T & operator[](const Key &key) const {
	    return this->at(key) ;
	}
	/**
	 * return a iterator to the beginning
	 */
	iterator begin() {
	    if ( root == nullptr ){
	        return this->end() ;
	    }
	    node *mover = root->get_min(root) ;
	    iterator temp_it ;
	    temp_it.real_pointer = this ;
	    temp_it.node_pointer = mover ;
	    return temp_it ;
	}
	const_iterator cbegin() const {
	    if ( root == nullptr ){
	        return this->cend() ;
	    }
	    node *mover = root->get_min(root) ;
	    const_iterator temp_it ;
	    temp_it.real_pointer = this ;
	    temp_it.node_pointer = mover ;
	    return temp_it ;
	}
	/**
	 * return a iterator to the end
	 * in fact, it returns past-the-end.
	 */
	iterator end() {
	    return real_end_iterator ;
	}
	const_iterator cend() const {
	    const_iterator temp_it = real_end_iterator ;
	    return temp_it ;
	}
	/**
	 * checks whether the container is empty
	 * return true if empty, otherwise false.
	 */
	bool empty() const {
	    return length == 0 ;
	}
	/**
	 * returns the number of elements.
	 */
	size_t size() const {
	    return length ;
	}
	/**
	 * clears the contents
	 */
	void clear() {
	    root->del_all(root) ;
	    delete max_key ;
	    delete min_key ;
	    length = 0 ;
	    root = nullptr ;
	    max_key = nullptr ;
	    min_key = nullptr ;
	}
	/**
	 * insert an element.
	 * return a pair, the first of the pair is
	 *   the iterator to the new element (or the element that prevented the insertion), 
	 *   the second one is true if insert successfully, or false.
	 */
	pair<iterator, bool> insert(const value_type &value) {
	    iterator temp_it ;
	    temp_it.real_pointer = this ;
	    Compare cmp ;

	    if ( length == 0 ){ // 特判第一个添加
	        root = new node (value) ;
	        temp_it.node_pointer  = root ;
            max_key = new Key (value.first) ;
            min_key = new Key (value.first) ;
            length++ ;
            return {temp_it, true} ;
	    }
	    node *mover = root->search(value) ;
	    if ( mover != nullptr ){
	        temp_it.node_pointer = mover ;
	        return {temp_it, false} ; // todo 不知道要不要修改 real_value
	    }
	    length++ ;
	    if ( max_key == nullptr || cmp(*max_key , value.first) ){
	        delete max_key ;
	        max_key = new Key (value.first) ;
	    }
	    if ( min_key == nullptr || cmp( value.first , *min_key ) ){
	        delete min_key ;
	        min_key = new Key (value.first) ;
	    }
	    mover = root->insert(value,root) ;
	    temp_it.node_pointer = mover ;
	    return {temp_it,true} ;
	}
	/**
	 * erase the element at pos.
	 *
	 * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
	 */
	void erase(iterator pos) { // todo erase 出现问题
        if ( pos == this->end() || pos.real_pointer != this || length == 0 ) throw invalid_iterator() ;
	    node *mover = root->search(pos.node_pointer->data.first) , *catcher ; // todo 直接用 pos 删
	    Compare cmp ;

	    if ( mover == nullptr ) throw invalid_iterator() ;
	    if ( !cmp(mover->data.first , *max_key) && !cmp(*max_key,mover->data.first) ){ // 删除最大 key
	        delete max_key ;
	        max_key = nullptr ;
	        if ( length > 1 ){ // 避免出现无路可找的情况
	            catcher = root->get_max(root)->get_precursor() ; // 找到前一个
	            max_key = new Key ( catcher->data.first ) ;
	        }
	    }
	    if ( !cmp(mover->data.first , *min_key) && !cmp(*min_key,mover->data.first) ){ // 删除最小 key
	        delete min_key ;
	        min_key = nullptr ;
	        if ( length > 1 ){
	            catcher = root->get_min(root)->get_successor() ; // 找到后一个
	            min_key = new Key ( catcher->data.first ) ;
	        }
	    }
	    root->del_key( mover->data.first , root ) ;
	    length-- ;
	}
	/**
	 * Returns the number of elements with key 
	 *   that compares equivalent to the specified argument,
	 *   which is either 1 or 0 
	 *     since this container does not allow duplicates.
	 * The default method of check the equivalence is !(a < b || b > a)
	 */
	size_t count(const Key &key) const {
	    if ( root == nullptr ) return 0 ;
	    node *mover = root->search(key) ;
	    if ( mover == nullptr ) return 0 ;
	    return 1 ;
	}
	/**
	 * Finds an element with key equivalent to key.
	 * key value of the element to search for.
	 * Iterator to an element with key equivalent to key.
	 *   If no such element is found, past-the-end (see end()) iterator is returned.
	 */
	iterator find(const Key &key) {
	    if ( !this->count(key) ){
	        return real_end_iterator ;
	    }
	    node *mover = root->search(key) ;
	    iterator temp_it ;
	    temp_it.real_pointer = this ;
	    temp_it.node_pointer = mover ;
	    return temp_it ;
	}
	const_iterator find(const Key &key) const {
	    if ( !this->count(key) ){
	        return real_end_iterator ;
	    }
	    node *mover = root->search(key) ;
	    const_iterator temp_it ;
	    temp_it.real_pointer = this ;
	    temp_it.node_pointer = mover ;
	    return temp_it ;
	}
};


}

#endif
