#include "display.h"

int main() {
    weather::set_time(2);
    thread t1(increase_time); t1.detach();
    thread t2(update_weather); t2.detach();
    thread t3(update_season); t3.detach();
    thread t4(check_alive_by_time); t4.detach();

    display x;
    x.show_scr();
}