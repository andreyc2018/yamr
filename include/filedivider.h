#pragma once
#include <string>
#include <vector>

/**
 * @brief Calculates the file parts
 *
 * Take a file name and a number of parts.
 * Calculates the begins and the ends for each part.
 * Opens a file and adjusts the begins and the ends
 * to be on an end of a string.
 * A result is an array of the pairs begins and the ends.
 */
class FileDivider
{
    public:
        struct chunk_t
        {
            chunk_t(off_t b, off_t e) : begin(b), end(e) {}
            off_t begin;
            off_t end;
        };
        using chunks_t = std::vector<chunk_t>;
        using iterator = chunks_t::reverse_iterator;

        FileDivider(const std::string& filename, size_t n);

        void create_chunks();
        const chunks_t& chunks() { return chunks_; }

        iterator begin() { return chunks_.rbegin(); }
        iterator end() { return chunks_.rend(); }

    private:
        const std::string filename_;
        size_t n_;
        chunks_t chunks_;

        void adjust_begin(std::ifstream& file, off_t& begin);
        void adjust_end(std::ifstream& file, off_t& end);
        void prev_chunk(off_t& begin, off_t& end, size_t chunk_size);
};
