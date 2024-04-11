#pragma once
template<typename T>
class ref_count_stack {
private:
	//ǰ�������ڵ�����
	struct count_node;
	struct counted_node_ptr {
		//1 �ⲿ���ü���
		int external_count;
		//2 �ڵ��ַ
		count_node* ptr;
	};

	struct count_node {
		//3  ����������ָ��
		std::shared_ptr<T> data;
		//4  �ڵ��ڲ����ü���
		std::atomic<int>  internal_count;
		//5  ��һ���ڵ�
		counted_node_ptr  next;
		count_node(T const& data_): data(std::make_shared<T>(data_)), internal_count(0) {}
	};
	//6 ͷ���ڵ�
	std::atomic<counted_node_ptr> head;

public:
	//����ͷ���ڵ���������
	void increase_head_count(counted_node_ptr& old_counter) {
		counted_node_ptr new_counter;
		
		do {
			new_counter = old_counter;
			++new_counter.external_count;
		}//7  ѭ���жϱ�֤head��old_counter���ʱ������,���߳������֤���ü���ԭ�ӵ�����
		while (!head.compare_exchange_strong(old_counter,  new_counter, 
			std::memory_order_acquire, std::memory_order_relaxed));
		//8  �ߵ��˴�˵��head��external_count�Ѿ���������
		old_counter.external_count = new_counter.external_count;
	}

	std::shared_ptr<T> pop() {
		counted_node_ptr old_head = head.load();
		for (;;) {
			increase_head_count(old_head);
			count_node* const ptr = old_head.ptr;
			//1  �ж�Ϊ����ֱ�ӷ���
			if (!ptr) {
				return std::shared_ptr<T>();
			}

			//2 ���߳�����������head�ĸ���
			if (head.compare_exchange_strong(old_head, ptr->next,  std::memory_order_relaxed)) {
				//����ͷ������
				std::shared_ptr<T> res;
				//��������
				res.swap(ptr->data);
				//3 �����ⲿ���ü�������ͳ�Ƶ�ĿǰΪֹ�����˶����ⲿ����
				int const count_increase = old_head.external_count - 2;
				//4 ���ڲ����ü������
				if (ptr->internal_count.fetch_add(count_increase, std::memory_order_release) == -count_increase) {
					delete  ptr;
				}
				return res;
			} else if (ptr->internal_count.fetch_add(-1, std::memory_order_acquire) == 1) { //5
				//�����ǰ�̲߳�����head�ڵ��Ѿ�������̸߳��£�������ڲ����ü���
				//��ǰ�̼߳����ڲ����ü���������֮ǰֵΪ1˵��ָ�������ǰ�߳�����
				ptr->internal_count.load(std::memory_order_acquire);
				delete ptr;
			}
		}
	}

	ref_count_stack(){
		counted_node_ptr head_node_ptr;
		//ͷ�ڵ㿪ʼֻ����ʶ�ã�ûЧ��
		head_node_ptr.external_count = 0;
		head_node_ptr.ptr = nullptr;
		head.store(head_node_ptr);
	}

	~ref_count_stack() {
		//ѭ����ջ
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
