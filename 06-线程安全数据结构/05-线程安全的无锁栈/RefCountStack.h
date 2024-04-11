#pragma once
template<typename T>
class ref_count_stack {
private:
	//前置声明节点类型
	struct count_node;
	struct counted_node_ptr {
		//1 外部引用计数
		int external_count;
		//2 节点地址
		count_node* ptr;
	};

	struct count_node {
		//3  数据域智能指针
		std::shared_ptr<T> data;
		//4  节点内部引用计数
		std::atomic<int>  internal_count;
		//5  下一个节点
		counted_node_ptr  next;
		count_node(T const& data_): data(std::make_shared<T>(data_)), internal_count(0) {}
	};
	//6 头部节点
	std::atomic<counted_node_ptr> head;

public:
	//增加头部节点引用数量
	void increase_head_count(counted_node_ptr& old_counter) {
		counted_node_ptr new_counter;
		
		do {
			new_counter = old_counter;
			++new_counter.external_count;
		}//7  循环判断保证head和old_counter想等时做更新,多线程情况保证引用计数原子递增。
		while (!head.compare_exchange_strong(old_counter,  new_counter, 
			std::memory_order_acquire, std::memory_order_relaxed));
		//8  走到此处说明head的external_count已经被更新了
		old_counter.external_count = new_counter.external_count;
	}

	std::shared_ptr<T> pop() {
		counted_node_ptr old_head = head.load();
		for (;;) {
			increase_head_count(old_head);
			count_node* const ptr = old_head.ptr;
			//1  判断为空责直接返回
			if (!ptr) {
				return std::shared_ptr<T>();
			}

			//2 本线程如果抢先完成head的更新
			if (head.compare_exchange_strong(old_head, ptr->next,  std::memory_order_relaxed)) {
				//返回头部数据
				std::shared_ptr<T> res;
				//交换数据
				res.swap(ptr->data);
				//3 减少外部引用计数，先统计到目前为止增加了多少外部引用
				int const count_increase = old_head.external_count - 2;
				//4 将内部引用计数添加
				if (ptr->internal_count.fetch_add(count_increase, std::memory_order_release) == -count_increase) {
					delete  ptr;
				}
				return res;
			} else if (ptr->internal_count.fetch_add(-1, std::memory_order_acquire) == 1) { //5
				//如果当前线程操作的head节点已经被别的线程更新，则减少内部引用计数
				//当前线程减少内部引用计数，返回之前值为1说明指针仅被当前线程引用
				ptr->internal_count.load(std::memory_order_acquire);
				delete ptr;
			}
		}
	}

	ref_count_stack(){
		counted_node_ptr head_node_ptr;
		//头节点开始只做标识用，没效果
		head_node_ptr.external_count = 0;
		head_node_ptr.ptr = nullptr;
		head.store(head_node_ptr);
	}

	~ref_count_stack() {
		//循环出栈
		while (pop());
	}

	void push(T const& data) {
		counted_node_ptr  new_node;
		new_node.ptr = new count_node(data);
		new_node.external_count = 1;
		new_node.ptr->next = head.load();
		while (!head.compare_exchange_weak(new_node.ptr->next, new_node, 
			std::memory_order_release, std::memory_order_relaxed));
	}

	
};
