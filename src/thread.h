#ifndef __SILAR_THREAD_H__
#define __SILAR_THREAD_H__

#include <thread>
#include <functional>
#include <memory>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <atomic>


namespace sylar{

class Semaphore {
public:
    Semaphore(uint32_t count = 0);
    ~Semaphore();

    void wait();
    void notify();
private:
    Semaphore(const Semaphore&) = delete;
    Semaphore(const Semaphore&&) = delete;
    Semaphore& operator= (const Semaphore&) = delete;

private:
    sem_t m_semaphore;
};

//局部锁的模板实现
template<class T>
struct ScopedLockImpl {
public:
    //构造函数
    ScopedLockImpl(T& mutex):m_mutex(mutex) {
        m_mutex.lock();
        m_locked = true;
    }

    //析构函数
    ~ScopedLockImpl() {
        unlock();
    }

    //加锁
    void lock() {
        if(!m_locked) {
            m_mutex.lock();
            m_locked = true;
        }
    }

    //解锁
    void unlock() {
        if(m_locked) {
            m_mutex.unlock();
            m_locked = false;
        }
    }

private:
    //锁
    T& m_mutex;
    //是否上锁
    bool m_locked;
};

//局部读锁的模板实现
template<class T>
struct ReadScopedLockImpl {
public:
    //构造函数
    ReadScopedLockImpl(T& mutex):m_mutex(mutex) {
        m_mutex.rdlock();
        m_locked = true;
    }

    //析构函数
    ~ReadScopedLockImpl() {
        unlock();
    }

    //加锁
    void lock() {
        if(!m_locked) {
            m_mutex.rdlock();
            m_locked = true;
        }
    }

    //解锁
    void unlock() {
        if(m_locked) {
            m_mutex.unlock();
            m_locked = false;
        }
    }

private:
    //锁
    T& m_mutex;
    //是否上锁
    bool m_locked;
};

//局部写锁的模板实现
template<class T>
struct WriteScopedLockImpl {
public:
    //构造函数
    WriteScopedLockImpl(T& mutex):m_mutex(mutex) {
        m_mutex.wrlock();
        m_locked = true;
    }

    //析构函数
    ~WriteScopedLockImpl() {
        unlock();
    }

    //加锁
    void lock() {
        if(!m_locked) {
            m_mutex.wrlock();
            m_locked = true;
        }
    }

    //解锁
    void unlock() {
        if(m_locked) {
            m_mutex.unlock();
            m_locked = false;
        }
    }

private:
    //锁
    T& m_mutex;
    //是否上锁
    bool m_locked;
};
//锁
class Mutex {
public:
    //局部锁
    typedef ScopedLockImpl<Mutex> Lock;

    //构造函数
    Mutex() {
        pthread_mutex_init(&m_mutex, nullptr);
    }

    //析构函数
    ~Mutex() {
        pthread_mutex_destroy(&m_mutex);
    }

    //加锁
    void lock () {
        pthread_mutex_lock(&m_mutex);
    }

    //解锁
    void unlock () {
        pthread_mutex_unlock(&m_mutex);
    }



private:
    //mutex
    pthread_mutex_t m_mutex;
};

//空锁 用于调试
class NullMutex {
public:
    //局部锁
    typedef ScopedLockImpl<NullMutex> Lock;

    NullMutex() {}
    ~NullMutex() {}
    void lock() {}
    void unlock() {}
};
//读写锁
class RWMutex{
public:
    //局部读锁
    typedef ReadScopedLockImpl<RWMutex> ReadLock;
    //局部写锁
    typedef WriteScopedLockImpl<RWMutex> WriteLock;

    //构造函数
    RWMutex() {
        pthread_rwlock_init(&m_lock, nullptr);
    }

    //析构函数
    ~RWMutex() {
        pthread_rwlock_destroy(&m_lock);
    }

    //加读锁
    void rdlock() {
        pthread_rwlock_rdlock(&m_lock);
    }

    //加写锁
    void wrlock() {
        pthread_rwlock_wrlock(&m_lock);
    }

    //解锁
    void unlock() {
        pthread_rwlock_unlock(&m_lock);
    }

private:
    //读写锁
    pthread_rwlock_t m_lock;
};

//空读写锁 用于调试
class NullRWMutex{
    //局部读锁
    typedef ReadScopedLockImpl<NullRWMutex> ReadLock;
    //局部写锁
    typedef WriteScopedLockImpl<NullRWMutex> WriteLock;

    //构造函数
    NullRWMutex() {}
    ~NullRWMutex() {}
    void rdlock() {}
    void wrlock() {}
    void unlock() {}
};

//自旋锁
class Spinlock{
public:
    //局部锁
    typedef ScopedLockImpl<Spinlock> Lock;

    //构造函数
    Spinlock() {
        pthread_spin_init(&m_mutex, 0);
    }
    //析构函数
    ~Spinlock() {
        pthread_spin_destroy(&m_mutex);
    }
    //上锁
    void lock() {
        pthread_spin_lock(&m_mutex);
    }
    //解锁
    void unlock() {
        pthread_spin_unlock(&m_mutex);
    }

private:
    //自旋锁
    pthread_spinlock_t m_mutex;
};

//原子锁

class CASLock {
public:
    //局部锁
    typedef ScopedLockImpl<CASLock> Lock;

    //构造函数
    CASLock() {
        m_mutex.clear();
    }
    //析构函数
    ~CASLock() {

    }
    //上锁
    void lock() {
        while(std::atomic_flag_test_and_set_explicit(&m_mutex, std::memory_order_acquire));
    }
    //解锁
    void unlock() {
        std::atomic_flag_clear_explicit(&m_mutex, std::memory_order_release);
    }
private:
    //原子状态
    volatile std::atomic_flag m_mutex;
};

class Thread{
public:
    typedef std::shared_ptr<Thread> ptr;
    Thread(std::function<void()>cb, const std::string& name);
    ~Thread();

    pid_t getId() const {return m_id;}
    const std::string& getName() const {return m_name;}

    void join();

    static Thread* GetThis();
    static const std::string& GetName();
    static void SetName(const std::string& name);

private:
    //禁止拷贝
    Thread(const Thread&) = delete;
    Thread(const Thread&&) = delete;
    Thread& operator= (const Thread&) = delete;

    static void* run(void* arg);
private:
    pid_t m_id = -1;
    pthread_t m_thread = 0;
    std::function<void()> m_cb;
    std::string m_name;
    Semaphore m_sem;
};

}

#endif