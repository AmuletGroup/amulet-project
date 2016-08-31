/* *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */


#ifndef AMULET_UART_H
#define AMULET_UART_H

// function prototypes
void uart_init(void);
void uart_rx_enable(void);
void uart_tx_enable(void);
void uart_rx_disable(void);
void uart_tx_disable(void);

#endif //AMULET_UART_H
