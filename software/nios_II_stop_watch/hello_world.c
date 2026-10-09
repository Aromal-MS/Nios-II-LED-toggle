#include <stdio.h>
#include <unistd.h>
#include "system.h"
#include "io.h"
#include "altera_avalon_pio_regs.h"

/*
 * DE10-Lite 7-segment display
 *
 * Each display group:
 *
 * [14]       = DP
 * [13:7]     = tens digit
 * [6:0]      = units digit
 *
 * Segment pattern:
 *       0
 *      ---
 *   5 |   | 1
 *      ---  <- 6
 *   4 |   | 2
 *      ---
 *       3
 *
 * DE10-Lite 7-segment is active LOW.
 */

#define SEG_0  0x40
#define SEG_1  0x79
#define SEG_2  0x24
#define SEG_3  0x30
#define SEG_4  0x19
#define SEG_5  0x12
#define SEG_6  0x02
#define SEG_7  0x78
#define SEG_8  0x00
#define SEG_9  0x10

unsigned char seg[10] =
{
    SEG_0,
    SEG_1,
    SEG_2,
    SEG_3,
    SEG_4,
    SEG_5,
    SEG_6,
    SEG_7,
    SEG_8,
    SEG_9
};


/* Display two digits on one 14-bit group */
unsigned int make_display(int value)
{
    int tens;
    int units;

    tens  = value / 10;
    units = value % 10;

    return ((unsigned int)seg[tens] << 7) |
           (unsigned int)seg[units];
}


/* Update all six digits */
void update_display(int milliseconds, int seconds, int minutes)
{
    unsigned int msec_value;
    unsigned int sec_value;
    unsigned int min_value;

    /*
     * milliseconds: display 00-99
     * seconds:      display 00-59
     * minutes:      display 00-99
     */

    msec_value = make_display(milliseconds);

    sec_value = make_display(seconds);

    min_value = make_display(minutes);

    /*
     * DP on seconds
     * DP on minutes
     *
     * Bit 14 = DP
     */
    sec_value |= (1 << 14);
    min_value |= (1 << 14);

    /*
     * Write to Platform Designer PIOs
     */
    IOWR_ALTERA_AVALON_PIO_DATA(MSEC_BASE, msec_value);

    IOWR_ALTERA_AVALON_PIO_DATA(SEC_BASE, sec_value);

    IOWR_ALTERA_AVALON_PIO_DATA(MIN_BASE, min_value);
}


int main()
{
    int milliseconds = 0;
    int seconds = 0;
    int minutes = 0;

    int running = 0;

    unsigned int buttons;
    unsigned int old_buttons;

    old_buttons = 0x03;

    printf("Nios II Stopwatch\n");

    /* Initially display 00:00:00 */
    update_display(milliseconds, seconds, minutes);

    while (1)
    {
        /*
         * Read push buttons
         *
         * Assuming buttons are ACTIVE LOW:
         *
         * input_export[0] = Start/Stop
         * input_export[1] = Reset
         */
        buttons = IORD_ALTERA_AVALON_PIO_DATA(INPUT_BASE);

        /*
         * Start/Stop button
         *
         * Detect falling edge
         */
        if ((old_buttons & 0x01) && !(buttons & 0x01))
        {
            running = !running;

            usleep(200000);   // debounce
        }

        /*
         * Reset button
         *
         * Detect falling edge
         */
        if ((old_buttons & 0x02) && !(buttons & 0x02))
        {
            milliseconds = 0;
            seconds = 0;
            minutes = 0;

            update_display(milliseconds, seconds, minutes);

            usleep(200000);   // debounce
        }

        old_buttons = buttons;


        /*
         * Stopwatch running
         *
         * Update every 10 ms.
         */
        if (running)
        {
            usleep(10000);

            milliseconds++;

            if (milliseconds >= 100)
            {
                milliseconds = 0;
                seconds++;

                if (seconds >= 60)
                {
                    seconds = 0;
                    minutes++;

                    if (minutes >= 100)
                    {
                        minutes = 0;
                    }
                }
            }

            update_display(milliseconds, seconds, minutes);
        }
        else
        {
            /*
             * Small delay while stopped
             */
            usleep(10000);
        }
    }

    return 0;
}
