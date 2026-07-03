#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include "core/command.hpp"

namespace trm::core {

    class Shell {
    public:
        Shell();
        ~Shell() = default;

        Shell(const Shell&) = delete;
        Shell& operator=(const Shell&) = delete;

        int run();

    private:
        bool m_is_running;
        
        // Command Registry: Ánh xạ từ chuỗi (tên lệnh) sang object thực thi
        std::unordered_map<std::string, std::unique_ptr<Command>> m_commands;

        void register_commands();
        
        // Cụm xử lý logic REPL
        [[nodiscard]] std::vector<std::string> tokenize(const std::string& input) const;
        int execute_line(const std::string& input);
    };

} // namespace trm::core