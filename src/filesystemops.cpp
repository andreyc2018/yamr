#include "filesystemops.h"
#include <system_error>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

ssize_t LinuxFSOps::file_size(const std::string& filename)
{
    struct stat st_buffer;
    int rc = stat(filename.c_str(), &st_buffer);
    if (rc != 0) {
        std::error_code ec(errno, std::system_category());
        throw std::system_error(ec);
    }
    return st_buffer.st_size;
}

void LinuxFSOps::unlink(const std::string& filename)
{
    int rc = ::unlink(filename.c_str());
    if (rc != 0) {
        std::error_code ec(errno, std::system_category());
        throw std::system_error(ec);
    }
}
