#pragma once
#include <memory>
#include <atomic>


//最基本的无锁栈  
//如果一直多线程都在pop有可能无法回收内存，也就是待删列表一直变大
//可以放到析构函数实现最后的回收
template<typename T>
class lock_free_stack {
private:
	struct node {
		std::shared_ptr<T> data;
		node* next;
		node(T const& data_) :data(std::make_shared<T>(data_)) {}
	};
	lock_free_stack(const lock_free_stack&) = delete;
	lock_free_stack& operator = (const lock_free_stack&) = delete;
	std::atomic<node*> head;
	std::atomic<node*> to_be_deleted;
	std::atomic<int> threads_in_pop;
public:
	lock_free_stack() {}

	void push(T const& data) {
		node* const new_node = new node(data);    //⇽-- - 2			
		new_node->next = head.load();    //⇽-- - 3			
		while (!head.compare_exchange_weak(new_node->next, new_node));    //⇽-- - 4	
	}

	std::shared_ptr<T> pop() {
		++threads_in_pop;   //1 计数器首先自增，然后才执行其他操作
		node* old_head = nullptr; 	
		do {
			old_head = head.load();  //2 加载head节点给旧head存储
			if (old_head == nullptr) {
				--threads_in_pop;
				return nullptr; 
			}
		} while (!head.compare_exchange_weak(old_head, old_head->next)); // 3	比较更新head为旧head的下一个节点	
	
		std::shared_ptr<T> res;
		if (old_head)
		{
            // 4 只要有可能，就回收已删除的节点数据
			res.swap(old_head->data);    
		}
        // 5 从节点提取数据，而非复制指针
		try_reclaim(old_head);   
		return res;
	}

    void try_reclaim(node* old_head)
	{
        //1 原子变量判断仅有一个线程进入
		if(threads_in_pop == 1)
		{
			//2 当前线程把待删列表取出
            node* nodes_to_delete = to_be_deleted.exchange(nullptr);
            //3 更新原子变量获取准确状态，判断pop是否仅仅正被当前线程唯一调用
            if(!--threads_in_pop)
            {
	            //4 如果唯一调用则将待删列表删除
                delete_nodes(nodes_to_delete);
            }else if(nodes_to_delete)
            {
	            //5 如果pop还有其他线程调用且待删列表不为空，
	            //则将待删列表首节点更新给to_be_deleted
                chain_pending_nodes(nodes_to_delete);
            }
            delete old_head;
        }
        else {
            //多个线程pop竞争head节点，此时不能删除old_head
            //将其放入待删列表
            chain_pending_node(old_head);
            --threads_in_pop;
        }
	}

	static void delete_nodes(node* nodes)
	{
		while (nodes)
		{
			node* next = nodes->next;
			delete nodes;
			nodes = next;
		}
	}

	void chain_pending_node(node* n)
	{
		chain_pending_nodes(n, n);   
	}

	void chain_pending_nodes(node* first, node* last)
	{
		//1 先将last的next节点更新为待删列表的首节点
		last->next = to_be_deleted;    
		//2  借循环保证 last->next指向正确
		// 将待删列表的首节点更新为first节点
		while (!to_be_deleted.compare_exchange_weak(
			last->next, first));     
	}

	void chain_pending_nodes(node* nodes)
	{
		node* last = nodes;
		//1 沿着next指针前进到链表末端
		while (node* const next = last->next)    
		{
			last = next;
		}
		//2 将链表放入待删链表中
		chain_pending_nodes(nodes, last);
	}
};
