#include "filedivider.h"
#include "filesystemops.h"
#include <fstream>

FileDivider::FileDivider(const std::string& filename, size_t n)
    : filename_(filename)
    , n_(n)
{
}

void FileDivider::adjust_begin(std::ifstream& file, off_t& begin)
{
    auto pos = file.tellg();
    std::ifstream::int_type c = '\0';
    while (begin > 0 && c != '\n') {
        file.seekg(begin - 1);
        c = file.peek();
        if (c != '\n') {
            --begin;
        }
    }
    file.seekg(pos);
}

void FileDivider::adjust_end(std::ifstream& file, off_t& end)
{
    auto pos = file.tellg();
    std::ifstream::int_type c = '\0';
    while (end > 0 && c != '\n') {
        file.seekg(end - 1);
        c = file.peek();
        if (c != '\n') {
            --end;
        }
    }
    file.seekg(pos);
}

void FileDivider::prev_chunk(off_t& begin, off_t& end, size_t chunk_size)
{
    end = begin;
    begin -= (chunk_size+1);
    if (begin < 0) {
        begin = 0;
    }
}

void FileDivider::create_chunks()
{
    LinuxFSOps fs;
    auto file_size = fs.file_size(filename_);
    auto chunk_size = file_size / n_;
    off_t begin = file_size - chunk_size;
    off_t end = begin + chunk_size;

    if (begin <= 0 && end <= 0) {
        return;
    }

    std::ifstream file(filename_);
    do {
        adjust_begin(file, begin);
        if (end != file_size) {
            adjust_end(file, end);
        }
        chunks_.emplace_back(begin, end);
        prev_chunk(begin, end, chunk_size);
    } while (begin >= 0 && end > 0);
}
