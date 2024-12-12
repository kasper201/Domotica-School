#ifndef UART_H_
#define UART_H_

extern struct k_msgq uart_msgq; // Declare uart_msgq here

int uartSetup(void);

#endif //UART_H_