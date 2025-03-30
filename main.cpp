#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <algorithm>
#include "fileSystemApp.h"
using namespace std;

const QString FileSystem::FS_FILE = "qt_filesystem.dat";

FileSystem::FileSystem(QObject *parent) : QObject(parent)
{
    if(!QFile::exists(FS_FILE)) {
        initialize();
    }
}
class FileSystem {
private:
    static constexpr int FILENAME_LENGTH = 20;
    static constexpr int PATH_LENGTH = 100;
    static constexpr int INITIAL_OFFSET = 100;
    static constexpr int INITIAL_FILE_END_OFFSET = 1000;
    static constexpr char DUMP_FILE[] = "mydump";

    struct MetaFile {
        int count;
        char name[FILENAME_LENGTH];
        char path[PATH_LENGTH]; // Добавили путь к файлу
    };

    struct FileEntry {
        MetaFile meta;
        string data;
    };

    struct FileSystemHeader {
        int offset;
        int count;
        int fileEndOffset;
        char currentDir[PATH_LENGTH]; // Текущая директория
    };

    struct FileTableEntry {
        char name[FILENAME_LENGTH];
        char path[PATH_LENGTH]; // Путь к файлу
        int offset;
        bool isDirectory; // Флаг - это директория или файл
    };

    // Вспомогательная функция для нормализации пути
    string normalizePath(const string& path) {
        if (path.empty()) return string(currentDirectory());

        string result;
        if (path[0] == '/') {
            // Абсолютный путь
            result = path;
        } else {
            // Относительный путь
            result = string(currentDirectory()) + "/" + path;
        }
        return result;
    }

    FileSystemHeader readHeader() {
        FileSystemHeader header;
        fstream file(DUMP_FILE, ios::binary | ios::in);
        
        if (!file.is_open()) {
            throw runtime_error("Failed to open filesystem file");
        }

        file.read(reinterpret_cast<char*>(&header), sizeof(header));
        file.close();
        return header;
    }

    void writeHeader(const FileSystemHeader& header) {
        fstream file(DUMP_FILE, ios::binary | ios::in | ios::out);
        
        if (!file.is_open()) {
            throw runtime_error("Failed to open filesystem file");
        }

        file.seekp(0, ios::beg);
        file.write(reinterpret_cast<const char*>(&header), sizeof(header));
        file.close();
    }

    vector<FileTableEntry> readFileTable(int offset, int count) {
        vector<FileTableEntry> table(count);
        fstream file(DUMP_FILE, ios::binary | ios::in);
        
        if (!file.is_open()) {
            throw runtime_error("Failed to open filesystem file");
        }

        file.seekg(offset, ios::beg);
        file.read(reinterpret_cast<char*>(table.data()), count * sizeof(FileTableEntry));
        file.close();
        return table;
    }

    void writeFileTableEntry(const FileTableEntry& entry, int position) {
        fstream file(DUMP_FILE, ios::binary | ios::in | ios::out);
        
        if (!file.is_open()) {
            throw runtime_error("Failed to open filesystem file");
        }

        file.seekp(position, ios::beg);
        file.write(reinterpret_cast<const char*>(&entry), sizeof(entry));
        file.close();
    }

    const char* currentDirectory() {
        static FileSystemHeader header = readHeader();
        return header.currentDir;
    }

    bool directoryExists(const string& path) {
        auto table = readFileTable(readHeader().offset, readHeader().count);
        return any_of(table.begin(), table.end(), [&path](const FileTableEntry& e) {
            return e.isDirectory && strcmp(e.path, path.c_str()) == 0;
        });
    }

public:
    FileSystem() {
        ifstream testFile(DUMP_FILE);
        if (!testFile.good()) {
            initialize();
        }
    }

    void initialize() {
        fstream file(DUMP_FILE, ios::binary | ios::out);
        
        if (!file.is_open()) {
            throw runtime_error("Failed to initialize filesystem");
        }

        FileSystemHeader header;
        header.offset = INITIAL_OFFSET;
        header.count = 0;
        header.fileEndOffset = INITIAL_FILE_END_OFFSET;
        strcpy(header.currentDir, "/"); // Начинаем с корневой директории

        // Создаем запись для корневой директории
        FileTableEntry rootDir;
        strcpy(rootDir.name, "/");
        strcpy(rootDir.path, "/");
        rootDir.offset = 0;
        rootDir.isDirectory = true;

        file.write(reinterpret_cast<const char*>(&header), sizeof(header));
        
        // Записываем корневую директорию в таблицу
        file.seekp(header.offset, ios::beg);
        file.write(reinterpret_cast<const char*>(&rootDir), sizeof(rootDir));
        
        file.close();

        // Обновляем заголовок
        header.count = 1;
        writeHeader(header);
    }

