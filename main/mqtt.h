#pragma once

void mqtt_client_init();
void mqtt_client_deinit();
void mqtt_publish_value(const char* topic, double value);