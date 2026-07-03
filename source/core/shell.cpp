#include "core/shell.hpp"
#include <winsock2.h>
#include <windows.h>
#include <Lmcons.h> // Bắt buộc cho macro UNLEN
#include <filesystem>
#include <string>
#include <iostream>
#include <sstream>
#include <ShlObj.h>
#include <KnownFolders.h>

// Đưa các hàm helper vào anonymous namespace để tránh lỗi linker (ODR violation)
namespace {

    std::string getUsername() {
        char username[UNLEN + 1];
        DWORD size = UNLEN + 1;

        if (GetUserNameA(username, &size)) {
            return username;
        }

        return "unknown";
    }

    std::string getHostname() {
        char hostname[MAX_COMPUTERNAME_LENGTH + 1];
        DWORD size = MAX_COMPUTERNAME_LENGTH + 1;

        if (GetComputerNameA(hostname, &size)) {
            return hostname;
        }

        return "unknown";
    }

    std::filesystem::path getHomePath() {
        PWSTR path = nullptr;

        if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Profile, 0, nullptr, &path))) {
            std::filesystem::path home(path);
            CoTaskMemFree(path);
            return home;
        }

        return {};
    }

    std::string getCurrentPath() {
        namespace fs = std::filesystem;

        fs::path cwd = fs::current_path();
        fs::path home = getHomePath();

        // Dùng '/' thay vì '\'
        std::string cwdStr = cwd.generic_string();

        if (home.empty()) {
            return cwdStr;
        }

        std::string homeStr = home.generic_string();

        // Đúng thư mục Home
        if (cwdStr == homeStr) {
            return "~";
        }

        // Thư mục con của Home
        if (cwdStr.rfind(homeStr + "/", 0) == 0) {
            return "~" + cwdStr.substr(homeStr.length());
        }

        return cwdStr;
    }

}

namespace trm::core {

    Shell::Shell() : m_is_running(true) {
        register_commands();
    }

    void Shell::register_commands() {
        // Nơi đây sẽ đăng ký các lệnh. 
        // Ví dụ sau này: m_commands["ls"] = std::make_unique<commands::LsCommand>();
    }

    std::vector<std::string> Shell::tokenize(const std::string& input) const {
        std::vector<std::string> tokens;
        std::istringstream stream(input);
        std::string token;
        
        // Tokenizer cơ bản: Cắt theo khoảng trắng
        // Sẽ được thay thế bằng Parser xịn hơn ở các phase sau (để xử lý dấu nháy kép, escape char...)
        while (stream >> token) {
            tokens.push_back(token);
        }
        return tokens;
    }

    int Shell::execute_line(const std::string& input) {
        auto tokens = tokenize(input);
        if (tokens.empty()) {
            return 0; // Không có gì để chạy
        }

        std::string cmd_name = tokens[0];
        tokens.erase(tokens.begin()); // Xóa tên lệnh, phần còn lại chính là mảng arguments

        // Hardcode lệnh exit ở tầng shell để phá vỡ vòng lặp REPL
        if (cmd_name == "exit") {
            m_is_running = false;
            return 0;
        }

        // Tìm lệnh trong Registry
        auto it = m_commands.find(cmd_name);
        if (it != m_commands.end()) {
            try {
                return it->second->execute(tokens);
            } catch (const std::exception& e) {
                std::cerr << "Command execution error: " << e.what() << '\n';
                return 1;
            }
        }

        std::cerr << "'" << cmd_name << "' is not recognized as an internal or external command, operable program or file.\n";
        return 127;
    }

    int Shell::run() {
        std::string input_line;
        
        std::cout << "TRM.exe [Version 0.0.000.01]\n";
        std::cout << "Copyright (c) Congg. All rights reserved.\n\n"; // Sửa 'reversed' thành 'reserved'

        // Vòng lặp REPL thực sự
        while (m_is_running) {
            // Tích hợp các hàm helper để tạo giao diện prompt đẹp mắt: user$host | path> 
            std::cout << getUsername() << "$" << getHostname() << " | " << getCurrentPath() << "> ";
            
            if (!std::getline(std::cin, input_line)) {
                std::cout << "\n[EOF detected, terminating]\n";
                break;
            }

            if (input_line.empty()) {
                continue;
            } 
            execute_line(input_line);
        }

        return 0;
    }

} // namespace trm::core