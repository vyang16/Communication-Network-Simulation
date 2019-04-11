#include <iostream>
#include <string>
#include <queue>
#include <vector>

using namespace std;
class Person{
public:
  int age;
  string name;
  Person(int a, string n): age(a), name(n){};
  ~Person(){};
};

class Student: public Person{
public:
    int id;
    Student(int a, string n, int i): Person(a,n), id(i){};
    ~Student(){};
};

struct CompareAge{
  bool operator() (Person const& p1, Person const& p2){
    return p1.age < p2.age;
  }
};
int main(int argc, char const *argv[]) {
  /* code */
  Person p = Person(14, "Anna");
  Student s = Student(13, "Steve", 1);
  priority_queue<Person, vector<Person>, CompareAge> list;
  list.push(p);
  list.push(s);
  Student s2 = list.top();
  list.pop();
  Person p2= list.top();
  list.pop();
  cout<<"Person name is "<<p2.age<<endl;
  cout<<"Student name is "<<s2.age<<endl;
  return 0;
}
