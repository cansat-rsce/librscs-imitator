#ifndef CONFIG_H_
#define CONFIG_H_

// ========================================================
// Настройки модуля UART
// ========================================================
// Использовать ли буферизацию
#define RSCS_UART_USEBUFFERS // Добавить код для поддержки циклических буферов в UART
#define RSCS_UART_BUFSIZE_RX 50 // размер буфера на RX
#define RSCS_UART_BUFSIZE_TX 50 // размер буфера на TX

#endif /* CONFIG_H_ */
