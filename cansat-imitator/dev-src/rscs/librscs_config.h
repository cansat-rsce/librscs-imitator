#ifndef CONFIG_H_
#define CONFIG_H_

// ========================================================
// Настройки модуля UART
// ========================================================
// Использовать ли буферизацию
#define RSCS_UART_USEBUFFERS // Добавить код для поддержки циклических буферов в UART
#define RSCS_UART_BUFSIZE_RX 50 // размер буфера на RX
#define RSCS_UART_BUFSIZE_TX 50 // размер буфера на TX

// настройки GPS
// размер буфера для накопления сообщения
#define RSCS_GPS_BUFFER_SIZE (100)
#define RSCS_GPS_UART_BAUD_RATE (9600)
#define RSCS_GPS_UART_STOP_BITS (1)
#define RSCS_GPS_UART_PARITY (RSCS_UART_PARITY_NONE)

#endif /* CONFIG_H_ */
