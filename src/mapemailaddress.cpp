#include "mapemailaddress.h"
#include "logger.h"

MapEmailAddress::MapEmailAddress()
    : unique_idx_(0)
{
}

void MapEmailAddress::process_input(const std::string& input)
{
    for (size_t i = 0; i < input.size(); ++i) {
    }
    data_.push_back(input);
}

std::vector<std::string> MapEmailAddress::data()
{
    return data_;
}
