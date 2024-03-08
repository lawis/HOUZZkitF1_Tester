#!/usr/bin/env python3

GLOBAL_DEVICE_PORT = "COM3"

def device_port():
    return GLOBAL_DEVICE_PORT.split(',', -1)
    