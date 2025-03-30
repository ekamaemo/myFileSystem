#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
using namespace std;

class FileSystem {
private:
    static constexpr int FILENAME_LENGTH = 20;
    static constexpr int INITIAL_OFFSET = 100;
    static constexpr int INITIAL_FILE_END_OFFSET = 1000;
    static constexpr char DUMP_FILE[] = "mydump";

    struct MetaFile {
        int count;
        char name[FILENAME_LENGTH];
    };

    struct FileEntry {
        MetaFile meta;
        string data;
    };

    struct FileSystemHeader {
        int offset;
        int count;
        int fileEndOffset;
    };

    struct FileTableEntry {
        char name[FILENAME_LENGTH];
        int offset;
    };

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

public:
    FileSystem() {
        // Проверяем, существует ли файловая система
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

        file.write(reinterpret_cast<const char*>(&header), sizeof(header));
        file.close();
    }

    void writeFile(const string& filename) {
        // Читаем содержимое исходного файла
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
        strncpy(newFile.meta.name, filename.c_str(), FILENAME_LENGTH - 1);
        newFile.meta.name[FILENAME_LENGTH - 1] = '\0';

        newFile.data.resize(newFile.meta.count);
        sourceFile.read(&newFile.data[0], newFile.meta.count);
        sourceFile.close();

        // Обновляем файловую систему
        FileSystemHeader header = readHeader();
        
        // Создаем запись в таблице файлов
        FileTableEntry tableEntry;
        strncpy(tableEntry.name, newFile.meta.name, FILENAME_LENGTH);
        tableEntry.offset = header.fileEndOffset;

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
    }

    void readFile(const string& filename) {
        FileSystemHeader header = readHeader();
        auto fileTable = readFileTable(header.offset, header.count);

        // Ищем файл в таблице
        const FileTableEntry* foundEntry = nullptr;
        for (const auto& entry : fileTable) {
            if (strcmp(entry.name, filename.c_str()) == 0) {
                foundEntry = &entry;
                break;
            }
        }

        if (!foundEntry) {
            throw runtime_error("File not found: " + filename);
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

    void listFiles() {
        FileSystemHeader header = readHeader();
        auto fileTable = readFileTable(header.offset, header.count);

        cout << "Files in filesystem (" << header.count << "):\n";
        for (const auto& entry : fileTable) {
            cout << " - " << entry.name << "\n";
        }
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
            } else {
                cerr << "Invalid command. Use 'w' or 'r'.\n";
                return 1;
            }
        } else if (argc == 2 && string(argv[1]) == "i") {
            fs.initialize();
            cout << "Filesystem initialized\n";
        } else if (argc == 2 && string(argv[1]) == "l") {
            fs.listFiles();
        } else {
            cerr << "Usage:\n"
                      << "  Initialize: " << argv[0] << " i\n"
                      << "  Write file: " << argv[0] << " w <filename>\n"
                      << "  Read file: " << argv[0] << " r <filename>\n"
                      << "  List files: " << argv[0] << " l\n";
            return 1;
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}