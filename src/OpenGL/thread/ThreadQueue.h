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
    void AddItem(QueueItemType *item);
    QueueItemType* WaitItem();

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
