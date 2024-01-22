add_executable(main
        main.c
        lcd.h
        lcd.c
        )

# pull in common dependencies
target_link_libraries(main pico_stdlib hardware_i2c hardware_adc hardware_pwm hardware_rtc)

# create map/bin/hex file etc.
pico_add_extra_outputs(main)

# add url via pico_set_program_url
example_auto_set_url(main)
