import os
class GPIO:
    def __init__(self, gpio_index):
        self.gpio_index = gpio_index
        self.sysfs_path = f"/sys/class/gpio/gpio{gpio_index}"
        self.direction = None
        self.value = None
 
        self.export_gpio()
        self.refresh_direction()
 
    def is_exported(self):
        return os.path.exists(self.sysfs_path)
 
    def export_gpio(self):
        if not self.is_exported():
            with open("/sys/class/gpio/export", "w") as export_file:
                export_file.write(str(self.gpio_index))
 
    def unexport_gpio(self):
        if self.is_exported():
            with open("/sys/class/gpio/unexport", "w") as unexport_file:
                unexport_file.write(str(self.gpio_index))
            self.direction = None
            self.value = None
    
    def set_direction(self, direction):
        if not self.is_exported():
            raise ValueError("GPIO pin has not been exported")
 
        if direction not in ["in", "out"]:
            raise ValueError("Invalid direction. Must be 'in' or 'out'")
        
        if direction != self.direction:
            with open(f"{self.sysfs_path}/direction", "w") as direction_file:
                direction_file.write(direction)
            self.direction = direction
 
    def get_value(self):
        if not self.is_exported():
            raise ValueError("GPIO pin has not been exported")
        
        #if self.direction == "in":
        self.refresh_value()
        
        return self.value
 
    def set_value(self, value):
        if not self.is_exported():
            raise ValueError("GPIO pin has not been exported")
 
        if self.direction != "out":
            raise ValueError("Cannot set value in input mode")
        
        if value not in [0, 1]:
            raise ValueError("Invalid value. Must be 0 or 1")
        
        if value != self.value:
            with open(f"{self.sysfs_path}/value", "w") as value_file:
                value_file.write(str(value))
            self.value = value
 
    def get_direction(self):
        if not self.is_exported():
            raise ValueError("GPIO pin has not been exported")
 
        if self.direction is None:
            self.refresh_direction()
 
        return self.direction
 
    def refresh_direction(self):
        with open(f"{self.sysfs_path}/direction", "r") as direction_file:
            self.direction = direction_file.read().strip()
 
    def refresh_value(self):
        with open(f"{self.sysfs_path}/value", "r") as value_file:
            value = value_file.read().strip()
        self.value = int(value)