/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/10 20:13
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef THREADQUEUE_H
#define THREADQUEUE_H

#include "../../common/common.h"

template<typename QueueItemType>
class ThreadQueue {
public:
    void AddItem(QueueItemType *item) {
        m_writeItemLock.lock();
        m_writeItemsQueue.push(item);
        m_writeItemLock.unlock();

        ++m_itemCount;
        std::unique_lock<std::mutex> locker(m_conditionLock);
        m_condition.notify_one();
    }

    QueueItemType* WaitItem() {
        QueueItemType* item = nullptr;
        std::unique_lock<std::mutex> locker(m_conditionLock);
        m_condition.wait(locker, [&]() { return m_itemCount > 0 || m_exit; });

        if (m_exit) return nullptr;
        --m_itemCount;

        m_readItemLock.lock();
        if (m_readItemQueue.empty()) {
            m_writeItemLock.lock();
            m_readItemQueue.swap(m_writeItemsQueue);
            m_writeItemLock.unlock();
        }

        item = m_readItemQueue.front();
        m_readItemQueue.pop();

        m_readItemLock.unlock();
        return item;
    }

private:
    std::atomic<u32> m_itemCount = 0;
    std::queue<QueueItemType*> m_readItemQueue;
    std::queue<QueueItemType*> m_writeItemsQueue;
    std::mutex m_readItemLock;
    std::mutex m_writeItemLock;
    std::mutex m_conditionLock;
    std::condition_variable m_condition;
    std::atomic<bool> m_exit = false;
};

#endif //THREADQUEUE_H
