#include "hcsr.h"
#include "cmsis_os.h"
#include "main.h"
#include "tim.h"
#include "usart.h"

osThreadId handle_hc_tarefa;

osSemaphoreId consumidor;   // Semaphore ID
osSemaphoreDef(consumidor); // Semaphore definition

osMessageQDef(msg_tempo, 5, uint32_t); // Define message queue
osMessageQId msg_tempo;

void inicializar_tarefas()
{
    osThreadDef(hc_tarefa_nome, hc_sr04_task_tarefa, osPriorityNormal, 0, AQUISICAO_STACK_SIZE / 4);
    handle_hc_tarefa = osThreadCreate(osThread(hc_tarefa_nome), NULL);
}

static void hc_sr04_task_tarefa()
{
    consumidor = osSemaphoreCreate(osSemaphore(consumidor), 1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    msg_tempo = osMessageCreate(osMessageQ(msg_tempo), osThreadGetId()); // create msg queue
    osEvent evt_msg;

    char tx[32];
    uint32_t diferenca;
    float distancia;
    size_t len;
    for (;;)
    {
        evt_msg = osMessageGet(msg_tempo, osWaitForever);
        if (evt_msg.status = osEventMessage)
        {
            diferenca = (uint32_t)evt_msg.value.p; //us
            distancia = (float)diferenca / 58.0;
            len = sprintf(tx, "distancia %.0f cm\n", distancia);
            HAL_UART_Transmit(&huart1, tx, len, osWaitForever);
        }
    }
}

// void EXTI3_IRQHandler(void)
// {
//     char tx[32];
//     size_t len = sprintf(tx, "tempo inicial = %d\n", HAL_GetTick());
//     HAL_UART_Transmit(&huart1, tx, len, 1000);
// }

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    static uint32_t diferenca = 0;

    if (GPIO_Pin == ECHO_RISING_Pin)
        HAL_TIM_OC_Start(&htim3, TIM_CHANNEL_1);

    if (GPIO_Pin == ECHO_FALLING_Pin)
    {
        HAL_TIM_OC_Stop(&htim3, TIM_CHANNEL_1);
        diferenca = __HAL_TIM_GET_COUNTER(&htim3);
        __HAL_TIM_SET_COUNTER(&htim3, 0);
        osMessagePut(msg_tempo, diferenca, 0);
    }
}