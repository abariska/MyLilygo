SET(SOURCES screens/ui_mainscreen.c
    screens/ui_consolescreen.c
    screens/ui_keyboardscreen.c
    screens/ui_midicontrolscreen.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    images/ui_img_console_5653_png.c
    images/ui_img_1401235292.c
    images/ui_img_knob_png.c)

add_library(ui ${SOURCES})
