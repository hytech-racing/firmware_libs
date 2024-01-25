#include "WatchdogInterface.h"

/* Pin mode output to watchdog WD */
void WatchdogInterface::init(unsigned long curr_time) {
    watchdog_time = curr_time;
    pinMode(WATCHDOG_INPUT, OUTPUT);
}

/* Initial output to watchdog WD */
void WatchdogInterface::set_start_state() {
    digitalWrite(WATCHDOG_INPUT, HIGH);
}

/* Toggle watchdog WD to kick dog */
void WatchdogInterface::kick_watchdog(unsigned long curr_time) {
    if ((curr_time - watchdog_time) > WATCHDOG_KICK_INTERVAL) {
        watchdog_state = !watchdog_state;
        digitalWrite(WATCHDOG_INPUT, watchdog_state);
        watchdog_time = curr_time;
    }
}


