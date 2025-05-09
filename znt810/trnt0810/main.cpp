#include "display.h"

int main() {
    weather::set_time(2);
    thread t1(increase_time); t1.detach(); // thread to increase time
    thread t2(update_weather); t2.detach(); // thread to update weather
    thread t3(update_season); t3.detach(); // thread to update season
    thread t4(check_alive_by_time); t4.detach(); // thread to check if the plant is alive every 1 second

    display x;
    x.show_scr();
}