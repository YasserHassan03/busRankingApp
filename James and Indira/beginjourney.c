#include <stdio.h>
#include <string.h>
#include "system.h"
#include "altera_up_avalon_accelerometer_spi.h"
#include "altera_avalon_timer_regs.h"
#include "altera_avalon_timer.h"
#include "altera_avalon_pio_regs.h"
#include "sys/alt_irq.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "alt_types.h"
#include "sys/times.h"
#include "sys/alt_timestamp.h"

#define OFFSET -32
#define PWM_PERIOD 16
#define F_ORDER 20
#define FRACTION_BITS 23
#define CHARLIM 7		// Maximum character length of what the user places in memory.  Increase to allow longer sequences
#define QUITLETTER '~' 		// Letter to kill all processing

alt_8 pwm = 0;
alt_u8 led;
int level;


typedef int32_t fixed_32;

int start_stop(int button){
	if (button==1){
		//printf('start journey\r');
		IOWR_ALTERA_AVALON_PIO_DATA(HEX_5_BASE,0b0010010);
		IOWR_ALTERA_AVALON_PIO_DATA(HEX_4_BASE,0b0000111);
		IOWR_ALTERA_AVALON_PIO_DATA(HEX_3_BASE,0b0001000);
		IOWR_ALTERA_AVALON_PIO_DATA(HEX_2_BASE,0b1001110);
		IOWR_ALTERA_AVALON_PIO_DATA(HEX_1_BASE,0b0000111);
		IOWR_ALTERA_AVALON_PIO_DATA(HEX_0_BASE,0b1111111);
		return 1;

	}
	else if (button==2){
		//printf('stop journey\r');
		IOWR_ALTERA_AVALON_PIO_DATA(HEX_5_BASE,0b0001110);
		IOWR_ALTERA_AVALON_PIO_DATA(HEX_4_BASE,0b1111001);
		IOWR_ALTERA_AVALON_PIO_DATA(HEX_3_BASE,0b1001000);
		IOWR_ALTERA_AVALON_PIO_DATA(HEX_2_BASE,0b1111001);
		IOWR_ALTERA_AVALON_PIO_DATA(HEX_1_BASE,0b0010010);
		IOWR_ALTERA_AVALON_PIO_DATA(HEX_0_BASE,0b0001001);
		return 0;
	}
}
int start(int button){
	if (button==1){
		//printf('start journey\r');
		IOWR_ALTERA_AVALON_PIO_DATA(HEX_5_BASE,0b0010010);
		IOWR_ALTERA_AVALON_PIO_DATA(HEX_4_BASE,0b0000111);
		IOWR_ALTERA_AVALON_PIO_DATA(HEX_3_BASE,0b0001000);
		IOWR_ALTERA_AVALON_PIO_DATA(HEX_2_BASE,0b1001110);
		IOWR_ALTERA_AVALON_PIO_DATA(HEX_1_BASE,0b0000111);
		IOWR_ALTERA_AVALON_PIO_DATA(HEX_0_BASE,0b1111111);
		return 1;

	}
	return 0;
}
int stop(int button){
	if (button==2){
		//printf('stop journey\r');
		IOWR_ALTERA_AVALON_PIO_DATA(HEX_5_BASE,0b0001110);
		IOWR_ALTERA_AVALON_PIO_DATA(HEX_4_BASE,0b1111001);
		IOWR_ALTERA_AVALON_PIO_DATA(HEX_3_BASE,0b1001000);
		IOWR_ALTERA_AVALON_PIO_DATA(HEX_2_BASE,0b1111001);
		IOWR_ALTERA_AVALON_PIO_DATA(HEX_1_BASE,0b0010010);
		IOWR_ALTERA_AVALON_PIO_DATA(HEX_0_BASE,0b0001001);
		return 1;

	}
	return 0;
}
void Display_hex(char* text){
    //enumerate each hex

	for (int i =0; i<6;i++){
		if (text[i]=='1'){
			if (i==0){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_5_BASE,0b1111001);
			}
			else if (i==1){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_4_BASE,0b1111001);
			}
			else if (i==2){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_3_BASE,0b1111001);
			}
			else if (i==3){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_2_BASE,0b1111001);
			}
			else if (i==4){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_1_BASE,0b1111001);
			}
			else if (i==5){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_0_BASE,0b1111001);
			}
		}
		else if(text[i]=='2'){
			if (i==0){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_5_BASE,0b0100100);
			}
			else if (i==1){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_4_BASE,0b0100100);
			}
			else if (i==2){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_3_BASE,0b0100100);
			}
			else if (i==3){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_2_BASE,0b0100100);
			}
			else if (i==4){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_1_BASE,0b0100100);
			}
			else if (i==5){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_0_BASE,0b0100100);
			}
		}
		else if(text[i]=='3'){
			if (i==0){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_5_BASE,0b0110000);
			}
			else if (i==1){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_4_BASE,0b0110000);
			}
			else if (i==2){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_3_BASE,0b0110000);
			}
			else if (i==3){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_2_BASE,0b0110000);
			}
			else if (i==4){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_1_BASE,0b0110000);
			}
			else if (i==5){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_0_BASE,0b0110000);
			}
		}
		else if(text[i]=='4'){
			if (i==0){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_5_BASE,0b0011001);
			}
			else if (i==1){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_4_BASE,0b0011001);
			}
			else if (i==2){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_3_BASE,0b0011001);
			}
			else if (i==3){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_2_BASE,0b0011001);
			}
			else if (i==4){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_1_BASE,0b0011001);
			}
			else if (i==5){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_0_BASE,0b0011001);
			}
		}
		else if(text[i]=='5'){
			if (i==0){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_5_BASE,0b0010010);
			}
			else if (i==1){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_4_BASE,0b0010010);
			}
			else if (i==2){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_3_BASE,0b0010010);
			}
			else if (i==3){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_2_BASE,0b0010010);
			}
			else if (i==4){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_1_BASE,0b0010010);
			}
			else if (i==5){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_0_BASE,0b0010010);
			}
		}
		else if(text[i]=='6'){
			if (i==0){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_5_BASE,0b0000010);
			}
			else if (i==1){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_4_BASE,0b0000010);
			}
			else if (i==2){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_3_BASE,0b0000010);
			}
			else if (i==3){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_2_BASE,0b0000010);
			}
			else if (i==4){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_1_BASE,0b0000010);
			}
			else if (i==5){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_0_BASE,0b0000010);
			}
		}
		else if(text[i]=='7'){
			if (i==0){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_5_BASE,0b1111000);
			}
			else if (i==1){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_4_BASE,0b1111000);
			}
			else if (i==2){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_3_BASE,0b1111000);
			}
			else if (i==3){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_2_BASE,0b1111000);
			}
			else if (i==4){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_1_BASE,0b1111000);
			}
			else if (i==5){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_0_BASE,0b1111000);
			}
		}
		else if(text[i]=='8'){
			if (i==0){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_5_BASE,0b0000000);
			}
			else if (i==1){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_4_BASE,0b0000000);
			}
			else if (i==2){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_3_BASE,0b0000000);
			}
			else if (i==3){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_2_BASE,0b0000000);
			}
			else if (i==4){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_1_BASE,0b0000000);
			}
			else if (i==5){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_0_BASE,0b0000000);
			}
		}
		else if(text[i]=='9'){
			if (i==0){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_5_BASE,0b0011000);
			}
			else if (i==1){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_4_BASE,0b0011000);
			}
			else if (i==2){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_3_BASE,0b0011000);
			}
			else if (i==3){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_2_BASE,0b0011000);
			}
			else if (i==4){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_1_BASE,0b0011000);
			}
			else if (i==5){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_0_BASE,0b0011000);
			}
		}
		else{//0 case
			if (i==0){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_5_BASE,0b1000000);
			}
			else if (i==1){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_4_BASE,0b1000000);
			}
			else if (i==2){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_3_BASE,0b1000000);
			}
			else if (i==3){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_2_BASE,0b1000000);
			}
			else if (i==4){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_1_BASE,0b1000000);
			}
			else if (i==5){
				IOWR_ALTERA_AVALON_PIO_DATA(HEX_0_BASE,0b1000000);
			}
		}

	}

}

