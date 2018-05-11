#include "filesystemops.h"
#include "filedivider.h"
#include "scheduler.h"
#include "logger.h"
#include "trie.h"
#include <gtest/gtest.h>
#include <fstream>

namespace {
const std::string content = "first@mail.com\n"

                            "second@aol.com\n"
                            "third@hotmail.com\n"
                            "fourth@hotmail.com\n"

                            "fith@aol.com\n"
                            "sixth@gmail.com\n"
                            "seventh@gmail.com\n"

                            "eighth@gmail.com\n"
                            "ninth@gmail.com\n"
                            "atenth@aol.com";
}

/*
 * http://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html
 */
std::string get_file_contents(const char *filename)
{
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (in) {
        std::ostringstream contents;
        contents << in.rdbuf();
        in.close();
        return(contents.str());
    }
    throw(errno);
}

TEST(YAMR, GetSize)
{
    std::ofstream file("test_file");

    std::string content = "hello, world!\n";
    file << content;
    file.close();

    LinuxFSOps fs;

    auto size = fs.file_size("test_file");

    EXPECT_EQ(content.size(), size);

    fs.unlink("test_file");
}

TEST(YAMR, Adjust_Chunks)
{
    ssize_t file_size = content.size();
    std::cout << "file_size = " << file_size << "\n";
    EXPECT_EQ(161, file_size);

    size_t n = 4;
    size_t chunk_size = file_size / n;

    EXPECT_EQ(40, chunk_size);

    FileDivider::chunks_t chunks;
    off_t begin = file_size-chunk_size;
    off_t end = begin+chunk_size;
    EXPECT_GE(begin, 0);
    EXPECT_GE(end, 0);

    do {
        std::cout << "b: begin = " << begin << " end = " << end
                  << " e-b = " << end - begin
                  << "\nsubstr = \n"
                  << content.substr(begin, end-begin);

        while (begin > 0 && content[begin-1] != '\n') {
            --begin;
        }

        while (end > 0 && content[end-1] != '\n' && end != file_size) {
            --end;
        }

        std::cout << "\na: begin = " << begin << " end = " << end
                  << " e-b = " << end - begin
                  << "\nsubstr = \n"
                  << content.substr(begin, end-begin);

        FileDivider::chunk_t part (begin, end);
        chunks.push_back(part);

        end = begin;
        begin -= (chunk_size+1);
        if (begin < 0) {
            begin = 0;
        }

    } while (begin >= 0 && end > 0);

    EXPECT_EQ(n, chunks.size());

    for(auto i = chunks.rbegin(); i != chunks.rend(); ++i) {
        auto c = *i;
        std::cout << "b = " << c.begin
                  << " e = " << c.end
                  << "\n:\n" << content.substr(c.begin, c.end-c.begin)
                  << ":\n";
    }
}

void preapre_test_file(const std::string& fname)
{
    std::ofstream test_file(fname);
    test_file << content;
    test_file.close();
}

void print_chunks(const std::string& fname, const FileDivider::chunks_t& chunks)
{
    std::ifstream file(fname);

    for(auto i = chunks.rbegin(); i != chunks.rend(); ++i) {
        auto c = *i;
        auto size = c.end-c.begin;
        std::string str(size+1, '\0');

        std::cout << "b = " << c.begin
                  << " e = " << c.end
                  << " size = " << size
                  << "\n:\n";

        file.seekg(c.begin);

        if (file.read(&str[0], size))
            std::cout << str;
        std::cout << ":\n";
    }
}

void write_chunks(const std::string& src_fname,
                  const std::string& dst_fname,
                  FileDivider& chunks)
{
    std::ifstream src_file(src_fname);
    std::ofstream dst_file(dst_fname);

    for(auto i = chunks.begin(); i != chunks.end(); ++i) {
        auto c = *i;
        auto size = c.end-c.begin;
        std::string str(size+1, '\0');

        src_file.seekg(c.begin);
        if (src_file.read(&str[0], size)) {
            dst_file.write(&str[0], size);
        }
    }
}

void adjust_begin(std::ifstream& file, off_t& begin)
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

void adjust_end(std::ifstream& file, off_t& end)
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

TEST(YAMR, Adjust_Chunks_File)
{
    preapre_test_file("test_file");

    LinuxFSOps fs;

    ssize_t file_size = fs.file_size("test_file");

    std::cout << "file_size = " << file_size << "\n";
    EXPECT_EQ(161, file_size);

    size_t n = 4;
    size_t chunk_size = file_size / n;

    EXPECT_EQ(40, chunk_size);

    FileDivider::chunks_t chunks;
    off_t begin = file_size-chunk_size;
    off_t end = begin+chunk_size;
    EXPECT_GE(begin, 0);
    EXPECT_GE(end, 0);

    std::ifstream file("test_file");

    do {
        std::cout << "begin = " << begin << " end = " << end
                  << " e-b = " << end - begin << "\n";
        adjust_begin(file, begin);
        if (end != file_size) {
            adjust_end(file, end);
        }
        std::cout << "begin = " << begin << " end = " << end
                  << " e-b = " << end - begin << "\n";

        FileDivider::chunk_t part (begin, end);
        chunks.push_back(part);

        end = begin;
        begin -= (chunk_size+1);
        if (begin < 0) {
            begin = 0;
        }

    } while (begin >= 0 && end > 0);

    file.close();

    EXPECT_EQ(n, chunks.size());

    print_chunks("test_file", chunks);

    fs.unlink("test_file");
}

TEST(ERROR, Try_Catch)
{
    LinuxFSOps fs;
    try {
        auto size = fs.file_size("test_file");
        std::cout << size << "\n";
    } catch (const std::system_error& e) {
        std::cout << "Caught a system_error: "
                  << e.code()
                  << " " << e.what() << "\n";
    }
    EXPECT_THROW(fs.file_size("test_file"), std::system_error);
}

TEST(FileDivider, CreateChunks)
{
    preapre_test_file("test_file");
    size_t n = 4;
    FileDivider divider("test_file", n);
    divider.create_chunks();

    EXPECT_EQ(n, divider.chunks().size());

    write_chunks("test_file", "dst_file", divider);

    std::string exp_str = get_file_contents("test_file");
    std::string real_str = get_file_contents("dst_file");
    EXPECT_EQ(exp_str, real_str);

    std::ifstream in_file("dst_file");
    std::string line;
    auto it = divider.begin();
    ++it;
    in_file.seekg(it->begin);
    std::getline(in_file, line);
    std::string expected = "second@aol.com";
    EXPECT_EQ(expected, line);

    it += 2;
    in_file.seekg(it->begin);
    std::getline(in_file, line);
    expected = "eighth@gmail.com";
    EXPECT_EQ(expected, line);

    in_file.close();

    LinuxFSOps fs;
    fs.unlink("test_file");
    fs.unlink("dst_file");
}

TEST(Scheduller, Run)
{
    gLogger->set_level(spdlog::level::debug);

    std::string filename = "test_file";
    preapre_test_file(filename);
    Scheduler s;
    s.run(filename, 4, 2);
}

TEST(TrieNode, Init)
{
    TrieNode root;
}

TEST(Trie, Add)
{
    Trie t;
    t.add("one");
    std::cout << t;
}
