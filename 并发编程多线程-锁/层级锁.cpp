//对于现实开发中，我们很难保证嵌套加锁，所以尽可能将互斥操作封装为原子操作，尽量不要在一个函数里嵌套用两个锁。
//对于嵌套用锁，也可以采用权重的方式限制使用顺序。

//层级锁
class hierarchical_mutex {
public:
	explicit hierarchical_mutex(unsigned long value) :_hierarchy_value(value),
		_previous_hierarchy_value(0) {}
	hierarchical_mutex(const hierarchical_mutex&) = delete;
	hierarchical_mutex& operator=(const hierarchical_mutex&) = delete;
	void lock() {
		check_for_hierarchy_violation();
		_internal_mutex.lock();
		update_hierarchy_value();
	}

	void unlock() {
		if (_this_thread_hierarchy_value != _hierarchy_value) {
			throw std::logic_error("mutex hierarchy violated");
		}

		_this_thread_hierarchy_value = _previous_hierarchy_value;
		_internal_mutex.unlock();
	}

	bool try_lock() {
		check_for_hierarchy_violation();
		if (!_internal_mutex.try_lock()) {
			return false;
		}

		update_hierarchy_value();
		return true;
	}
private:
	std::mutex  _internal_mutex;
	//当前层级值
	unsigned long const _hierarchy_value;
	//上一次层级值
	unsigned long _previous_hierarchy_value;
	//本线程记录的层级值
	static thread_local  unsigned long  _this_thread_hierarchy_value;

	void check_for_hierarchy_violation() {
		if (_this_thread_hierarchy_value <= _hierarchy_value) {
			throw  std::logic_error("mutex  hierarchy violated");
		}
	}

	void  update_hierarchy_value() {
		_previous_hierarchy_value = _this_thread_hierarchy_value;
		_this_thread_hierarchy_value = _hierarchy_value;
	}
};

thread_local unsigned long hierarchical_mutex::_this_thread_hierarchy_value(ULONG_MAX);

void test_hierarchy_lock() {
	hierarchical_mutex  hmtx1(1000);
	hierarchical_mutex  hmtx2(500);
	std::thread t1([&hmtx1, &hmtx2]() {
		hmtx1.lock();
		hmtx2.lock();
		hmtx2.unlock();
		hmtx1.unlock();
		});

	std::thread t2([&hmtx1, &hmtx2]() {
		hmtx2.lock();
		hmtx1.lock();
		hmtx1.unlock();
		hmtx2.unlock();
		});

	t1.join();
	t2.join();
}
