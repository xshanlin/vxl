
#include <vcl/vcl_iostream.h>
#include <vcl/vcl_algorithm.h>
#include <vcl/vcl_iterator.h>
#include <vcl/vcl_vector.h>

vcl_ostream &delim(vcl_ostream &os) 
{
  //return os << endl;
  return os << ", ";
}

int main()
{
  {
    typedef vcl_vector<int> container;
    container m;
    
    m.push_back(1);
    m.push_back(2);
    
    for(container::iterator p = m.begin(); p != m.end(); ++p)
    cout << (*p) << endl;
    
    //   copy(m.begin(), m.end(), vcl_ostream_iterator<int>(cerr));
  }
  {
    vcl_vector<double> v;
    for (unsigned i=0; i<10; ++i) {
      cout << "size : " << v.size() << delim
	   << "capacity : " << v.capacity() << delim
	   << "begin : " << (void*)v.begin() << delim
	   << "end : " << (void*)v.end() << endl;
      
      v.push_back(3.1415926 * i);
    }
  }
  
  return 0;
}
