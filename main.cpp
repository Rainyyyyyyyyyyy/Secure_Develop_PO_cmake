#include <iostream>
#include <string>
#include <filesystem>
#include <vector>


#include <cryptopp/files.h>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/osrng.h>

// написать функцию шифрования
// написать функицю дешифрования
// написать функцию проверки шифрованности (не шифровать если зашифровано и наоборот)

// обернуть в класс (синглтон)




namespace fs = std::filesystem; // для сокращения

// Вспомогательная функция для вывода отступа
void printIndent(int depth) {
    for (int i = 0; i < depth; ++i) {
        std::cout << "  ";
    }
}

// Основная рекурсивная функция для обхода папок
// Теперь принимает базовый путь (root) и текущий путь (current)
void FolderTravel(const fs::path& rootPath, const fs::path& currentPath, int depth = 0) {
    try {
        // Проверяем, существует ли путь и является ли он директорией
        if (!fs::exists(currentPath)) {
            //printIndent(depth);
            std::cout << "[Errror] File/Folder does not exists: " << currentPath << std::endl;
            return;
        }
        
        if (!fs::is_directory(currentPath)) {
            printIndent(depth);
            std::cout << "[Error] It is not a folder: " << currentPath << std::endl;
            return;
        }

        // Получаем относительный путь от корневой папки
        fs::path relativePath = fs::relative(currentPath, rootPath);
        
        // Выводим текущую папку (если это корень, то выводим ".")
        printIndent(depth);
        if (currentPath == rootPath) {
            std::cout << "[Folder] . (Root path)" << std::endl;
        } else {
            std::cout << "[Folder] " << relativePath.string() << std::endl;
        }

        // Собираем все элементы, чтобы отделить папки от файлов
        std::vector<fs::directory_entry> dirs;
        std::vector<fs::directory_entry> files;

        for (const auto& entry : fs::directory_iterator(currentPath)) {
            if (entry.is_directory()) {
                dirs.push_back(entry);
            } else {
                files.push_back(entry);
            }
        }
        // dirs - стэк папок    (для них будет применена рекурсия)
        // files - стэк файлов  (будут выведены)


        // Выводим все папки (рекурсивно)
        for (const auto& dir : dirs) {
            FolderTravel(rootPath, dir.path(), depth + 1);
        }

        // Выводим все файлы
        for (const auto& file : files) {
            printIndent(depth + 1);
            fs::path fileRelativePath = fs::relative(file.path(), rootPath);
            std::cout << "[File] " << fileRelativePath.string() 
                      << " (size: " << file.file_size() << " bytes)" << std::endl;
        }

    } catch (const fs::filesystem_error& e) {
        printIndent(depth);
        std::cout << "[Error (File system)] " << e.what() << std::endl;
    } catch (const std::exception& e) {
        printIndent(depth);
        std::cout << "[Error] " << e.what() << std::endl;
    }
}

// Обёртка (т.к. в рекурсии нужно два пути - полный и относительный)
void FolderTravel(const fs::path& rootPath) {
    FolderTravel(rootPath, rootPath, 0);
}

int main() {
    // не всегда работает
    //setlocale(LC_ALL, "ru_RU.UTF-8"); // Для поддержки русского языка в консоли (Windows)

    std::cout << "Enter path to folder (enter 'exit' to shutdown program):" << std::endl;

    std::string input;
    
    while (true) {
        std::cout << "\n> ";
        std::getline(std::cin, input);

        // Проверка на выход
        if (input == "exit") {
            std::cout << "Program stopped." << std::endl;
            break;
        }

        // Проверка на пустой ввод
        if (input.empty()) {
            std::cout << "[Error] Empty input path." << std::endl;
            continue;
        }

        
        fs::path userPath(input);

        std::cout << "\nPath: " << userPath.string() << std::endl;
        std::cout << "======================================" << std::endl;
        
        // Запускаем обход папки (используем перегрузку)
        FolderTravel(userPath);
    }

    return 0;
}