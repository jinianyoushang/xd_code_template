#pragma once
#include <mutex>
#include <memory>


////β�巨ʵ�ֵ��̰߳�ȫ����
template<typename T>
class double_push_list
{
	struct node_d
	{
		std::mutex m;
		std::shared_ptr<T> data;
		std::unique_ptr<node_d> next;
		node_d() :
			next()
		{}
		node_d(T const& value) :
			data(std::make_shared<T>(value))
		{}
	};

	node_d head;
	node_d *  last_node_ptr;
	//�������һ���ڵ�ָ�����
	std::mutex   last_ptr_mtx;
public:
	double_push_list()
	{
		last_node_ptr = &head;
	}

	~double_push_list()
	{
		remove_if([](node_d const&) {return true; });
	}

	double_push_list(double_push_list const& other) = delete;
	double_push_list& operator=(double_push_list const& other) = delete;

	void push_front(T const& value)
	{
		std::unique_ptr<node_d> new_node(new node_d(value));
		std::lock_guard<std::mutex> lk(head.m);
		new_node->next = std::move(head.next);
		head.next = std::move(new_node);
		//�������һ���ڵ�
		if (head.next->next == nullptr) {
			std::lock_guard<std::mutex> last_lk(last_ptr_mtx);
			last_node_ptr = head.next.get();
		}
	}

	void push_back(T const& value) {
		//��ֹ��push_headͬʱ����
		//���ұ�֤ͷ������ɾ���ڵ����last_node_ptrΨһ, ����ͬʱ����
		std::unique_ptr<node_d> new_node(new node_d(value));
		std::lock(last_node_ptr->m, last_ptr_mtx);
		std::unique_lock<std::mutex> lk(last_node_ptr->m, std::adopt_lock);
		std::unique_lock<std::mutex> last_lk(last_ptr_mtx, std::adopt_lock);
		//ԭ�������ڵ����һ���ڵ�ָ�������ɵĽڵ�
		last_node_ptr->next = std::move(new_node);
		//�����һ���ڵ����
		last_node_ptr = last_node_ptr->next.get();
	}

	template<typename Function>
	void for_each(Function f)
	{
		node_d* current = &head;
		std::unique_lock<std::mutex> lk(head.m);
		while (node_d* const next = current->next.get())
		{
			std::unique_lock<std::mutex> next_lk(next->m);
			lk.unlock();
			f(*next->data);
			current = next;
			lk = std::move(next_lk);
		}
	}

	template<typename Predicate>
	std::shared_ptr<T> find_first_if(Predicate p)
	{
		node_d* current = &head;
		std::unique_lock<std::mutex> lk(head.m);
		while (node_d* const next = current->next.get())
		{
			std::unique_lock<std::mutex> next_lk(next->m);
			lk.unlock();
			if (p(*next->data))
			{
				return next->data;
			}
			current = next;
			lk = std::move(next_lk);
		}
		return std::shared_ptr<T>();
	}

	template<typename Predicate>
	void remove_if(Predicate p)
	{
		node_d* current = &head;
		std::unique_lock<std::mutex> lk(head.m);
		while (node_d* const next = current->next.get())
		{
			std::unique_lock<std::mutex> next_lk(next->m);
			if (p(*next->data))
			{
				std::unique_ptr<node_d> old_next = std::move(current->next);
				current->next = std::move(next->next);
				//�ж�ɾ�����Ƿ�Ϊ���һ���ڵ�
				if (current->next == nullptr) {
					std::lock_guard<std::mutex> last_lk(last_ptr_mtx);
					last_node_ptr = current;
				}
				next_lk.unlock();
			}
			else
			{
				lk.unlock();
				current = next;
				lk = std::move(next_lk);
			}
		}
	}

	template<typename Predicate>
	bool remove_first(Predicate p)
	{
		node_d* current = &head;
		std::unique_lock<std::mutex> lk(head.m);
		while (node_d* const next = current->next.get())
		{
			std::unique_lock<std::mutex> next_lk(next->m);
			if (p(*next->data))
			{
				std::unique_ptr<node_d> old_next = std::move(current->next);
				current->next = std::move(next->next);
				//�ж�ɾ�����Ƿ�Ϊ���һ���ڵ�
				if (current->next == nullptr) {
					std::lock_guard<std::mutex> last_lk(last_ptr_mtx);
					last_node_ptr = current;
				}
				next_lk.unlock();

				return true;
			}

			lk.unlock();
			current = next;
			lk = std::move(next_lk);
		}

		return false;
	}

	//�Ӷ��Դ���ѯ���������ĵ�һ����֮ǰ����
	template<typename Predicate>
	void insert_if(Predicate p, T const & value)
	{
		node_d* current = &head;
		std::unique_lock<std::mutex> lk(head.m);
		while(node_d * const next = current->next.get())
		{
			std::unique_lock<std::mutex> next_lk(next->m);
			if(p(*(next->data)))
			{
				std::unique_ptr<node_d> new_node(new node_d(value));

				//���浱ǰ�ڵ����һ���ڵ���Ϊ�ɽڵ�
				auto old_next = std::move(current->next);

				//�½ڵ�ָ��ǰ�ڵ����һ���ڵ�
				new_node->next = std::move(old_next);
				//��ǰ�ڵ����һ���ڵ����Ϊ�½ڵ�
				current->next = std::move(new_node);
				return;
			}
			lk.unlock();
			current = next;
			lk = std::move(next_lk);
		}
	}
};