#pragma once
#include <atomic>

template<typename T>
class single_ref_stack {
public:
	single_ref_stack():head(nullptr) {

	}

	~single_ref_stack() {
		//ѭ����ջ
		while (pop());
	}

	void push(T const& data) {
		auto new_node = new ref_node(data);
		new_node->next = head.load();
		while (!head.compare_exchange_weak(new_node->next, new_node));
	}


	std::shared_ptr<T> pop() {
		ref_node* old_head = head.load();
		for (;;) {
			if (!old_head) {
				return std::shared_ptr<T>();
			}
			//1 ֻҪִ��pop�Ͷ����ü���+1
			++(old_head->_ref_count);
			//2 �Ƚ�head��old_head����򽻻�����˵��head�Ѿ��������̸߳���
			if (head.compare_exchange_strong(old_head, old_head->_next)) {
				auto cur_count = old_head->_ref_count.load();
				auto new_count;
				//3  ѭ�����Ա�֤���ü�����ȫ����
				do {
					//4 ��ȥ���߳����ӵ�1�κͳ�ʼ��1��
					new_count = cur_count - 2;
				} while (!old_head->_ref_count.compare_exchange_weak(cur_count,  new_count));

				//����ͷ������
				std::shared_ptr<T> res;
				//5  ��������
				res.swap(old_head->_data);

				if (old_head->_ref_count == 0) {
					delete old_head;
				}

				return res;
			}
			else {
				if (old_head->_ref_count.fetch_sub(1) == 1) {
					delete old_head;
				}
			}
		}
	}

private:
	struct ref_node {
		//1 ����������ָ��
		std::shared_ptr<T>  _data;
		//2 ���ü���
		std::atomic<int> _ref_count;
		//3  ��һ���ڵ�
		ref_node* _next;
		ref_node(T const& data_) : _data(std::make_shared<T>(data_)),
			_ref_count(1), _next(nullptr) {}
	};

	//ͷ���ڵ�
	std::atomic<ref_node*> head;
};
