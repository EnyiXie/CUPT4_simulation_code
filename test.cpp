#include <fstream>
#include <iostream>

int main() {
    std::ofstream testfile;
    testfile.open("/桌面/deng/software/记录光子数（ success）（复件）/build/");
    if (testfile.is_open()) {
        testfile << "This is a test." << std::endl;
        testfile.close();
        std::cout << "Test file created successfully." << std::endl;
    } else {
        std::cout << "Failed to create test file." << std::endl;
    }
    return 0;
}
