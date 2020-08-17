#include "hcsr.h"
#include "cmsis_os.h"
#include "main.h"
#include "tim.h"

osThreadId handle_hc_tarefa;

osSemaphoreId consumidor;   // Semaphore ID
osSemaphoreDef(consumidor); // Semaphore definition

void inicializar_tarefas()
{
    osThreadDef(hc_tarefa_nome, hc_sr04_task_tarefa, osPriorityNormal, 0, AQUISICAO_STACK_SIZE / 4);
    handle_hc_tarefa = osThreadCreate(osThread(hc_tarefa_nome), NULL);
}

static void hc_sr04_task_tarefa()
{
    consumidor = osSemaphoreCreate(osSemaphore(consumidor), 1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    for (;;)
    {
    }
}