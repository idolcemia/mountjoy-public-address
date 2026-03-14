# Mountjoy Public Address
# Giga R1 M7 + Giga Display Shield LVGL

This project is a GUI applications on the **Arduino Giga R1 M7** with the **Giga Display Shield (800×480)** using **LVGL 9**, touch input, networking utilities, and additional custom modules.

---

## Features

- LVGL-based GUI running on the Giga Display Shield  
- Touch input via `Arduino_GigaDisplayTouch`  
- Custom UI screens with the help of SquareLine  
- JSON handling using ArduinoJson
- Custom logging and diagnostic utilities  
- WiFi + HTTP wrapper modules  

---

## Project Structure (simplified)

```
/include
    lv_conf.h           <-- custom LVGL configuration - See LVGL Configuration Notes
    config.h
    Globals.h

src/
    main.cpp
    ui/                <-- LVGL UI
    events/
```

---

## LVGL Configuration Notes (Important)

By default, the **Arduino_H7_Video** library ships with its own `lv_conf.h`, which point to either `lv_conf_8.h` or `lv_conf_9.h`.
LVGL automatically checks all include paths and will always select the first `lv_conf.h` it finds.

### Problem  
LVGL kept including:

```
Arduino_H7_Video/src/lv_conf.h  (and lv_conf_9.h)
```

This caused:

- Conflicting LVGL settings  
- Redefinition warnings (e.g., `LV_USE_SYSMON`)  
- Incorrect configuration being loaded  
- Build failures when using custom features

### Solution
To force LVGL to use *correct* configuration, the following steps were taken:

1. **Renamed the Arduino_H7_Video `lv_conf.h`**  
   In:  
   ```
   .pio/packages/framework-arduino-mbed/libraries/Arduino_H7_Video/src/
   ```  
   Change:  
   ```
   lv_conf.h → lv_conf_disabled.h
   ```

2. **Placed a custom `lv_conf.h` inside `include/`**

3. **Added flags in `platformio.ini`:**

   ```ini
   build_flags =
       -Iinclude
       -DLV_CONF_INCLUDE_SIMPLE
   ```

This ensures LVGL always includes:

```
include/lv_conf.h
```

---

## UI / LVGL Setup

UI screens are generated via SquareLine Studio and placed under:

```
src/ui/
```
---

## Future Work

- 
---
