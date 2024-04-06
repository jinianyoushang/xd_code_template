//C++ 17 标准shared_mutex
//C++14  提供了 shared_time_mutex
//C++11 无上述互斥，想使用可以利用boost库
class DNService {
public:
	DNService() {}
	//读操作采用共享锁
	std::string QueryDNS(std::string dnsname) {
		std::shared_lock<std::shared_mutex> shared_locks(_shared_mtx);
		auto iter = _dns_info.find(dnsname);
		if (iter != _dns_info.end()) {
			return iter->second;
		}

		return "";
	}

	//写操作采用独占锁
	void AddDNSInfo(std::string dnsname, std::string dnsentry) {
		std::lock_guard<std::shared_mutex>  guard_locks(_shared_mtx);
		_dns_info.insert(std::make_pair(dnsname, dnsentry));
	}
private:
	std::map<std::string, std::string> _dns_info;
	mutable std::shared_mutex  _shared_mtx;
};