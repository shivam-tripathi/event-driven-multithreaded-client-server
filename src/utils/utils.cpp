#include "./utils.h"

#include <stdio.h>

#include <thread>

template <typename T>
Node<T>::Node() {}

template <typename T>
Node<T>::Node(T item) {
  this->item = item;
}

template <typename T>
Node<T>::Node(T item, Node* prev, Node* next) {
  this->item = item;
  this->prev = prev;
  this->next = next;
}

template <typename T>
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
  Node<T>* node = new Node<T>(item);
  if (this->head == nullptr) {
    this->head = this->tail = node;
  } else {
    node->prev = this->tail;
    this->tail->next = node;
    this->tail = this->tail->next;
  }
  this->size++;
  std::cout << this->size << " ";
  return this->size;
}

template <typename T>
T ConcurrentQueue<T>::remove() {
  std::lock_guard<std::mutex> lock(this->mutex);
  if (this->size == 0) {
    throw std::runtime_error(
        "ConcurrentQueue::remove Cannot remove from empty queue");
  }
  Node<T>* node = this->head;
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

void add_items(ConcurrentQueue<int>* queue, int begin, int end) {
  while (begin <= end) {
    queue->add(begin++);
  }
}
