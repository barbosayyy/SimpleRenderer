#pragma once
#include <mutex>
#include <tuple>
#include <vector>
#include <thread>
#include <queue>
#include <functional>

class RenderThreadPool{
public:
    RenderThreadPool(int numWorkers);
    ~RenderThreadPool() {};

    void Start();
    void OrderStop();
    void Stop();
    void EnqueueTask(std::function<void(int, int, int, int)> task, int x0, int y0, int x1, int y1);
    void WorkerThread();
    unsigned int GetThreadIndex(std::thread::id);
    unsigned int GetNumWorkers() const {return _numWorkers;};
    void FlushRenderTasks();
    void NotifyFrameComplete();
    void WaitFrameComplete();
private:
    std::queue<std::pair<std::function<void(int, int, int, int)>, std::tuple<int, int, int, int>>> _tasks;
    std::vector<std::thread> _workers;
    int _numWorkers;
    unsigned int _numStopped;
    bool _orderedStop;
    bool _stop;

    std::atomic<bool> _aStop;
    std::atomic<bool> _aFrameComplete;
    std::atomic<int> _aTasksCompleted;
    std::atomic<int> _aTasksTotal;
    std::condition_variable _condition;
    std::mutex queueMutex;
};