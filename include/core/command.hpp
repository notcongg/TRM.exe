#pragma once
#include <string>
#include <vector>

namespace trm::core {

    /**
     * @class Command
     * @brief Abstract base interface cho tất cả các lệnh của TRM.
     */
    class Command {
    public:
        virtual ~Command() = default;

        // Lấy metadata của lệnh (dùng cho lệnh 'help' sau này)
        [[nodiscard]] virtual std::string get_name() const = 0;
        [[nodiscard]] virtual std::string get_description() const = 0;

        /**
         * @brief Thực thi logic của lệnh.
         * @param args Danh sách tham số truyền vào (đã loại bỏ tên lệnh).
         * @return Mã exit (0 là thành công, khác 0 là lỗi).
         */
        virtual int execute(const std::vector<std::string>& args) = 0;
    };

} // namespace trm::core