#include "buffer/buffer_pool_manager.h"

namespace scudb {

/*
 * BufferPoolManager Constructor
 * When log_manager is nullptr, logging is disabled (for test purpose)
 * WARNING: Do Not Edit This Function
 */
BufferPoolManager::BufferPoolManager(size_t pool_size,
                                                 DiskManager *disk_manager,
                                                 LogManager *log_manager)
    : pool_size_(pool_size), disk_manager_(disk_manager),
      log_manager_(log_manager) {
  // a consecutive memory space for buffer pool
  pages_ = new Page[pool_size_];
  page_table_ = new ExtendibleHash<page_id_t, Page *>(BUCKET_SIZE);
  replacer_ = new LRUReplacer<Page *>;
  free_list_ = new std::list<Page *>;

  // put all the pages into free list
  for (size_t i = 0; i < pool_size_; ++i) {
    free_list_->push_back(&pages_[i]);
  }
}

/*
 * BufferPoolManager Deconstructor
 * WARNING: Do Not Edit This Function
 */
BufferPoolManager::~BufferPoolManager() {
  delete[] pages_;
  delete page_table_;
  delete replacer_;
  delete free_list_;
}

/**
 * 1. search hash table.
 *  1.1 if exist, pin the page and return immediately
 *  1.2 if no exist, find a replacement entry from either free list or lru
 *      replacer. (NOTE: always find from free list first)
 * 2. If the entry chosen for replacement is dirty, write it back to disk.
 * 3. Delete the entry for the old page from the hash table and insert an
 * entry for the new page.
 * 4. Update page metadata, read page content from disk file and return page
 * pointer
 *
 * This function must mark the Page as pinned and remove its entry from LRUReplacer before it is returned to the caller.
 */
Page *BufferPoolManager::FetchPage(page_id_t page_id) {
  lock_guard<mutex> lck(latch_);
  Page *tarPageget = nullptr;

  // 如果在buffer中找到了该页，则直接返回该页
  if (page_table_->Find(page_id,tarPage)) {//1.1
    tarPage->pin_count_++;
    replacer_->Erase(tarPage);
    return tarPage;
  }
  //1.2 如果没找到，寻找一个buffer中空余的位置，如果没有这样的位置则选择一个该换的页
  // 寻找位置或换页的过程通过调用GetVictimPage实现
  tarPage = GetVictimPage();
  if (tarPage == nullptr) return tarPage;
  //2 找到的页如果是脏页，先写出再换页
  if (tarPage->is_dirty_) {
    disk_manager_->WritePage(tarPage->GetPageId(),tarPage->data_);
  }
  //3 从哈希表中删除旧页信息，并插入新表的信息
  page_table_->Remove(tarPage->GetPageId());
  page_table_->Insert(page_id,tarPage);
  //4 换页后，从buffer中读页
  disk_manager_->ReadPage(page_id,tarPage->data_);
  tarPage->pin_count_ = 1;
  tarPage->is_dirty_ = false;
  tarPage->page_id_= page_id;

  return tarPage;
}
//Page *BufferPoolManager::find

/*
 * Implementation of unpin page
 * if pin_count>0, decrement it and if it becomes zero, put it back to
 * replacer if pin_count<=0 before this call, return false. is_dirty: set the
 * dirty flag of this page
 */
// pin页面更新， 返回值是一个布尔类型，代表着是否为pin页
bool BufferPoolManager::UnpinPage(page_id_t page_id, bool is_dirty) {
  
  // 根据page_id找页  
  lock_guard<mutex> lck(latch_);
  Page *tarPage = nullptr;
  page_table_->Find(page_id,tarPage);
  
  // 空余的页认为是未被pin的，可以插入新页
  if (tarPage == nullptr) {
    return false;
  }
  
  // Pin_count小于0，代表该页未被pin
  tarPage->is_dirty_ = is_dirty;
  if (tarPage->GetPinCount() <= 0) {
    return false;
  }

  // pin_count刚变为0，该页由pin转为非pin页，进入可替换的页集中
  if (--tarPage->pin_count_ == 0) {
    replacer_->Insert(tarPage);
  }

  // 以上情况皆不是，说明pin_count > 0 ,该页仍在pin中
  return true;
}

/*
 * Used to flush a particular page of the buffer pool to disk. Should call the
 * write_page method of the disk manager
 * if page is not found in page table, return false
 * NOTE: make sure page_id != INVALID_PAGE_ID
 */
bool BufferPoolManager::FlushPage(page_id_t page_id) {
  
  // 根据id找页
  lock_guard<mutex> lck(latch_);
  Page *tarPage = nullptr;
  page_table_->Find(page_id,tarPage);

  // 若页表空余或page_id无效，返回false
  if (tarPage == nullptr || tarPage->page_id_ == INVALID_PAGE_ID) {
    return false;
  }
  // 脏页先写出
  if (tarPage->is_dirty_) {
    disk_manager_->WritePage(page_id,tarPage->GetData());
    tarPage->is_dirty_ = false;
  }

  return true;
}

/**
 * User should call this method for deleting a page. This routine will call
 * disk manager to deallocate the page. First, if page is found within page
 * table, buffer pool manager should be reponsible for removing this entry out
 * of page table, reseting page metadata and adding back to free list. Second,
 * call disk manager's DeallocatePage() method to delete from disk file. If
 * the page is found within page table, but pin_count != 0, return false
 */
// 删页函数
bool BufferPoolManager::DeletePage(page_id_t page_id) {
  // 根据id找页
  lock_guard<mutex> lck(latch_);
  Page *tarPage = nullptr;
  page_table_->Find(page_id,tarPage);
  
  if (tarPage != nullptr) {
    // 如果找到了该页
    
    // 页仍被pin，不能进行删除  
    if (tarPage->GetPinCount() > 0) {
      return false;
    }
    // 从buffer中删页并更新页表
    replacer_->Erase(tarPage);
    page_table_->Remove(page_id);
    tarPage->is_dirty_= false;
    tarPage->ResetMemory();
    free_list_->push_back(tarPage);
  }
  // 磁盘文件删除
  disk_manager_->DeallocatePage(page_id);
  return true;
}

/**
 * User should call this method if needs to create a new page. This routine
 * will call disk manager to allocate a page.
 * Buffer pool manager should be responsible to choose a victim page either
 * from free list or lru replacer(NOTE: always choose from free list first),
 * update new page's metadata, zero out memory and add corresponding entry
 * into page table. return nullptr if all the pages in pool are pinned
 */
// 创建新页的函数
Page *BufferPoolManager::NewPage(page_id_t &page_id) {
  lock_guard<mutex> lck(latch_);
  Page *tarPage = nullptr;
  // 寻找页表中的空余或者目标替换页
  tarPage = GetVictimPage();
  // tarPage为空，说明页表空余，无需进行替换
  if (tarPage == nullptr) return tarPage;

  // 获取一个新的page_id
  page_id = disk_manager_->AllocatePage();
  // 如果目标替换页为脏页，先写出
  if (tarPage->is_dirty_) {
    disk_manager_->WritePage(tarPage->GetPageId(),tarPage->data_);
  }
  // 在页表中删除旧页，插入新页
  page_table_->Remove(tarPage->GetPageId());
  page_table_->Insert(page_id,tarPage);

  // 初始化新页
  tarPage->page_id_ = page_id;
  tarPage->ResetMemory();
  tarPage->is_dirty_ = false;
  tarPage->pin_count_ = 1;

  return tarPage;
}
// GetVictimPage函数，用于获取页表中该替换的页
Page *BufferPoolManager::GetVictimPage() {
  Page *tarPage = nullptr;
  if (free_list_->empty()) {


    if (replacer_->Size() == 0) {
      return nullptr;
    }
    replacer_->Victim(tarPage);
  } else {
    // 更新可替换页集。pop链表尾结点，将插入的新结点插入头部
    tarPage = free_list_->front();
    free_list_->pop_front();
    // 检测新页id是否有效
    assert(tarPage->GetPageId() == INVALID_PAGE_ID);
  }
  // 检测新页的pin_count
  assert(tarPage->GetPinCount() == 0);
  return tarPage;
}

} // namespace cmudb

