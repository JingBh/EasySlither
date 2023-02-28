#include <iostream>

#include "exercise/1.h"
#include "exercise/2.h"
#include "exercise/3.h"
#include "exercise/4.h"

int main() {
    std::cout << "EasyX 练习作业" << std::endl
        << "\t22080206 敬博浩" << std::endl
        << "\t2023-02-28" << std::endl;

    std::cout << std::endl
        << "1. 小球碰撞" << std::endl
        << "2. 打砖块" << std::endl
        << "3. 鼠标画线" << std::endl
        << "4. 打飞机" << std::endl
        << "请选择项目：";

    auto choice = 0;
    std::cin >> choice;

    switch (choice)
    {
    case 1:
        doBalls();
    case 2:
        doBreakout();
    case 3:
        doMouse();
    case 4:
        doShoot();
    default:
        std::cout << "Invalid choice." << std::endl;
        return 0;
    }
}
