/**
 * LRU implementation
 */
#include "buffer/lru_replacer.h"
#include "page/page.h"

namespace scudb {

// lru替换链表，头结点连接最新的页，尾部为最旧的页
template <typename T> LRUReplacer<T>::LRUReplacer() {
  head = make_shared<Node>();
  tail = make_shared<Node>();
  head->next = tail;
  tail->prev = head;
}

template <typename T> LRUReplacer<T>::~LRUReplacer() {}

/*
 * Insert value into LRU
 */
// 在lru链表中插入新页，采用头插
template <typename T> void LRUReplacer<T>::Insert(const T &value) {
  lock_guard<mutex> lck(latch);
  shared_ptr<Node> tmp;
  
  if (map.find(value) != map.end()) {
    // 如果找到该页，需要更新该页在链表中的位置
    tmp = map[value];
    shared_ptr<Node> prev = tmp->prev;
    shared_ptr<Node> latv = tmp->next;
    prev->next = latv;
    latv->prev = prev;
  } else {
    // 如果是一个新页，直接进行头插
    tmp = make_shared<Node>(value);
  }
  // 更新head
  shared_ptr<Node> fir = head->next;
  tmp->next = fir;
  fir->prev = tmp;
  tmp->prev = head;
  head->next = tmp;
  map[value] = tmp;
  return;
}

/* If LRU is non-empty, pop the tail member from LRU to argument "value", and
 * return true. If LRU is empty, return false
 */
// 由于链表靠尾部的链表越旧，因此采用链表尾部pop
template <typename T> bool LRUReplacer<T>::Victim(T &value) {
  lock_guard<mutex> lck(latch);
  
  // LRU不为空，直接返回false
  if (map.empty()) {
    return false;
  }
  // LRU链表不为空，直接pop出尾部的页，即最旧的页，更新链表
  shared_ptr<Node> last = tail->prev;
  tail->prev = last->prev;
  last->prev->next = tail;
  value = last->val;
  map.erase(last->val);
  return true;
}

/*
 * Remove value from LRU. If removal is successful, return true, otherwise
 * return false
 */
// LRU链表中删除页
template <typename T> bool LRUReplacer<T>::Erase(const T &value) {
  lock_guard<mutex> lck(latch);
  if (map.find(value) != map.end()) {
    // 需要保证在lru链表中找到该页
    shared_ptr<Node> tmp = map[value];
    tmp->prev->next = tmp->next;
    tmp->next->prev = tmp->prev;
  }
  return map.erase(value);
}
// 返回链表长度
template <typename T> size_t LRUReplacer<T>::Size() {
  lock_guard<mutex> lck(latch);
  return map.size();
}

template class LRUReplacer<Page *>;
// test only
template class LRUReplacer<int>;

} // namespace cmudb

