#include "RenderThreadPool.h"
#include "Console.h"
#include <thread>
#include <tuple>

RenderThreadPool::RenderThreadPool(int numWorkers) 
    : _stop(false),
    _aStop(false),
    _aFrameComplete(false),
    _aTasksCompleted(0),
    _aTasksTotal(0),
    _numWorkers((numWorkers > std::thread::hardware_concurrency()) ? _numWorkers = std::thread::hardware_concurrency() : _numWorkers = numWorkers)
{

}

void RenderThreadPool::Start()
{
    // Thread creation
    for(int i = 0; i < _numWorkers; i++){
        _workers.emplace_back([this]{this->WorkerThread();});
    }
}

void RenderThreadPool::OrderStop()
{
    _orderedStop = true;
}

void RenderThreadPool::Stop()
{
    _aStop.store(true);
    NotifyFrameComplete();
    WaitFrameComplete();
    for(std::thread &thr : _workers){
        if(thr.joinable()){
            thr.join();
        }
    }
}

void RenderThreadPool::EnqueueTask(std::function<void(int, int, int, int)> task, int x0, int y0, int x1, int y1)
{
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        _tasks.emplace(task, std::make_tuple(x0, y0, x1, y1));
        _aTasksTotal++;
    }
    _condition.notify_one();
}

void RenderThreadPool::WorkerThread()
{
    // Thread loop
    while(!_aStop.load()){
        std::pair<std::function<void(int, int, int, int)>, std::tuple<int, int, int, int>> task;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            _condition.wait(lock, [this]{return _aStop.load() || !_tasks.empty();});
            if(_aStop.load() && _tasks.empty()){
                return;
            }
            task = std::move(_tasks.front());
            _tasks.pop();
        }
        task.first(std::get<0>(task.second), std::get<1>(task.second), std::get<2>(task.second), std::get<3>(task.second));

        _aTasksCompleted++;
        if(_aTasksCompleted == _aTasksTotal){
            NotifyFrameComplete();
        }
    }
}

void RenderThreadPool::FlushRenderTasks()
{
    if(_tasks.empty() != true){
        std::queue<std::pair<std::function<void(int, int, int, int)>, std::tuple<int, int, int, int>>> empty;
        std::swap(_tasks, empty);
    }
}

void RenderThreadPool::NotifyFrameComplete()
{
    std::unique_lock<std::mutex> lock(queueMutex);
    _aFrameComplete.store(true);
    _condition.notify_all();
}

void RenderThreadPool::WaitFrameComplete()
{
    std::unique_lock<std::mutex> lock(queueMutex);
    _condition.wait(lock, [this] {return _aFrameComplete.load();});
    _aFrameComplete.store(false);
    _aTasksCompleted.store(0);
    _aTasksTotal.store(0);
    FlushRenderTasks();
}