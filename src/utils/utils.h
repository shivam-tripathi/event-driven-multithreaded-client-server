#ifndef UTILS_H
#define UTILS_H

#include <mutex>

template <typename T>
class Node {
 public:
  T item;
  Node *next;
  Node *prev;
  Node();
  Node(T item);
  Node(T item, Node *next, Node *prev);
  void set_prev(Node *);
  void set_next(Node *);
};

template <typename T>
class ConcurrentQueue {
  Node<T> *head;
  Node<T> *tail;
  unsigned long long int size;
  mutable std::mutex mutex;

 public:
  ConcurrentQueue();
  unsigned long long int get_size();
  unsigned long long int add(T item);
  T remove();
};

#endif