#ifndef CONCURRENT_QUEUE_H
#define CONCURRENT_QUEUE_H

/*
Template libraries have to be header only or have declared for the type of
import
*/

#include <mutex>

template <typename T>
/**
 * @brief
 *
 */
class Node {
 public:
  T item;
  Node *next;
  Node *prev;
  Node();
  Node(T item);
};

template <typename T>
/**
 * @brief
 *
 */
class ConcurrentQueue {
  Node<T> *head;
  Node<T> *tail;
  unsigned long long int size;
  mutable std::mutex mutex;

 public:
  /**
   * @brief Construct a new Concurrent Queue object
   */
  ConcurrentQueue();
  /**
   * @brief Get the current size of the queue. Thread safe.
   * @return unsigned long long int size of the queue
   */
  unsigned long long int get_size();
  /**
   * @brief Add a new item to the queue. Thread safe.
   * @param item
   * @return Size after inserting the current element
   */
  unsigned long long int add(T item);
  /**
   * @brief Remove an item from the queue. Thread safe.
   * @return T
   */
  T remove();
};

#include <thread>

template <typename T>
/**
 * @brief Construct a new Node< T>:: Node object
 */
Node<T>::Node() {}

template <typename T>
/**
 * @brief Construct a new Node< T>:: Node object
 * @param item Item this node will contain
 */
Node<T>::Node(T item) {
  this->item = item;
}

template <typename T>
/**
 * @brief Construct a new Concurrent Queue<T>:: Concurrent Queue object
 */
ConcurrentQueue<T>::ConcurrentQueue() {
  this->size = 0;
  this->head = this->tail = nullptr;
}

template <typename T>
unsigned long long int ConcurrentQueue<T>::get_size() {
  std::lock_guard<std::mutex> lock(this->mutex);
  return this->size;
}

template <typename T>
unsigned long long int ConcurrentQueue<T>::add(T item) {
  std::lock_guard<std::mutex> lock(this->mutex);
  Node<T> *node = new Node<T>(item);
  if (this->size == 0) {
    this->head = this->tail = node;
  } else {
    node->prev = this->tail;
    this->tail->next = node;
    this->tail = this->tail->next;
  }
  this->size++;
  return this->size;
}

template <typename T>
T ConcurrentQueue<T>::remove() {
  std::lock_guard<std::mutex> lock(this->mutex);
  if (this->size == 0) {
    throw std::runtime_error(
        "ConcurrentQueue::remove Cannot remove from empty queue");
  }
  Node<T> *node = this->head;
  if (this->tail == this->head) {
    this->head = this->tail = nullptr;
  } else {
    this->head = this->head->next;
    this->head->prev = nullptr;
  }
  T item = node->item;
  delete node;
  this->size--;
  return item;
}

#endif  // CONCURRENT_QUEUE_H