    void createDirectory(const string& dirname) {
        string fullPath = normalizePath(dirname);
        
        // Проверяем, нет ли уже такой директории
        if (directoryExists(fullPath)) {
            throw runtime_error("Directory already exists");
        }

        FileSystemHeader header = readHeader();
        
        // Создаем запись в таблице файлов
        FileTableEntry dirEntry;
        string dirName = fullPath.substr(fullPath.find_last_of('/') + 1);
        strncpy(dirEntry.name, dirName.c_str(), FILENAME_LENGTH - 1);
        dirEntry.name[FILENAME_LENGTH - 1] = '\0';
        
        strncpy(dirEntry.path, fullPath.c_str(), PATH_LENGTH - 1);
        dirEntry.path[PATH_LENGTH - 1] = '\0';
        
        dirEntry.offset = 0; // Для директорий offset не используется
        dirEntry.isDirectory = true;

        // Записываем запись в таблицу
        int tablePosition = header.offset + header.count * sizeof(FileTableEntry);
        writeFileTableEntry(dirEntry, tablePosition);

        // Обновляем заголовок
        header.count++;
        writeHeader(header);
        
        cout << "Directory created: " << fullPath << endl;
    }

    // Функция перемещения по директориям
    void changeDirectory(const string& path) {
        string newPath = normalizePath(path);
        
        // Проверяем существование директории
        if (!directoryExists(newPath)) {
            throw runtime_error("Directory not found: " + newPath);
        }

        FileSystemHeader header = readHeader();
        strncpy(header.currentDir, newPath.c_str(), PATH_LENGTH - 1);
        header.currentDir[PATH_LENGTH - 1] = '\0';
        writeHeader(header);
        
        cout << "Current directory is now: " << newPath << endl;
    }

    void writeFile(const string& filename) {
        ifstream sourceFile(filename, ios::binary);
        if (!sourceFile.is_open()) {
            throw runtime_error("Failed to open source file: " + filename);
        }

        FileEntry newFile;
        sourceFile.seekg(0, ios::end);
        newFile.meta.count = sourceFile.tellg();
        sourceFile.seekg(0, ios::beg);

        if (filename.size() >= FILENAME_LENGTH) {
            throw runtime_error("Filename too long");
        }
        
        // Сохраняем имя файла и путь
        string nameOnly = filename.substr(filename.find_last_of("/\\") + 1);
        strncpy(newFile.meta.name, nameOnly.c_str(), FILENAME_LENGTH - 1);
        newFile.meta.name[FILENAME_LENGTH - 1] = '\0';
        
        string currentDir = currentDirectory();
        strncpy(newFile.meta.path, currentDir.c_str(), PATH_LENGTH - 1);
        newFile.meta.path[PATH_LENGTH - 1] = '\0';

        newFile.data.resize(newFile.meta.count);
        sourceFile.read(&newFile.data[0], newFile.meta.count);
        sourceFile.close();

        // Обновляем файловую систему
        FileSystemHeader header = readHeader();
        
        // Создаем запись в таблице файлов
        FileTableEntry tableEntry;
        strncpy(tableEntry.name, newFile.meta.name, FILENAME_LENGTH);
        strncpy(tableEntry.path, newFile.meta.path, PATH_LENGTH);
        tableEntry.offset = header.fileEndOffset;
        tableEntry.isDirectory = false;

        // Записываем запись в таблицу
        int tablePosition = header.offset + header.count * sizeof(FileTableEntry);
        writeFileTableEntry(tableEntry, tablePosition);

        // Записываем данные файла
        fstream file(DUMP_FILE, ios::binary | ios::in | ios::out);
        if (!file.is_open()) {
            throw runtime_error("Failed to open filesystem file");
        }

        file.seekp(header.fileEndOffset, ios::beg);
        file.write(reinterpret_cast<const char*>(&newFile.meta), sizeof(newFile.meta));
        file.write(newFile.data.data(), newFile.data.size());
        file.close();

        // Обновляем заголовок
        header.count++;
        header.fileEndOffset += sizeof(newFile.meta) + newFile.meta.count;
        writeHeader(header);
        
        cout << "File written to: " << currentDir << "/" << nameOnly << endl;
    }

