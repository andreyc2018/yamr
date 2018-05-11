#pragma once

#include "mapemailaddress.h"
#include <string>
#include <memory>

class Scheduler;

class MapRunner
{
    public:
        MapRunner(Scheduler& scheduler,
                  MapEmailAddressUPtr& m, const std::string& filename,
                  off_t begin, off_t end);

        void run();

        const std::string& filename() const { return filename_; }
        off_t begin() const { return begin_; }
        off_t end() const { return end_; }

    private:
        Scheduler& scheduler_;
        MapEmailAddressUPtr mapper_;
        const std::string filename_;
        const off_t begin_;
        const off_t end_;
};

using MapRunnerUPtr = std::unique_ptr<MapRunner>;
