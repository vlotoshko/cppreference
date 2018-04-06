#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <random>
#include <future>
#include "multithreading.hpp"


static thread_local int counter = 0;
std::mutex cout_mutex;
std::timed_mutex cout_timed_mutex;
std::recursive_mutex rec_mutex;

class CallAbleMutexTest
{
public:
    CallAbleMutexTest(std::string name) : threadName_(name) {}
    using type = void;

    template<typename Type = int>
    void operator()(int a, Type t);

private:
    void someCircle(int a)
    {
        for (int i = 0; i < a; ++i)
        {
            std::cout << threadName_<< ": " << std::dec << ++counter <<std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    }
    std::string threadName_;
};

template<> void CallAbleMutexTest::operator()(int a, int)
{
    cout_mutex.lock();
//    cout_mutex.lock();
    someCircle(a);
    cout_mutex.unlock();
}

template<> void CallAbleMutexTest::operator()(int a, bool)
{
    using ms = std::chrono::microseconds;
    if (cout_timed_mutex.try_lock_for(ms(100)))
    {
        someCircle(a);
        std::this_thread::sleep_for(ms(100));
        cout_timed_mutex.unlock();
    }
    else
    {
        std::cout << threadName_ << ": failed to try lock timed mutex" << std::endl;
        std::this_thread::sleep_for(ms(2000));
        if (cout_timed_mutex.try_lock_for(ms(100)))
        {
            someCircle(a);
            cout_timed_mutex.unlock();
        }
        else
        {
            std::cout << threadName_ << ": failed again to try lock timed mutex" << std::endl;
        }
    }
}

template<> void CallAbleMutexTest::operator()(int a, char)
{
    rec_mutex.lock();
    rec_mutex.lock();
    someCircle(a);
    rec_mutex.unlock();
    rec_mutex.unlock();
}

class lock_guard_stub{};
template<> void CallAbleMutexTest::operator()(int a, lock_guard_stub)
{
    std::lock_guard<std::mutex> guard{cout_mutex};
    someCircle(a);
}

class unique_lock_stub{};
template<> void CallAbleMutexTest::operator()(int a, unique_lock_stub)
{
    std::unique_lock<std::mutex> lock1 (cout_mutex, std::defer_lock);
    std::unique_lock<std::recursive_mutex> lock2 (rec_mutex, std::defer_lock);
    std::lock(lock1, lock2);
    someCircle(a);
}

class scoped_lock_stub{};
template<> void CallAbleMutexTest::operator()(int a, scoped_lock_stub)
{
    // scoped_lock available since C++17
//    std::scoped_lock(cout_mutex);
//    someCircle(a);
}

class GuardedThread : public std::thread
{
public:
    using std::thread::thread;
    GuardedThread(const GuardedThread&) = delete;
    GuardedThread& operator=(const GuardedThread&) = delete;
    ~GuardedThread() { if (joinable()) join(); }
};

void mutexTest()
{
    // Simple mutex
    auto repeat = 3;
    std::thread t1{CallAbleMutexTest{"mutex"}, repeat, int{}};
    std::thread t2{CallAbleMutexTest{"mutex"}, repeat, int{}};
    t1.join();
    t2.join();

    // Timed mutex
    std::thread t3{CallAbleMutexTest{"timed mutex 1"}, repeat, bool{}};
    std::thread t4{CallAbleMutexTest{"timed mutex 2"}, repeat, bool{}};
    t3.join();
    t4.join();

    // Recursive mutex
    std::thread t6{CallAbleMutexTest{"recursive mutex"}, repeat, char{}};
    t6.join();

    // Guarded thread
    GuardedThread gT{[](){ std::cout << "guarded thread processing" << std::endl; }};

    // Guarded mutex
    GuardedThread t7{CallAbleMutexTest{"lock guard"}, repeat, lock_guard_stub{}};
    GuardedThread t8{CallAbleMutexTest{"unique lock"}, repeat, unique_lock_stub{}};
    GuardedThread t9{CallAbleMutexTest{"scopped lock"}, repeat, scoped_lock_stub{}};

     std::vector<GuardedThread> producers;
     // TODO: fix compilation error
     // producers.emplace_back(GuardedThread{CallAbleMutexTest{"scopped lock"}, repeat, scoped_lock_stub{}});
}

// t.get_id() == std::thread::id{} when thread was:
// created without task
// terminated
// detached
// moved

void threadId()
{
    std::thread t(CallAbleMutexTest{""}, 0, int{});
    std::thread t1; // without task
    if (t.get_id() == std::thread::id{})
    {
        std::cout << "thread t is not joinable" << std::endl;
    }
    else
    {
        t.join();
    }
    if (t1.get_id() == std::thread::id{})
    {
        std::cout << "thread t1 is not joinable" << std::endl;
    }
}


void callOnce(std::once_flag & once)
{
    std::call_once(once, [](){ std::cout << " this lambda should be called once only" << std::endl; });
}

void callOnceTest()
{
    std::once_flag once;
    std::thread t1{callOnce, std::ref(once)};
    std::thread t2{callOnce, std::ref(once)};
    t1.join();
    t2.join();
}

class ConditionVariableTest
{
    std::mt19937 generator_;
    std::condition_variable addedToQueue_;
    std::queue<int> queue_;
    std::mutex lockQueue_;
    std::mutex lockPrint_;
    int threadsCount_;
    bool addedToQueueSW_;
    bool initedProducers_;
    std::vector<std::thread> producers;

