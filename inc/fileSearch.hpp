#include <algorithm>
#include <boost/filesystem.hpp>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <thread>
#include <vector>

using namespace boost::system;
namespace filesys = boost::filesystem;

/// @brief Class used to search recursively for keyword occurences in files
class FileSearch {
public:
    /// Constructor clear
    ///
    /// @param
    ///     file - file or directory from were to read
    /// @param
    ///     toSearch - keyword to be searched
    FileSearch(std::string file, std::string toSearch);

    /// Destructor
    virtual ~FileSearch() {};

    /// Function used to trigger all validations and reading of files
    void process();

private:
    std::string m_file;
    std::string m_toSearch;
    const std::uint8_t m_maxKeywordLength = 128U;
    std::vector<std::string> m_filesToRead;

    /// Verify path that was given as an argument
    ///
    /// @param
    ///     f_filePath - file or directory path that was provided
    /// @return true if the path is valid, otherwise false
    bool verifyPath(const std::string& f_filePath);

    /// extract recursively all file paths from directories
    ///
    /// @param
    ///     f_filePath file or directory from were to read
    /// @param
    ///     f_filesToRead vector used to store all file paths
    void getAllFilesInDir(const std::string& f_filePath, std::vector<std::string>& f_filesToRead);

    /// Verify if keyword to be searched that was given as an argument is valid
    ///
    /// @param
    ///     f_keyword - keyword to validate
    /// @return true if keyword length <= 128, false otherwise
    bool verifySearchKeyword(const std::string& f_keyword);

    /// Substract elements from a vector
    ///
    /// @param
    ///     f_filesToRead vector from where elements will be extracted
    /// @param
    ///     f_container vector where elements will be added
    /// @param
    ///     f_x start index from where to copy
    /// @param
    ///     f_y end index from where to stop copying
    void sliceVector(const std::vector<std::string>& f_filesToRead, std::vector<std::string>& f_container, const int& f_x, const int& f_y);

    /// Read a file line by line and search if a keyword is present inside it
    /// If it is found then print details about were this keyword was found
    /// @param
    ///     f_filePath file from were to read
    /// @param
    ///     f_toSearch keyword to be searched
    void readFile(const std::string& f_filePath, const std::string& f_toSearch);

    /// Read multiple files
    ///
    /// @param
    ///     f_filesPathsToRead vector of file paths from where to read
    /// @param
    ///     f_toSearch keyword to be searched
    void readFiles(const std::vector<std::string>& f_filesPathsToRead, const std::string& f_toSearch);
};