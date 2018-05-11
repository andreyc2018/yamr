#pragma once

#include <string>
#include <memory>

class FileSystemOps
{
    public:
        virtual ssize_t file_size(const std::string& filename) = 0;
        virtual void unlink(const std::string& filename) = 0;
};

class LinuxFSOps : public FileSystemOps
{
    public:
        ssize_t file_size(const std::string& filename) override;
        void unlink(const std::string& filename) override;
};

using FileSystemOpsUPtr = std::unique_ptr<FileSystemOps>;
