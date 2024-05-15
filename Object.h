#include "Conteiner.h"
#include <map>
#include <sstream>
/*------------------------------------Abstract class Object----------------------------------*/

class Object {
protected:
  static ull objectCount;

public:
  Object() { objectCount++; }

  virtual ~Object() { objectCount--; }

  static int getObjectCount() { return objectCount; }

  virtual std::string toString() const = 0;
};

ull Object::objectCount = 0;
/*------------------------------------Abstract class Task------------------------------------*/
class Task : public Object {
public:
  Task() {};
  virtual bool hasResult() const = 0;
  virtual void execute() = 0;
  virtual std::string toString() const = 0;
};

/*------------------------------------Abstract class Named-----------------------------------*/
class Named : public Object {
protected:
  std::string name;

public:
  Named(const std::string &name) : name{name} {};
  virtual std::string toString() const override { return name; }
};

enum BinaryOperator { ADD, SUB, MUL, DIV };

/*------------------------------------Class BinaryOperation----------------------------------*/
class BinaryOperation : public Task, public Named {
private:
  double left;
  double right;
  BinaryOperator operat;
  double result;
  void add() { result = left + right; };
  void subtruct() { result = left - right; };
  void mult() { result = left * right; };
  void divide() { 
    if (right == 0) {
      throw new std::invalid_argument("Cant devide on zero");
    }
    result = left / right; 
    };

public:
  BinaryOperation(BinaryOperator op, const double &l, const double &r,
                  const std::string &name)
      : Named(name), left{l}, right{r}, operat{op}, result{0} {objectCount--;}

  ~BinaryOperation() {objectCount++;}

  bool hasResult() const override { return true; }

  void execute() override {
    switch (this->operat) {
    case ADD:
      this->add();
      break;
    case SUB:
      this->subtruct();
      break;
    case MUL:
      this->mult();
      break;
    case DIV:
      this->divide();
      break;
    }
  }

  std::string toString() const override {
    std::string operation;
    switch (this->operat) {
    case ADD:
      operation = "+";
      break;
    case SUB:
      operation = "-";
      break;
    case MUL:
      operation = "*";
      break;
    case DIV:
      operation = "/";
      break;
    }
    std::stringstream ss;
    ss << "name: " << this->name << "\n"
       << "must culculate " << this->left << operation << this->right << "\n"
       << "result: " << this->result;
    return ss.str();
  }
};

/*------------------------------------Class СountObjectsInContainerTask----------------------------------*/
class CountObjectsInContainerTask : public Task {
private:
  int result;
  const Container<Task *> *container;

public:
  CountObjectsInContainerTask(const Container<Task *> *container)
      : container{container} {}
  bool hasResult() const override { return true; }
  void execute() override { result = container->getLength(); }

  std::string toString() const override {
    return "Number of objects in container: " + std::to_string(result);
  }
};

/*------------------------------------Class AddToContainerTask--------------------------------*/
class AddToContainerTask : public Task {
private:
  Container<Task *> *container;
  Task *task;

public:
  AddToContainerTask(Container<Task *> *container, Task *task)
      : container(container), task(task) {}

  bool hasResult() const override { return false; }

  void execute() override { container->pushBack(task); }

  std::string toString() const override { return "Task added to container"; }
};

/*------------------------------------Class CountTasksWithResultTask-------------------------*/
class CountTasksWithResultTask : public Task {
private:
  const Container<Task *> *container;
  ull result;

public:
  CountTasksWithResultTask(const Container<Task *> *container)
      : container(container) {}
  bool hasResult() const override { return true; };

  void execute() override {
    result = 0;
    for (Container<Task *>::Iterator it = container->begin();
         it != container->last(); it++) {
      if ((*it)->hasResult()) {
        ++result;
      }
    }
  }

  std::string toString() const override {
    return "Number of tasks with result: " + std::to_string(result);
  }
};

/*----------------------------------Class DeleteContainerTask--------------------------------*/
class DeleteContainerTask : public Task {
private:
  Container<Task *> *container;

public:
  DeleteContainerTask(Container<Task *> *cont) : container{cont} {};
  void execute() override {
    while (!container->isEmpty()) {
      Task *t = container->peekFront();
      container->removeFront();
      delete t;
    }
  }
  bool hasResult() const override { return false; }

  std::string toString() const override { return "Container has been cleaned"; }
};

/*-----------------------------------Class CountObjectsTask----------------------------------*/
class CountObjectsTask : public Task {
private:
  ull result;

public:
  CountObjectsTask(){};
  bool hasResult() const override { return true; };

  void execute() override { result = this->objectCount; };
  
  std::string toString() const override {
    return ("Amount of Objects: " + std::to_string(result));
  }
};