    inline void initProducers()
    {
        for(int i = 0; i < threadsCount_; ++i)
        {
            producers.push_back(std::thread([this](int idx)
            {
//                {
//                     std::unique_lock<std::mutex> locker(lockPrint_);
//                     std::cout << "[produser " << idx << "]\trunning..." << std::endl;
//                }

                int workResultSimulation = 1 + generator_() % 5;
                {
                    std::this_thread::sleep_for(std::chrono::seconds(workResultSimulation));
                    std::lock_guard<std::mutex> g(lockPrint_);
                    std::cout << "[produser " << idx << "] made some work: " << workResultSimulation << std::endl;
                }

                {
                    std::lock_guard<std::mutex> g(lockQueue_);
                    queue_.push(workResultSimulation);
                    addedToQueueSW_ = true;
                    addedToQueue_.notify_one();
                }

            }, i+1));
        }
        for(int i = 0; i < threadsCount_; ++i)
        {
            producers[i].join();
        }
    }
public:
    ConditionVariableTest(int threadsCount) : generator_((unsigned int)std::chrono::system_clock::now().time_since_epoch().count()),
        threadsCount_(threadsCount), addedToQueueSW_(false), initedProducers_(false)
    {
        std::thread consumer([this]()
        {
            {
                 std::unique_lock<std::mutex> locker(lockPrint_);
                 std::cout << "[consumer]\trunning..." << std::endl;
            }

            int i = 0;
            while (i < threadsCount_)
            {
                std::unique_lock<std::mutex> locker(lockQueue_);
//                while(!addedToQueueSW_)
//                    addedToQueue_.wait(locker);
                addedToQueue_.wait(locker, [this](){return addedToQueueSW_;});

                while(!queue_.empty())
                {
                    int result = queue_.front();
                    queue_.pop();

                    std::lock_guard<std::mutex> g1(lockPrint_);
                    std::cout << "[consumer]\tgot some result: " << result << std::endl;
                    ++i;
                }
                addedToQueueSW_ = false;
            }
        });

        initProducers();
        consumer.join();
    }
};

void doAsync()
{
    auto fu = std::async(std::launch::async, []()
    {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        return 50;
    });

    int result = 0;
    if(fu.wait_for(std::chrono::seconds(0)) == std::future_status::deferred)
    {
        std::cout << "async was deferred" << std::endl;
        result = fu.get();
    }
    else
    {
        while(fu.wait_for(std::chrono::seconds(1)) != std::future_status::ready)
            std::cout << "async does not ready yet" << std::endl;
        result = fu.get();
    }

    std::cout << "async result = " << result << std::endl;
}

void promiseMe()
{
    std::promise<int> pro;
    auto fu = pro.get_future();

    std::thread t([](std::promise<int> pro)
    {
        std::cout << "promising..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        pro.set_value(33);

    }, std::move(pro));

    fu.wait();
    std::cout << "promise result = " << fu.get() << std::endl;
    t.join();
}

void testMultiThreading()
{
    std::cout << "supported concurrent threads: " << std::thread::hardware_concurrency() <<std::endl;
    mutexTest();
    threadId();
    callOnceTest();
    ConditionVariableTest cvTest(5);
    doAsync();
    promiseMe();

}


template <typename T>
class queue
{
public:
    queue() : head_(new node), tail_(head_.get()) {}
    queue(queue const& other) = delete;
    queue& operator =(queue const& other) = delete;

