#pragma once
#include <atomic>

template<typename T>
class single_ref_stack {
public:
	single_ref_stack():head(nullptr) {

	}

	~single_ref_stack() {
		//循环出栈
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
			//1 只要执行pop就对引用计数+1
			++(old_head->_ref_count);
			//2 比较head和old_head想等则交换否则说明head已经被其他线程更新
			if (head.compare_exchange_strong(old_head, old_head->_next)) {
				auto cur_count = old_head->_ref_count.load();
				auto new_count;
				//3  循环重试保证引用计数安全更新
				do {
					//4 减去本线程增加的1次和初始的1次
					new_count = cur_count - 2;
				} while (!old_head->_ref_count.compare_exchange_weak(cur_count,  new_count));

				//返回头部数据
				std::shared_ptr<T> res;
				//5  交换数据
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
		//1 数据域智能指针
		std::shared_ptr<T>  _data;
		//2 引用计数
		std::atomic<int> _ref_count;
		//3  下一个节点
		ref_node* _next;
		ref_node(T const& data_) : _data(std::make_shared<T>(data_)),
			_ref_count(1), _next(nullptr) {}
	};

	//头部节点
	std::atomic<ref_node*> head;
};
