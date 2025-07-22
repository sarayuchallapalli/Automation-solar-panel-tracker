#include <regx51.h>

// Motor control pins
sbit motor_left  = P2^0;
sbit motor_right = P2^1;

// ADC control pins
sbit ADC_WR = P3^6;
sbit ADC_RD = P3^7;
sbit INTR = P3^4;

// MUX control
sbit MUX_A = P3^2;  // 74HC4051 A line (Y0/Y1 switch)

// I2C pins for RTC
sbit SCL = P3^0;
sbit SDA = P3^1;

// Global variables
unsigned char hours = 0, minutes = 0;

// Delay functions
void delay(unsigned int t) {
    unsigned int i, j;
    for (i = 0; i < t; i++)
        for (j = 0; j < 1275; j++);
}

void delay_us(unsigned int us) {
    while (us--);
}

// I2C Functions (bit-banged)
void I2C_Start() {
    SDA = 1; SCL = 1; delay_us(5);
    SDA = 0; delay_us(5);
    SCL = 0;
}

void I2C_Stop() {
    SDA = 0; SCL = 1; delay_us(5);
    SDA = 1; delay_us(5);
}

void I2C_Write(unsigned char dat) {
    unsigned char i;
    for (i = 0; i < 8; i++) {
        SDA = (dat & 0x80);  // MSB first
        SCL = 1; delay_us(5);
        SCL = 0; delay_us(5);
        dat <<= 1;
    }
    SDA = 1; // Release SDA
    SCL = 1; delay_us(5);
    SCL = 0;
}

unsigned char I2C_Read() {
    unsigned char i, dat = 0;
    SDA = 1; // Release SDA for input
    for (i = 0; i < 8; i++) {
        dat <<= 1;
        SCL = 1;
        if (SDA) dat |= 1;
        SCL = 0;
    }
    return dat;
}

unsigned char bcd2dec(unsigned char bcd) {
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

void read_time() {
    I2C_Start();
    I2C_Write(0xD0);     // RTC write address
    I2C_Write(0x01);     // Address pointer to minutes
    I2C_Start();
    I2C_Write(0xD1);     // RTC read address
    minutes = bcd2dec(I2C_Read());
    hours = bcd2dec(I2C_Read());
    I2C_Stop();
}

// ADC Read Function
unsigned char read_adc() {
    unsigned char val;
    ADC_WR = 0; delay(1);
    ADC_WR = 1;
    while (INTR);  // Wait until conversion complete
    ADC_RD = 0;
    val = P1;      // Read digital data
    ADC_RD = 1;
    return val;
}

unsigned char read_LDR(unsigned char mux_channel) {
    MUX_A = mux_channel;  // 0 = Y0 (LDR1), 1 = Y1 (LDR2)
    delay(2);
    return read_adc();
}

// Time-based movement (fallback)
void move_based_on_time(unsigned char h) {
    if (h >= 6 && h < 10) {
        motor_left = 1;
        motor_right = 0;
    } else if (h >= 14 && h < 18) {
        motor_left = 0;
        motor_right = 1;
    } else {
        motor_left = 0;
        motor_right = 0;
    }
}

// Main function
void main() {
    unsigned char ldr1_val, ldr2_val;

    // Initial motor state
    motor_left = 0;
    motor_right = 0;
    WR = 1; ADC_RD = 1;

    while (1) {
        // Read time from RTC
        read_time();

        // Read LDRs
        ldr1_val = read_LDR(0);  // LDR1 via Y0
        delay(2);
        ldr2_val = read_LDR(1);  // LDR2 via Y1

        // Light-based control
        if (ldr1_val > ldr2_val + 15) {
            motor_left = 1;
            motor_right = 0;
        }
        else if (ldr2_val > ldr1_val + 15) {
            motor_left = 0;
            motor_right = 1;
        }
        else {
            move_based_on_time(hours);  // Use RTC fallback
        }

        delay(100);  // Wait before next read
    }
}