
#ifndef CONFIG_ZTEST

#include "main.hpp"
#include <zephyr/kernel.h>
#include "State/State.hpp"

#include "Encoder/IEncoder.hpp"

#define ENCODER_UPDATE_TASK_PRIORITY -2 //high priority
#define ENCODER_UPDATE_TASK_STACK_SIZE 128

#ifdef CONFIG_SPINDLE_ENCODER_TYPE_TEST
    // Setup for TEST encoder
    #define myEncoder nullptr

#elif CONFIG_SPINDLE_ENCODER_TYPE_LS7366
    #include "Encoder/Driver/LS7366R.hpp"
    LS7366R* myEncoder;
    // Setup for LS7366 encoder
#elif CONFIG_SPINDLE_ENCODER_TYPE_SDEC

#else
    #error "No encoder type selected, see Kconfig"
    // Setup for SDEC encoder
#endif

// #if CONFIG_BOARD == stm32f411ce_blackpill
//     #if CONFIG_BOARD != rpi_pico
//         #error "Board mismatch, see README.md for supported boards"
//     #endif
// #endif

K_THREAD_STACK_DEFINE(encoderUpdateTaskStack, ENCODER_UPDATE_TASK_STACK_SIZE);
struct k_thread encoderUpdateTaskData;
k_tid_t encoderUpdateThreadId;

int main() {
    #if USE_LS7366R_ENCODER
        myEncoder = new LS7366R();
        myEncoder->Init();
    #endif

    encoderUpdateThreadId = k_thread_create(
        &encoderUpdateTaskData, 
        encoderUpdateTaskStack,
        K_THREAD_STACK_SIZEOF(encoderUpdateTaskStack),
        UpdateEncoderTask,
        myEncoder, NULL, NULL,
        ENCODER_UPDATE_TASK_PRIORITY,
        0, K_NO_WAIT);
}


void UpdateEncoderTask(void *arg1, void *arg2, void *arg3) {
    Encoder* encoder = static_cast<Encoder*>(arg1);
    for (;;) {
        encoder->Update();
        //vTaskDelay(1*portTICK_PERIOD_MS);
    }
}

#endif // TESTING