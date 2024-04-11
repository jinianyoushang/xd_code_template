#pragma once
#include <atomic>


template<typename T>
class single_ref_stack {
public:
	single_ref_stack(){
	
	}

	~single_ref_stack() {
		//ѭ����ջ
		while (pop());
	}

	void push(T const& data) {
		auto new_node =  ref_node(data);
		new_node._node_ptr->_next = head.load();
		while (!head.compare_exchange_weak(new_node._node_ptr->_next, new_node));
	}

	std::shared_ptr<T> pop() {
		ref_node old_head = head.load();
		for (;;) {
			//1 ֻҪִ��pop�Ͷ����ü���+1�����µ�head��
			ref_node new_head;
			do {
				new_head = old_head;
				new_head._ref_count += 1;
			} while (!head.compare_exchange_weak(old_head, new_head));

			old_head = new_head;

			auto* node_ptr = old_head._node_ptr;
			if (node_ptr == nullptr) {
				return  std::shared_ptr<T>();
			}

			//2 �Ƚ�head��old_head����򽻻�����˵��head�Ѿ��������̸߳���
			if (head.compare_exchange_strong(old_head, node_ptr->_next)) {
				
				//Ҫ���ص�ֵ
				std::shared_ptr<T> res;
				//��������ָ��
				res.swap(node_ptr->_data);

				//���ӵ�����
				int increase_count = old_head._ref_count - 2;
				
				if (node_ptr->_dec_count.fetch_add(increase_count) == -increase_count) {
					delete node_ptr;
				}

				return res;
			}else {
				if (node_ptr->_dec_count.fetch_sub(1) == 1) {
					delete node_ptr;
				}
			}
		}
	}

private:
	struct ref_node;
	struct node {
		//1 ����������ָ��
		std::shared_ptr<T>  _data;
		//2  ��һ���ڵ�
		ref_node _next;
		node(T const& data_) : _data(std::make_shared<T>(data_)) {}

		//���ٵ�����
		std::atomic<int>  _dec_count;
	};

	struct ref_node {
		// ���ü���
		int _ref_count;
	
		node* _node_ptr;
		ref_node( T const & data_):_node_ptr(new node(data_)), _ref_count(1){}

		ref_node():_node_ptr(nullptr),_ref_count(0){}
	};

	//ͷ���ڵ�
	std::atomic<ref_node> head;
};
