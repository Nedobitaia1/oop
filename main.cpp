#include "Object.h"
#include <iostream>
#include <experimental/random>


Task* generateTask(int n, Container<Task*> *con){
  Task* result;
  std::uniform_real_distribution<double> unif(-1000000,
                                           1000000);
  std::default_random_engine re;

  int k = std::experimental::randint(0, 3);
  BinaryOperator op = (k==0) ? ADD : (k==1)? SUB : (k==2) ? MUL : DIV;
  switch (n)
  {
  case 0:
    result = new BinaryOperation(op, unif(re), unif(re), "Name1");
    break;
  case 1:
    result = new AddToContainerTask(con, generateTask(std::experimental::randint(0, 4), con));
    break;
  case 2:
    result = new CountObjectsInContainerTask(con);
    break;
  case 3:
    result = new CountTasksWithResultTask(con);
    break;
  default:
    result = new CountObjectsTask();
    break;
  }
  
  return result;
}


int main() {
  Container<Task*>* conteiner = new Container<Task*>();
  int n = std::experimental::randint(15, 20);
  for (int i = 0; i < n; ++i){
    conteiner->pushBack(generateTask(std::experimental::randint(0, 4), conteiner));
  }
  Container<std::string> *results= new Container<std::string>();
  
  std::cout<<Object::getObjectCount()<<'\n';
  Task *t;
  while(!conteiner->isEmpty()){
    t = conteiner->peekFront();
    t->execute();
    results->pushBack(t->toString());
    conteiner->removeFront();
    delete t;
  }
  ull l = results->getLength();
  for (ull i = 0; i < l; ++i){
    std::cout<<results->peekFront()<<'\n';
    results->removeFront();
  }
  results->clear();
 
  std::cout<<Object::getObjectCount()<<'\n';
  
  return 0;
}