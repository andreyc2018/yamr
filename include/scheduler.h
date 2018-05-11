#pragma once

#include "maprunner.h"
#include <vector>
#include <thread>
#include <condition_variable>
#include <atomic>

class Scheduler
{
    public:
        using mappers_t = std::vector<MapRunnerUPtr>;
        using runners_t = std::vector<std::thread>;
        Scheduler();
        ~Scheduler();

        void run(const std::string& filename, size_t mnum, size_t rnum);

        void mapper_finished();

    private:
        const std::string filename_;
        mappers_t mappers_;
        runners_t threads_;

        std::mutex mappers_mutex_;
        std::condition_variable n_mappers_cv_;
        std::atomic_int n_mappers_;
};
