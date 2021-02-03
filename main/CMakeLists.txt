#[In this CMake file you have to put every source file to be include on the compilation process as you can below]


idf_component_register( SRCS            "main.c"
                                        "../components/leds/leds.c"
                                        "../components/tasks/tasks.c"
                                        "../components/espnow/espnow_app.c"
                                        "../components/nvs/nsv_second_dvce.c"
                                        "../components/events/events_dispatch.c"

                        INCLUDE_DIRS    "../components"".")
