#pragma once

#include <vector>
#include <string>
#include <memory>

class MapEmailAddress
{
    public:
        MapEmailAddress();

        void process_input(const std::string& input);
        std::vector<std::string> data();

    private:
        std::vector<std::string> data_;
        int unique_idx_;
};

using MapEmailAddressUPtr = std::unique_ptr<MapEmailAddress>;
