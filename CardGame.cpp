#include <iostream>
#include <thread>

void printHello() {
    std::cout << "Hello from thread!" << std::endl;
}

int main() {
    std::thread t(printHello);  // 스레드 생성
    t.join();  // 스레드가 종료될 때까지 대기

    std::cout << "Main thread finished." << std::endl;
    return 0;
}