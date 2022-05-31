#include "../inc/fileSearch.hpp"

FileSearch::FileSearch(std::string file, std::string toSearch)
    : m_file(file)
    , m_toSearch(toSearch)
{
}

void FileSearch::process()
{
    std::vector<std::string> firstHalf, lastHalf;

    if (verifySearchKeyword(m_toSearch)) {
        getAllFilesInDir(m_file, m_filesToRead);

        sliceVector(m_filesToRead, firstHalf, 0, m_filesToRead.size() / 2);
        sliceVector(m_filesToRead, lastHalf, m_filesToRead.size() / 2, m_filesToRead.size());

        std::thread parseFirstHalf(&FileSearch::readFiles, this, firstHalf, m_toSearch);
        std::thread parseLastHalf(&FileSearch::readFiles, this, lastHalf, m_toSearch);

        parseFirstHalf.join();
        parseLastHalf.join();
    } else {
        std::cerr << "Keyword size is greater than maximum length of 128 characters" << std::endl;
    }
}

void FileSearch::readFile(const std::string& f_filePath, const std::string& f_toSearch)
{
    std::ifstream file(f_filePath);
    long long int offset = 0;
    std::string prefix, sufix, line, fileName;
    fileName = filesys::path(f_filePath).filename().string();
    //remove quotes from fileName
    fileName.erase(remove(fileName.begin(), fileName.end(), '"'), fileName.end());

    if (file.is_open()) {
        line = "";

        while (std::getline(file, line)) {
            size_t found = line.find(f_toSearch);

            while (found != std::string::npos) {
                if (found > 2) {
                    prefix = line.substr(found - 3, 3);
                } else {
                    prefix = line.substr(0, found);
                }

                if ((line.size() - (found + f_toSearch.size())) > 2) {
                    sufix = line.substr(found + f_toSearch.size(), 3);
                } else {
                    //take available characters after
                    sufix = line.substr(found + f_toSearch.size(), (line.size() - (found + f_toSearch.size())));
                }

                prefix = std::regex_replace(prefix, std::regex(R"(\n)"), "\\n");
                prefix = std::regex_replace(prefix, std::regex(R"(\t)"), "\\t");
                sufix = std::regex_replace(sufix, std::regex(R"(\n)"), "\\n");
                sufix = std::regex_replace(sufix, std::regex(R"(\t)"), "\\t");

                std::cout << fileName
                          << "("
                          << (offset + found)
                          << "):"
                          << prefix
                          << f_toSearch
                          << sufix
                          << std::endl;

                found = line.find(f_toSearch, found + 1);
            }
            offset += line.size();
        }
    }
}

void FileSearch::getAllFilesInDir(const std::string& f_filePath, std::vector<std::string>& f_filesToRead)
{
    if (verifyPath(f_filePath)) {
        if (filesys::is_directory(f_filePath)) { // Path provided is pointing to a directory
            // recursive directory iterators object that points to the start and end of a directory
            filesys::recursive_directory_iterator beginIter(f_filePath), endIter;

            while (beginIter != endIter) {
                try {
                    if (!filesys::is_directory(filesys::canonical(beginIter->path()))) {
                        f_filesToRead.push_back(filesys::canonical(beginIter->path()).string());
                    }
                } catch (filesys::filesystem_error& e) {
                    std::cerr << "Wrong file or directory path: " << beginIter->path() << std::endl;
                    std::cerr << e.what() << std::endl;
                    break;
                }
                ++beginIter;
            }
        } else {
            // Path provided is pointing to a file
            f_filesToRead.push_back(filesys::canonical(f_filePath).string());
        }
    }
}

bool FileSearch::verifyPath(const std::string& f_filePath)
{
    bool result = true;
    try {
        // turn the relative path into an absolute path and remove any ..
        filesys::path pathObj(filesys::canonical(f_filePath));
    } catch (filesys::filesystem_error& e) {
        result = false;
        std::cerr << "Wrong file or directory path!" << std::endl;
        std::cerr << e.what() << std::endl;
    }

    return result;
}

bool FileSearch::verifySearchKeyword(const std::string& f_keyword)
{
    bool result = true;

    if (f_keyword.size() > m_maxKeywordLength) {
        result = false;
    }

    return result;
}

void FileSearch::readFiles(const std::vector<std::string>& f_filesPathsToRead, const std::string& f_toSearch)
{
    for (std::string path : f_filesPathsToRead) {
        readFile(path, f_toSearch);
    }
}

void FileSearch::sliceVector(const std::vector<std::string>& f_filesToRead, std::vector<std::string>& f_container, const int& f_x, const int& f_y)
{
    for (int i = f_x; i < f_y; i++) {
        f_container.push_back(f_filesToRead[i]);
    }
}
