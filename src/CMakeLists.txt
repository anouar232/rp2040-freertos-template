add_executable(${ProjectName}
    main.c
     led_green&red.c
    vibarator_sensor.c
   pressure_sensor&temp.c
    buzzer_pwm.c
    lcd.c
)

target_include_directories(${ProjectName} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)


target_link_libraries(${ProjectName} 
    pico_stdlib 
    FreeRTOS-Kernel-Heap4 
       hardware_i2c   # For I2C support
    hardware_pwm
    )

pico_add_extra_outputs(${ProjectName})