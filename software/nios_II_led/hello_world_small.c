/* 
 * "Small Hello World" example. 
 * 
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example 
 * designs. It requires a STDOUT  device in your system's hardware. 
 *
 * The purpose of this example is to demonstrate the smallest possible Hello 
 * World application, using the Nios II HAL library.  The memory footprint
 * of this hosted application is ~332 bytes by default using the standard 
 * reference design.  For a more fully featured Hello World application
 * example, see the example titled "Hello World".
 *
 * The memory footprint of this example has been reduced by making the
 * following changes to the normal "Hello World" example.
 * Check in the Nios II Software Developers Manual for a more complete 
 * description.
 * 
 * In the SW Application project (small_hello_world):
 *
 *  - In the C/C++ Build page
 * 
 *    - Set the Optimization Level to -Os
 * 
 * In System Library project (small_hello_world_syslib):
 *  - In the C/C++ Build page
 * 
 *    - Set the Optimization Level to -Os
 * 
 *    - Define the preprocessor option ALT_NO_INSTRUCTION_EMULATION 
 *      This removes software exception handling, which means that you cannot 
 *      run code compiled for Nios II cpu with a hardware multiplier on a core 
 *      without a the multiply unit. Check the Nios II Software Developers 
 *      Manual for more details.
 *
 *  - In the System Library page:
 *    - Set Periodic system timer and Timestamp timer to none
 *      This prevents the automatic inclusion of the timer driver.
 *
 *    - Set Max file descriptors to 4
 *      This reduces the size of the file handle pool.
 *
 *    - Check Main function does not exit
 *    - Uncheck Clean exit (flush buffers)
 *      This removes the unneeded call to exit when main returns, since it
 *      won't.
 *
 *    - Check Don't use C++
 *      This builds without the C++ support code.
 *
 *    - Check Small C library
 *      This uses a reduced functionality C library, which lacks  
 *      support for buffering, file IO, floating point and getch(), etc. 
 *      Check the Nios II Software Developers Manual for a complete list.
 *
 *    - Check Reduced device drivers
 *      This uses reduced functionality drivers if they're available. For the
 *      standard design this means you get polled UART and JTAG UART drivers,
 *      no support for the LCD driver and you lose the ability to program 
 *      CFI compliant flash devices.
 *
 *    - Check Access device drivers directly
 *      This bypasses the device file system to access device drivers directly.
 *      This eliminates the space required for the device file system services.
 *      It also provides a HAL version of libc services that access the drivers
 *      directly, further reducing space. Only a limited number of libc
 *      functions are available in this configuration.
 *
 *    - Use ALT versions of stdio routines:
 *
 *           Function                  Description
 *        ===============  =====================================
 *        alt_printf       Only supports %s, %x, and %c ( < 1 Kbyte)
 *        alt_putstr       Smaller overhead than puts with direct drivers
 *                         Note this function doesn't add a newline.
 *        alt_putchar      Smaller overhead than putchar with direct drivers
 *        alt_getchar      Smaller overhead than getchar with direct drivers
 *
 */

/*
#include <stdio.h>
#include "system.h"
#include "altera_avalon_pio_regs.h"
int main()
{
	printf("Hello from Nios II!\n");
	//set value to pio core // Turn on leds
	// IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, 0xf00);
	//Read and print the values of pio core
	// printf("LEDs value: 0x%x", IORD_ALTERA_AVALON_PIO_DATA(LED_BASE));
	// Event loop never exits.
	while (1)
	{
		IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, 0x3FF);
		printf("LEDs value: 0x%x", IORD_ALTERA_AVALON_PIO_DATA(LED_BASE));
		usleep(500000);
		IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, 0x000);
		printf("LEDs value: 0x%x", IORD_ALTERA_AVALON_PIO_DATA(LED_BASE));
		usleep(500000);
	}
	return 0;
	}

	*/

#include <stdio.h>
#include "system.h"
#include "altera_avalon_pio_regs.h"
#include "unistd.h"

void delay(void)
{
    usleep(100000);   // 100 ms
}

int main()
{
    printf("Nios II 10 LED Pattern Program\n");

    while (1)
    {
        /* =================================================
         * PATTERN 1: Left to Right
         * 0000000001
         * 0000000010
         * ...
         * 1000000000
         * ================================================= */
        int i;

        for (i = 0; i < 10; i++)
        {
            IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, (1 << i));
            delay();
        }


        /* =================================================
         * PATTERN 2: Right to Left
         * ================================================= */
        for (i = 9; i >= 0; i--)
        {
            IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, (1 << i));
            delay();
        }


        /* =================================================
         * PATTERN 3: Fill from Left
         *
         * 0000000001
         * 0000000011
         * 0000000111
         * ...
         * 1111111111
         * ================================================= */
        unsigned int pattern = 0;

        for (i = 0; i < 10; i++)
        {
            pattern |= (1 << i);

            IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, pattern);
            delay();
        }


        /* =================================================
         * PATTERN 4: Fill from Right
         * ================================================= */
        pattern = 0;

        for (i = 9; i >= 0; i--)
        {
            pattern |= (1 << i);

            IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, pattern);
            delay();
        }


        /* =================================================
         * PATTERN 5: Alternating LEDs
         *
         * 0101010101
         * 1010101010
         * ================================================= */
        for (i = 0; i < 5; i++)
        {
            IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, 0x155);
            delay();

            IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, 0x2AA);
            delay();
        }


        /* =================================================
         * PATTERN 6: Center Outward
         *
         * 0000011000
         * 0000111100
         * 0011111110
         * 1111111111
         * ================================================= */

        IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, 0x018);
        delay();

        IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, 0x03C);
        delay();

        IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, 0x0FF);
        delay();

        IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, 0x3FF);
        delay();


        /* =================================================
         * PATTERN 7: Outside Inward
         * ================================================= */

        IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, 0x201);
        delay();

        IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, 0x303);
        delay();

        IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, 0x387);
        delay();

        IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, 0x3CF);
        delay();

        IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, 0x3FF);
        delay();


        /* =================================================
         * PATTERN 8: Knight Rider
         * ================================================= */

        for (i = 0; i < 10; i++)
        {
            IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, (1 << i));
            delay();
        }

        for (i = 8; i > 0; i--)
        {
            IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, (1 << i));
            delay();
        }


        /* =================================================
         * PATTERN 9: Blink All
         * ================================================= */

        for (i = 0; i < 3; i++)
        {
            IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, 0x3FF);
            delay();

            IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, 0x000);
            delay();
        }


        /* =================================================
         * PATTERN 10: Binary Counter
         *
         * 0000000000
         * 0000000001
         * 0000000010
         * ...
         * 1111111111
         * ================================================= */

        for (i = 0; i < 1024; i++)
        {
            IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, i);
            usleep(20000);       // 20 ms
        }


        /* Small pause before repeating everything */
        IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, 0x000);
        usleep(500000);
    }

    return 0;
}
