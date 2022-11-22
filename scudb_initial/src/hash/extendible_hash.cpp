#include <list>

#include "hash/extendible_hash.h"
#include "page/page.h"
using namespace std;

namespace scudb {

/*
 * constructor
 * array_size: fixed array size for each bucket
 */
template <typename K, typename V>
ExtendibleHash<K, V>::ExtendibleHash(size_t size) :  globalDepth(0),bucketSize(size),bucketNum(1) {
  buckets.push_back(make_shared<Bucket>(0));
}
template<typename K, typename V>
ExtendibleHash<K, V>::ExtendibleHash() {
  ExtendibleHash(64);
}

/*
 * helper function to calculate the hashing address of input key
 */
// 使用哈希函数计算输入的key对应的哈希地址
template <typename K, typename V>
size_t ExtendibleHash<K, V>::HashKey(const K &key) const{
  return hash<K>{}(key);
}

/*
 * helper function to return global depth of hash table
 * NOTE: you must implement this function in order to pass test
 */
// 返回哈希表的全局深度，全局深度决定了每个桶的局部深度的最大值
template <typename K, typename V>
int ExtendibleHash<K, V>::GetGlobalDepth() const{
  lock_guard<mutex> lock(latch);
  return globalDepth;
}

/*
 * helper function to return local depth of one specific bucket
 * NOTE: you must implement this function in order to pass test
 */
// 获取一个特定分组（桶）中的局部深度
template <typename K, typename V>
int ExtendibleHash<K, V>::GetLocalDepth(int bucket_id) const {
  //lock_guard<mutex> lck2(latch);
  if (buckets[bucket_id]) {
    // 需要保证有这个桶

    lock_guard<mutex> lck(buckets[bucket_id]->latch);
    
    //同时也需要保证桶的大小不为0 
    if (buckets[bucket_id]->kmap.size() == 0) return -1;
    return buckets[bucket_id]->localDepth;
  }
  return -1;
}

/*
 * helper function to return current number of bucket in hash table
 */
// 计算目前在整个表中的分组（桶）数
template <typename K, typename V>
int ExtendibleHash<K, V>::GetNumBuckets() const{
  lock_guard<mutex> lock(latch);
  return bucketNum;
}

/*
 * lookup function to find value associate with input key
 */
// 查找与输入的key对应的value值
template <typename K, typename V>
bool ExtendibleHash<K, V>::Find(const K &key, V &value) {

  int idx = getIdx(key);
  lock_guard<mutex> lck(buckets[idx]->latch);
  if (buckets[idx]->kmap.find(key) != buckets[idx]->kmap.end()) {
     // 如果找到了这个key对应的桶，返回value值
     value = buckets[idx]->kmap[key];
    return true;

  }
  return false;
}

// 获取输入key对应的哈希地址的后几位，即输入key对应第几个桶
template <typename K, typename V>
int ExtendibleHash<K, V>::getIdx(const K &key) const{
  lock_guard<mutex> lck(latch);
  return HashKey(key) & ((1 << globalDepth) - 1);
}

/*
 * delete <key,value> entry in hash table
 * Shrink & Combination is not required for this project
 */
// 在哈希表中删除页
template <typename K, typename V>
bool ExtendibleHash<K, V>::Remove(const K &key) {
  int idx = getIdx(key);
  lock_guard<mutex> lck(buckets[idx]->latch);
  // 找到key对应的桶
  shared_ptr<Bucket> cur = buckets[idx];
  
  // 如果在这个桶中没有找到这样的key，删除失败
  if (cur->kmap.find(key) == cur->kmap.end()) {
    return false;
  }
  // 如果找到了key对应的页，删除该页
  cur->kmap.erase(key);
  return true;
}

/*
 * insert <key,value> entry in hash table
 * Split & Redistribute bucket when there is overflow and if necessary increase
 * global depth
 */
// 在哈希表中添加元素
template <typename K, typename V>
void ExtendibleHash<K, V>::Insert(const K &key, const V &value) {
  
  // 获取key对应的桶
  int idx = getIdx(key);
  shared_ptr<Bucket> cur = buckets[idx];
  while (true) {
    lock_guard<mutex> lck(cur->latch);
    if (cur->kmap.find(key) != cur->kmap.end() || cur->kmap.size() < bucketSize) {
      // 如果桶中已经有这一个页，或者桶中仍有空余，直接将该页加入该桶
      cur->kmap[key] = value;
      break;
    }

    // 桶的目前深度
    int mask = (1 << (cur->localDepth));
    cur->localDepth++;
    

    {
      lock_guard<mutex> lck2(latch);
      if (cur->localDepth > globalDepth) {
        // 如果桶深度在插入新页后大于全局深度，就需要进行分裂
        size_t length = buckets.size();
        for (size_t i = 0; i < length; i++) {
          buckets.push_back(buckets[i]);
        }

        // 分裂后，全局深度变大
        globalDepth++;

      }
      // 简历一个新桶
      bucketNum++;
      auto newBuc = make_shared<Bucket>(cur->localDepth);

      typename map<K, V>::iterator it;
      for (it = cur->kmap.begin(); it != cur->kmap.end(); ) {
      // 旧桶元素转到新桶
         if (HashKey(it->first) & mask) {
          newBuc->kmap[it->first] = it->second;
          it = cur->kmap.erase(it);
        } else it++;
      }
      // 整理旧桶
      for (size_t i = 0; i < buckets.size(); i++) {
        if (buckets[i] == cur && (i & mask))
          buckets[i] = newBuc;
      }
    }
    idx = getIdx(key);
    cur = buckets[idx];
  }
}



template class ExtendibleHash<page_id_t, Page *>;
template class ExtendibleHash<Page *, std::list<Page *>::iterator>;
// test purpose
template class ExtendibleHash<int, std::string>;
template class ExtendibleHash<int, std::list<int>::iterator>;
template class ExtendibleHash<int, int>;
} // namespace cmudb

