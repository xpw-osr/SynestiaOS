#include <interrupt.h>
#include <stdlib.h>
#include <synestia_os_hal.h>
#include <kheap.h>
#include <vmm.h>
#include <tests.h>
#include <sched.h>
#include <gpu.h>
#include <gui_button.h>
#include <gui_window.h>
#include <gui_label.h>
#include <gfx2d.h>

void print_splash() {
    const char *str = "   _____                       _   _       \n"
                      "  / ____|                     | | (_)      \n"
                      " | (___  _   _ _ __   ___  ___| |_ _  __ _ \n"
                      "  \\___ \\| | | | '_ \\ / _ \\/ __| __| |/ _` |\n"
                      "  ____) | |_| | | | |  __/\\__ \\ |_| | (_| |\n"
                      " |_____/ \\__, |_| |_|\\___||___/\\__|_|\\__,_|\n"
                      "          __/ |                            \n"
                      "         |___/                          \n";

    print(str);
}

void draw_gui() {
    gfx2d_draw_logo(0, 0, 0xFFFFFF);

    GUIButton synestiaOSButton;
    gui_create_button(&synestiaOSButton);
    synestiaOSButton.component.size.height = 32;
    synestiaOSButton.component.padding.top = 12;
    gui_init_button(&synestiaOSButton, 32, 0, "SynestiaOS");
    gui_draw_button(&synestiaOSButton);

    GUILabel bar;
    gui_create_label(&bar);
    bar.component.size.width = 1024 - 32 - synestiaOSButton.component.size.width;
    bar.component.size.height = 32;
    bar.component.background.r = 0x00;
    bar.component.background.g = 0x78;
    bar.component.background.b = 0xD4;
    gui_init_label(&bar, 32 + synestiaOSButton.component.size.width, 0, "");
    gui_draw_label(&bar);

    GUILabel synestiaOSLabel;
    gui_create_label(&synestiaOSLabel);
    synestiaOSLabel.component.size.width = 100;
    gui_init_label(&synestiaOSLabel, 10, 40, "Welcome to Synestia Operation System.");
    gui_draw_label(&synestiaOSLabel);

    GUILabel synestiaOSLabel2;
    gui_create_label(&synestiaOSLabel2);
    gui_init_label(&synestiaOSLabel2, 200, 40, "Welcome to Synestia Operation System.");
    gui_draw_label(&synestiaOSLabel2);

    GUIWindow window;
    gui_create_window(&window);
    gui_init_window(&window, 100, 100, "SynestiaOS");
    gui_draw_window(&window);

    GUIWindow window1;
    gui_create_window(&window1);
    gui_init_window(&window1, 150, 150, "SynestiaOS 1");
    gui_draw_window(&window1);

    GUIWindow window2;
    gui_create_window(&window2);
    gui_init_window(&window2, 200, 200, "SynestiaOS 2");
    gui_draw_window(&window2);
}

void kernel_main(void) {

    print_splash();

    vmm_init();

    kheap_init(); // this is just for test
    __run_tests();

    kheap_init();

    gpu_init();

    draw_gui();

    init_interrupt();

    init_bsp();

    schd_init();

    schd_switch_to(thread_create_idle_thread(0));

    /**
     * swi test
     */
    swi(1);
    swi(2);

    /**
     * printf test
     */
    printf("test:%d \n", 12);
    printf("test:%s \n", "lalala");

    /**
     * heap test
     */
    KernelStatus kernelHeapInitStatus = kheap_init();
    if (kernelHeapInitStatus != OK) {
        printf("[heap] kernel heap init failed.\n");
    }
    int *testInt = (int *) kheap_alloc(sizeof(int) * 2);
    testInt[0] = 1;
    testInt[1] = 2;

    printf("test 1:%d \n", testInt[0]);
    printf("test 2:%d \n", testInt[1]);

    KernelStatus kernelHeapFreeStatus = kheap_free(testInt);
    if (kernelHeapFreeStatus != OK) {
        printf("[heap] kernel heap free failed.\n");
    }
}