    void readFile(const string& filename) {
        FileSystemHeader header = readHeader();
        auto fileTable = readFileTable(header.offset, header.count);

        string currentDir = currentDirectory();
        string fullPath = currentDir + "/" + filename;

        // Ищем файл в таблице
        const FileTableEntry* foundEntry = nullptr;
        for (const auto& entry : fileTable) {
            if (!entry.isDirectory && strcmp(entry.name, filename.c_str()) == 0 && 
                strcmp(entry.path, currentDir.c_str()) == 0) {
                foundEntry = &entry;
                break;
            }
        }

        if (!foundEntry) {
            throw runtime_error("File not found: " + fullPath);
        }

        // Читаем файл
        fstream file(DUMP_FILE, ios::binary | ios::in);
        if (!file.is_open()) {
            throw runtime_error("Failed to open filesystem file");
        }

        file.seekg(foundEntry->offset, ios::beg);
        FileEntry fileEntry;
        file.read(reinterpret_cast<char*>(&fileEntry.meta), sizeof(fileEntry.meta));
        fileEntry.data.resize(fileEntry.meta.count);
        file.read(&fileEntry.data[0], fileEntry.meta.count);
        file.close();

        // Сохраняем во временный файл
        string tempFilename = "tmp_" + string(fileEntry.meta.name);
        ofstream outFile(tempFilename, ios::binary);
        if (!outFile.is_open()) {
            throw runtime_error("Failed to create output file");
        }

        outFile.write(fileEntry.data.data(), fileEntry.data.size());
        outFile.close();

        cout << "File extracted to: " << tempFilename << endl;
    }

    void listDirectory() {
        FileSystemHeader header = readHeader();
        auto fileTable = readFileTable(header.offset, header.count);
        string currentDir = currentDirectory();

        cout << "Contents of " << currentDir << ":\n";

        // Сначала выводим поддиректории
        for (const auto& entry : fileTable) {
            if (entry.isDirectory) {
                string dirPath = entry.path;
                if (dirPath.find(currentDir) == 0 && dirPath != currentDir) {
                    size_t pos = dirPath.find('/', currentDir.size() + 1);
                    if (pos == string::npos) {
                        cout << "[DIR] " << entry.name << "\n";
                    }
                }
            }
        }

        // Затем файлы
        for (const auto& entry : fileTable) {
            if (!entry.isDirectory && strcmp(entry.path, currentDir.c_str()) == 0) {
                cout << "[FILE] " << entry.name << "\n";
            }
        }
    }

    void printWorkingDirectory() {
        cout << "Current directory: " << currentDirectory() << endl;
    }
};

int main(int argc, char** argv) {
    try {
        FileSystem fs;

        if (argc == 3) {
            string command = argv[1];
            string filename = argv[2];

            if (command == "w") {
                fs.writeFile(filename);
            } else if (command == "r") {
                fs.readFile(filename);
            } else if (command == "mkdir") {
                fs.createDirectory(filename);
            } else if (command == "cd") {
                fs.changeDirectory(filename);
            } else {
                cerr << "Invalid command. Use 'w', 'r', 'mkdir' or 'cd'.\n";
                return 1;
            }
        } else if (argc == 2) {
            string command = argv[1];
            
            if (command == "i") {
                fs.initialize();
                cout << "Filesystem initialized\n";
            } else if (command == "l") {
                fs.listDirectory();
            } else if (command == "pwd") {
                fs.printWorkingDirectory();
            } else {
                cerr << "Invalid command. Use 'i', 'l' or 'pwd'.\n";
                return 1;
            }
        } else {
            cerr << "Usage:\n"
                 << "  Initialize: " << argv[0] << " i\n"
                 << "  Write file: " << argv[0] << " w <filename>\n"
                 << "  Read file: " << argv[0] << " r <filename>\n"
                 << "  List files: " << argv[0] << " l\n"
                 << "  Create dir: " << argv[0] << " mkdir <dirname>\n"
                 << "  Change dir: " << argv[0] << " cd <path>\n"
                 << "  Show current dir: " << argv[0] << " pwd\n";
            return 1;
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}