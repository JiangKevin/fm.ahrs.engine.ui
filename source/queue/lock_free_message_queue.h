#pragma once
//
#include <atomic>
#include <vector>

template < typename T > class LockFreeMessageQueue
{
private:
    static constexpr size_t DEFAULT_CAPACITY = 1024;  // 默认队列容量
    std::vector< T >        buffer;
    std::atomic< size_t >   head;
    std::atomic< size_t >   tail;
    size_t                  capacity_;
public:
    // 无参数构造函数，使用默认容量
    LockFreeMessageQueue() : buffer( DEFAULT_CAPACITY ), head( 0 ), tail( 0 ), capacity_( DEFAULT_CAPACITY ) {}

    // 入队操作（复制值）
    bool enqueue( const T& value )
    {
        size_t current_tail = tail.load( std::memory_order_relaxed );
        size_t next_tail    = ( current_tail + 1 ) % capacity_;
        if ( next_tail == head.load( std::memory_order_acquire ) )
        {
            return false;  // 队列已满
        }
        buffer[ current_tail ] = value;
        tail.store( next_tail, std::memory_order_release );
        return true;
    }

    // 入队操作（移动值，支持右值引用）
    bool enqueue( T&& value )
    {
        size_t current_tail = tail.load( std::memory_order_relaxed );
        size_t next_tail    = ( current_tail + 1 ) % capacity_;
        if ( next_tail == head.load( std::memory_order_acquire ) )
        {
            return false;  // 队列已满
        }
        buffer[ current_tail ] = std::move( value );
        tail.store( next_tail, std::memory_order_release );
        return true;
    }

    // 出队操作（获取值）
    bool dequeue( T& value )
    {
        size_t current_head = head.load( std::memory_order_relaxed );
        if ( current_head == tail.load( std::memory_order_acquire ) )
        {
            return false;  // 队列为空
        }
        value = std::move( buffer[ current_head ] );  // 使用移动语义减少拷贝
        head.store( ( current_head + 1 ) % capacity_, std::memory_order_release );
        return true;
    }

    // 检查队列是否为空（线程安全）
    bool empty() const
    {
        return head.load( std::memory_order_acquire ) == tail.load( std::memory_order_acquire );
    }

    // 检查队列是否已满（线程安全）
    bool full() const
    {
        size_t current_tail = tail.load( std::memory_order_relaxed );
        return ( ( current_tail + 1 ) % capacity_ ) == head.load( std::memory_order_acquire );
    }

    // 获取队列当前容量（固定值，构造后不可变）
    size_t capacity() const
    {
        return capacity_;
    }
};