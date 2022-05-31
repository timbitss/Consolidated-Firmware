#include <stm32f3xx.h>
#include "main.h"
#include "Io_AcceleratorPedals.h"

bool Io_AcceleratorPedals_IsPappsEncoderAlarmActive(void)
{
    return HAL_GPIO_ReadPin(PRIMARY_APPS_ALARM_GPIO_Port, PRIMARY_APPS_ALARM_Pin) == GPIO_PIN_SET;
}

bool Io_AcceleratorPedals_IsSappsEncoderAlarmActive(void)
{
    return HAL_GPIO_ReadPin(SECONDARY_APPS_ALARM_GPIO_Port, SECONDARY_APPS_ALARM_Pin) == GPIO_PIN_SET;
}
