//----------------------------------------------------
#include <iostream>
#include <cstdio>  //snprintf()
#include <cstdlib> //RAND_MAX
#include <cstring> //strlen(), memcpy()
#include <string> 
using std::cin;
using std::cout;
using std::string;

//���� MyString �Ǟ��˜yԇ containers with moveable elements Ч��.  
//�г�����ʵ���Բ�����len
class MyString { 
public: 
    static size_t DCtor;  	//��Ӌ default-ctor �ĺ��дΔ� 
    static size_t Ctor;  	//��Ӌ ctor      �ĺ��дΔ� 
    static size_t CCtor;  	//��Ӌ copy-ctor �ĺ��дΔ� 
    static size_t CAsgn;  	//��Ӌ copy-asgn �ĺ��дΔ� 
    static size_t MCtor;  	//��Ӌ move-ctor �ĺ��дΔ� 
    static size_t MAsgn;  	//��Ӌ move-asgn �ĺ��дΔ� 		    
    static size_t Dtor;	//��Ӌ dtor �ĺ��дΔ� 
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
    	str._data = NULL;  	//���� delete (in dtor) 
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
      		str._data = NULL; 	//���� deleted in dtor 
    	} 
    	return *this; 
 	}
 
 	//��Լ����� ʹ�� new �����Ĳ��������黹�Ƿ�������ʽ�ڴ�ռ������ַ�ʽ���� �磺
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
  	operator<(const MyString& rhs) const	//����׌ set ���^��С  
  	{
	   return std::string(this->_data) < std::string(rhs._data); 	//��������string ���ܱ��^��С. 
	}
  	bool 
  	operator==(const MyString& rhs) const	//����׌ set �Д����. 
  	{
	   return std::string(this->_data) == std::string(rhs._data); 	//��������string �����Д����. 
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
