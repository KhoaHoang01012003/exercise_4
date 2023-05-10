#include <iostream>
#include <locale>

int main()
{
    // Đặt kiểu mã hóa đầu ra sang UTF-16
    std::wcout.imbue(std::locale("en_US.UTF-16"));

    // Xuất chuỗi ký tự rộng (wide characters) tiếng Việt
    std::wcout << L"Xin chào thế giới!" << std::endl;

    return 0;
}