fixed_32 float_to_fixed(float x){

return (fixed_32)(round(x*(1<<FRACTION_BITS)));

}


float fixed_to_float(fixed_32 fixed){

return ((float)fixed / (float)(1 << FRACTION_BITS));

}

void led_write(alt_u8 led_pattern) {

    IOWR(LED_BASE, 0, led_pattern);
}



void convert_read(alt_32 acc_read, int * level, alt_u8 * led) {

    acc_read += OFFSET;

    alt_u8 val = (acc_read >> 6) & 0x07;

    * led = (8 >> val) | (8 << (8 - val));

    * level = (acc_read >> 1) & 0x1f;

}



void sys_timer_isr() {

    IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_BASE, 0);
    if (pwm < abs(level)) {
        if (level < 0) {
            led_write(led << 1);
        }else{
            led_write(led >> 1);
        	}

    } else {

        led_write(led);

    }
    if (pwm > PWM_PERIOD) {
        pwm = 0;
    } else {
        pwm++;
    }
}



void timer_init(void * isr) {

    IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_BASE, 0x0003);

    IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_BASE, 0);

    IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_BASE, 0x0900);

    IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_BASE, 0x0000);

    alt_irq_register(TIMER_IRQ, 0, isr);

    IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_BASE, 0x0007);

}



alt_32 FIR(fixed_32 coeffs[], alt_32 *x){

	fixed_32 tmp=0;

	for(int i = 0; i < F_ORDER;i++){

		tmp+=coeffs[i]*x[i];

// printf("%d: %ld\n", i, (alt_32)tmp);

}
// printf("%ld\n", (alt_32)tmp);
	tmp=fixed_to_float(tmp);
	return (alt_32)tmp;
}



