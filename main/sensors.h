#pragma once

int sensors_init();
int sensors_acquire();
double sensors_temp();
double sensors_humidity();
double sensors_pressure();