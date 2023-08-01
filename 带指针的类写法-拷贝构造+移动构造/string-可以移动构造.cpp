//----------------------------------------------------
#include <iostream>
#include <cstdio>  //snprintf()
#include <cstdlib> //RAND_MAX
#include <cstring> //strlen(), memcpy()
#include <string> 
using std::cin;
using std::cout;
using std::string;

//以下 MyString 是為了測試 containers with moveable elements 效果.  
//有长度其实可以不考虑len
class MyString { 
public: 
    static size_t DCtor;  	//累計 default-ctor 的呼叫次數 
    static size_t Ctor;  	//累計 ctor      的呼叫次數 
    static size_t CCtor;  	//累計 copy-ctor 的呼叫次數 
    static size_t CAsgn;  	//累計 copy-asgn 的呼叫次數 
    static size_t MCtor;  	//累計 move-ctor 的呼叫次數 
    static size_t MAsgn;  	//累計 move-asgn 的呼叫次數 		    
    static size_t Dtor;	//累計 dtor 的呼叫次數 
private:     
  	char* _data; 
  	size_t _len; 
  	void _init_data(const char *s) { 
    		_data = new char[_len+1]; 
    		memcpy(_data, s, _len); 
    		_data[_len] = '\0'; 
  	} 
public: 
	//default ctor
  	MyString() : _data(NULL), _len(0) { ++DCtor;  }

	//ctor
  	MyString(const char* p) : _len(strlen(p)) { 
  		++Ctor; 
    	_init_data(p); 
  	} 

	// copy ctor
  	MyString(const MyString& str) : _len(str._len) { 
		++CCtor;  	  
    	_init_data(str._data); 	//COPY
  	} 

	//move ctor, with "noexcept"
    MyString(MyString&& str) noexcept : _data(str._data), _len(str._len)  {  
        ++MCtor;    
    	str._len = 0; 		
    	str._data = NULL;  	//避免 delete (in dtor) 
 	}
 
 	//copy assignment
  	MyString& operator=(const MyString& str) { 
    	++CAsgn;  	 
		if (this != &str) { 
    		if (_data) delete _data;  
      		_len = str._len; 
      		_init_data(str._data); 	//COPY! 
    	} 
    	else {
		    // Self Assignment, Nothing to do.   
		}
    	return *this; 
  	} 

	//move assignment
   	MyString& operator=(MyString&& str) noexcept { 	 
     	++MAsgn;   	
    	if (this != &str) { 
    		if (_data) delete _data; 
      		_len = str._len; 
      		_data = str._data;	//MOVE!
      		str._len = 0; 
      		str._data = NULL; 	//避免 deleted in dtor 
    	} 
    	return *this; 
 	}
 
 	//针对简单类型 使用 new 分配后的不管是数组还是非数组形式内存空间用两种方式均可 如：
    //int *a = new int[10];   
    //delete a;   
    //delete [] a; 
    
    //dtor 
  	virtual ~MyString() { 	
  	    ++Dtor;	      	  	    
    	if (_data) {
    		delete _data; 	
		}
  	}   	
  	
  	bool 
  	operator<(const MyString& rhs) const	//為了讓 set 比較大小  
  	{
	   return std::string(this->_data) < std::string(rhs._data); 	//借用事實：string 已能比較大小. 
	}
  	bool 
  	operator==(const MyString& rhs) const	//為了讓 set 判斷相等. 
  	{
	   return std::string(this->_data) == std::string(rhs._data); 	//借用事實：string 已能判斷相等. 
	}	
	
	char* get() const { return _data; }
}; 
size_t MyString::DCtor=0;  	
size_t MyString::Ctor=0;  	 
size_t MyString::CCtor=0;
size_t MyString::CAsgn=0;
size_t MyString::MCtor=0;
size_t MyString::MAsgn=0;
size_t MyString::Dtor=0;


int main(){
	
	MyString s("nihao");
	std::cout<<s.get()<<std::endl; 
	
	return 0;
}