void print_text(char *text, const int length) {
	Display_hex(text);
	//asprintf(&printMsg, "<--> Detected %d characters: %s <--> \n%c", length, text, 0x4); 	// Print out the strings
	memset(text, 0, 2*CHARLIM);								// Empty the text buffer for next input
}

char generate_text(char curr, int *length, char *text, int *running) {
	if(curr == '\n') return curr;								// If the line is empty, return nothing.
	int idx = 0;										// Keep track of how many characters have been sent down for later printing
	char newCurr = curr;

	while (newCurr != EOF && newCurr != '\n'){						// Keep reading characters until we get to the end of the line
		if (newCurr == QUITLETTER) { *running = 0; }					// If quitting letter is encountered, setting running to 0
		text[idx] = newCurr;								// Add the next letter to the text buffer
		idx++;										// Keep track of the number of characters read
		newCurr = alt_getchar();							// Get the next character
	}
	*length = idx;
	if (idx>6){
		*running=0;
	}
	return newCurr;
}

int read_chars() {
	char text[2*CHARLIM];									// The buffer for the printing text
	char prevLetter = '!';
	int length = 0;
	int running = 1;

	while (running) {									// Keep running until QUITLETTER is encountered
		prevLetter = alt_getchar();							// Extract the first character (and create a hold until one arrives)
		prevLetter = generate_text(prevLetter, &length, text, &running);		// Process input text
		print_text(text, length);
		// Print input text
		break;
	}

	return 0;
}

int main(){
	alt_u32 exec_t2;
	fixed_32 coeffs[]={0,-17803,-53061	,-97407	,-103638	,0	,266544	,683134	,1153381	,1527779,	1670749,	1527779,	1153381,	683134	,266544,	0,	-103638,	-97407,	-53061,	-17803,	0};
	alt_32 *x = malloc(F_ORDER*sizeof(alt_32));
	alt_32 *y = malloc(F_ORDER*sizeof(alt_32));
	alt_32 *z = malloc(F_ORDER*sizeof(alt_32));
	alt_32 x_read;
    alt_32 y_read;
    alt_32 z_read;
    int zero = 0b1000000;
    int button;
    //initialise each number
    IOWR_ALTERA_AVALON_PIO_DATA(HEX_0_BASE,zero);
    IOWR_ALTERA_AVALON_PIO_DATA(HEX_1_BASE,zero);
    IOWR_ALTERA_AVALON_PIO_DATA(HEX_2_BASE,zero);
    IOWR_ALTERA_AVALON_PIO_DATA(HEX_3_BASE,zero);
    IOWR_ALTERA_AVALON_PIO_DATA(HEX_4_BASE,zero);
    IOWR_ALTERA_AVALON_PIO_DATA(HEX_5_BASE,zero);
    //FILE* fp;
    //fp = fopen ("/dev/jtag_uart", "w");
    int n = 0;
    alt_up_accelerometer_spi_dev * acc_dev;
    acc_dev = alt_up_accelerometer_spi_open_dev("/dev/accelerometer_spi");

    if (acc_dev == NULL) { // if return 1, check if the spi ip name is "accelerometer_spi"
        return 1;
    }

    timer_init(sys_timer_isr);
    begin:
    int l=read_chars();
    //printf("filt\n");
    printf("x_axis,y_axis,z_axis\n");
    while (1) {
    	button= IORD_ALTERA_AVALON_PIO_DATA(BUTTON_BASE);
    	//printf("%d",start_stop(button));
    	start_stop(button);
    	if (button==2){
    		printf("END\n");
    	}
    	if (button==1){
    		printf("START\n");
    	}
        alt_up_accelerometer_spi_read_x_axis(acc_dev, & x_read);
        alt_up_accelerometer_spi_read_y_axis(acc_dev, & y_read);
        alt_up_accelerometer_spi_read_z_axis(acc_dev, & z_read);
        for(int i = 0; i < F_ORDER - 1; i ++){
        	x[i + 1] = x[i];//fill/shift elements of array
        	y[i+1]=y[i];
        	z[i+1]=z[i];
        }
        x[0] = x_read;
        alt_32 x_filt = FIR(coeffs, x);
        y[0] = y_read;
        alt_32 y_filt = FIR(coeffs, y);
        z[0] = z_read;
        alt_32 z_filt = FIR(coeffs ,z);
        //printf("%ld\n",y);
        //alt_printf("filtered: %x\n", x_filt);
        printf("%ld,%ld,%ld\n",x_filt,y_filt,z_filt);
        convert_read((x_read+y_read+z_read)/3, & level, & led);
        n++;//count
        if (n>1000){
        	printf("%c",0x4);
        	n=0;//index to reset
        	goto begin;
        }
    }
    free(x);
    return 0;

}