    std::shared_ptr<T> try_pop()
    {
        if (head_.get() == tail_)
        {
            return std::shared_ptr<T>();
        }
        std::shared_ptr<T> const res(head_->data);
        std::unique_ptr<node> old_head = std::move(head_);
        head_ = std::move(old_head->next);
        return res;
    }

    void push(T new_value)
    {
        std::shared_ptr<T> new_data = std::make_shared<T>(std::move(new_value));
        std::unique_ptr<node> p(new node);
        tail_->data = new_data;
        node * const new_tail = p.get();
        tail_->next = std::move(p);
        tail_ = new_tail;
    }

private:
    struct node
    {
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;
    };

    std::unique_ptr<node> head_;
    node* tail_;
};

template <typename T>
class threadsafe_queue
{
public:
    threadsafe_queue() : head_(new node), tail_(head_.get()) {}
    threadsafe_queue(threadsafe_queue const&) = delete;
    threadsafe_queue& operator =(threadsafe_queue const&) = delete;
    std::shared_ptr<T> tryPop();
    bool tryPop(T& value);
    std::shared_ptr<T> waitAndPop();
    void waitAndPop(T& value);
    void push(T newValue);
    bool empty();

private:
    struct node
    {
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;
    };

    std::mutex headMutex;
    std::unique_ptr<node> head_;
    std::mutex tailMutex;
    node* tail_;
    std::condition_variable dataCond;

    node* getTail()
    {
        std::lock_guard<std::mutex> tailLock(tailMutex);
        return tail_;
    }

    std::unique_ptr<node> popHead()
    {
        std::unique_ptr<node> oldHead = std::move(head_);
        head_ = std::move(oldHead->next);
        return oldHead;
    }

    std::unique_lock<std::mutex> waitForData()
    {
        std::unique_lock<std::mutex> headLock(headMutex);
        dataCond.wait(headLock, [&](){ return head_.get() != getTail(); });
        return std::move(headLock);
    }

    std::unique_ptr<node> waitPopHead()
    {
        std::unique_lock<std::mutex> headLock(waitForData());
        return popHead();
    }

    std::unique_ptr<node> waitPopHead(T& value)
    {
        std::unique_lock<std::mutex> headLock(waitForData());
        value = std::move(*head_->next);
        return popHead();
    }

    std::unique_ptr<node> tryPopHead()
    {
        std::unique_lock<std::mutex> headLock(headMutex);
        if (head_.get() == getTail())
        {
            return std::unique_ptr<node>();
        }
        return popHead();
    }

    std::unique_ptr<node> tryPopHead(T& value)
    {
        std::unique_lock<std::mutex> headLock(headMutex);
        if (head_.get() == getTail())
        {
            return std::unique_ptr<node>();
        }
        value = std::move(*head_->data);
        return popHead();
    }

};

template <typename T>
std::shared_ptr<T> threadsafe_queue<T>::waitAndPop()
{
    std::unique_ptr<node> const oldHead = waitPopHead();
    return oldHead->data;
}

template <typename T>
void threadsafe_queue<T>::waitAndPop(T& value)
{
    std::unique_ptr<node> const oldHead = waitPopHead(value);
}

template <typename T>
std::shared_ptr<T> threadsafe_queue<T>::tryPop()
{
    std::unique_ptr<node> const oldHead = tryPopHead();
    return oldHead ? oldHead->data : std::shared_ptr<T>();
}

template <typename T>
bool threadsafe_queue<T>::tryPop(T& value)
{
    std::unique_ptr<node> const oldHead = tryPopHead(value);
    return oldHead;
}

template <typename T>
bool threadsafe_queue<T>::empty()
{
    std::unique_lock<std::mutex> headLock(headMutex);
    return head_.get() == getTail();
}

template <typename T>
void threadsafe_queue<T>::push(T newValue)
{
    std::shared_ptr<T> newData(std::make_shared<T>(std::move(newValue)));
    std::unique_ptr<node> p(new node);
    {
        std::lock_guard<std::mutex> tailLock(tailMutex);
        tail_->data = newData;
        node * const newTail = p.get();
        tail_->next = std::move(p);
        tail_ = newTail;
    }
    dataCond.notify_one();
}

