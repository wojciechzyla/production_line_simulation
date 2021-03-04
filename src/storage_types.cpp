//
// Created by Wojciech Żyła on 20/12/2020.
//

#include "storage_types.hpp"

Package PackageQueue::pop(){
    Package item(0);
    if(!products_.empty()){
        switch(queue_type_){
            case PackageQueueType::FIFO:
                item = std::move(products_.front());
                products_.pop_front();
                break;
            case PackageQueueType::LIFO:
                item = std::move(products_.back());
                products_.pop_back();
                break;
        }
    }
    return item;
}
