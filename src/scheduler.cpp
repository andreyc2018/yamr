#include "scheduler.h"
#include "filedivider.h"
#include "mapemailaddress.h"
#include "logger.h"
#include <algorithm>

Scheduler::Scheduler()
    : n_mappers_(0)
{
}

Scheduler::~Scheduler()
{
    std::for_each(threads_.begin(), threads_.end(),
                  [](std::thread& t) { t.join(); });
}

void Scheduler::run(const std::string& filename, size_t mnum, size_t)
{
    FileDivider fd(filename, mnum);
    fd.create_chunks();
    for(auto i = fd.begin(); i != fd.end(); ++i) {
        auto m = std::make_unique<MapEmailAddress>();
        n_mappers_.fetch_add(1, std::memory_order_relaxed);
        mappers_.push_back(std::make_unique<MapRunner>(*this, m, filename, i->begin, i->end));
        threads_.emplace_back(&MapRunner::run, mappers_.back().get());
    }
    std::unique_lock<std::mutex> lock(mappers_mutex_);
    gLogger->debug("Waiting for the mappers: {}", n_mappers_.load(std::memory_order_relaxed));
    n_mappers_cv_.wait(lock, [this](){ return this->n_mappers_ < 1; });
    for(auto& m : mappers_) {
    }
}

void Scheduler::mapper_finished()
{
    n_mappers_.fetch_sub(1, std::memory_order_relaxed);
    n_mappers_cv_.notify_one();
}
