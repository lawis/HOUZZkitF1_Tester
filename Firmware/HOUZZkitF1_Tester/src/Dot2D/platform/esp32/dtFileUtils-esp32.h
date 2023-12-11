#include "../dtFileUtils.h"

NS_DT_BEGIN


class FileutilsESP32 : public FileUtils
{

    friend class FileUtils;

protected:

    FileutilsESP32();

    bool init() override;

public:

    void setup(int8_t fss) override;

    void end(int8_t fss) override;

    virtual std::string readStringFromSpiffsPath(const std::string& filepath) const;

    virtual uint8_t* readDataFromSpiffsPath(const std::string& filepath, size_t* size) const;

    virtual bool writeStringToSpiffsPath(const std::string& content,const std::string& filepath) const;

    virtual bool writeDataToSpiffsPath(const uint8_t* content,ssize_t size,const std::string& filepath) const;

    virtual bool removeFileFromSpiffs(const std::string &filepath) const;

    virtual bool renameFileFromSpiffs(const std::string &oldFilepath, const std::string &newFilepath) const;
    
    virtual ssize_t getFileSizeFromSpiffs(const std::string &filepath) const;
    
    virtual bool isFileExistFromSpiffs(const std::string& filepath) const;

    virtual bool createDirectoryFromSpiffs(const std::string& dirPath) const;

    virtual bool removeDirectoryFromSpiffs(const std::string& dirPath) const;

    virtual bool formatSpiffs() const;
    
    virtual std::string readStringFromSdPath(const std::string& filepath) const;

    virtual uint8_t* readDataFromSdPath(const std::string& filepath, size_t* size) const;

    virtual bool writeStringToSdPath(const std::string& content,const std::string& filepath) const;

    virtual bool writeDataToSdPath(const uint8_t*content,ssize_t size,const std::string& filepath) const;

    virtual bool removeFileFromSd(const std::string &filepath) const;

    virtual bool renameFileFromSd(const std::string &oldFilepath, const std::string &newFilepath) const;
    
    virtual ssize_t getFileSizeFromSd(const std::string &filepath) const;
    
    virtual bool isFileExistFromSd(const std::string& filepath) const;

    virtual bool createDirectoryFromSd(const std::string& dirPath) const;

    virtual bool removeDirectoryFromSd(const std::string& dirPath) const;

    virtual bool formatSd() const;

};



NS_DT_END
