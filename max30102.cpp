#include <cstdio>
#include <cstdlib>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdint>
#include <cmath>

// I2C bus and MAX30102 device address
#define I2C_BUS "/dev/i2c-1"
#define MAX30102_ADDR 0x57

// MAX30102 register addresses
#define REG_INTR_STATUS_1 0x00
#define REG_INTR_STATUS_2 0x01
#define REG_INTR_ENABLE_1 0x02
#define REG_INTR_ENABLE_2 0x03
#define REG_FIFO_WR_PTR 0x04
#define REG_OVF_COUNTER 0x05
#define REG_FIFO_RD_PTR 0x06
#define REG_FIFO_DATA 0x07
#define REG_MODE_CONFIG 0x09
#define REG_SPO2_CONFIG 0x0A
#define REG_LED1_PA 0x0C
#define REG_LED2_PA 0x0D
#define REG_PILOT_PA 0x10
#define REG_MULTI_LED_CTRL1 0x11
#define REG_MULTI_LED_CTRL2 0x12
#define REG_TEMP_INTR 0x1F
#define REG_TEMP_FRAC 0x20
#define REG_TEMP_CONFIG 0x21
#define REG_PROX_INT_THRESH 0x30
#define REG_REV_ID 0xFE
#define REG_PART_ID 0xFF

// Function prototypes
int i2c_write(int file, uint8_t reg, uint8_t value);
int i2c_read(int file, uint8_t reg, uint8_t *buf, int length);
void max30102_init(int file);
void max30102_read_fifo(int file, int32_t *red_value, int32_t *ir_value);
float calculate_heart_rate(int32_t *ir_buffer, int buffer_length);
void low_pass_filter(int32_t *buffer, int length);

int main() {
    int file; // File descriptor for I2C device
    int32_t red_value, ir_value; // Values for red and IR LEDs
    int32_t ir_buffer[200]; // Buffer to store IR values
    int buffer_index = 0; // Index for the buffer

    // Open the I2C bus
    if ((file = open(I2C_BUS, O_RDWR)) < 0) {
        perror("Failed to open the i2c bus");
        exit(1);
    }

    // Set the I2C device address
    if (ioctl(file, I2C_SLAVE, MAX30102_ADDR) < 0) {
        perror("Failed to set I2C device address");
        close(file);
        exit(1);
    }

    // Initialize the MAX30102 sensor
    max30102_init(file);

 // Main loop to read data and calculate heart rate
    while (1) {
        max30102_read_fifo(file, &red_value, &ir_value);
        ir_buffer[buffer_index] = ir_value;
        buffer_index++;

        // When the buffer is full, process the data
        if (buffer_index >= 200) {
            low_pass_filter(ir_buffer, 200); // Apply low-pass filter
            float heart_rate = calculate_heart_rate(ir_buffer, 200); // Calculate heart rate
            if (heart_rate != 0) {
                heart_rate =heart_rate*1.8; // Adjust heart rate
            }
            if (heart_rate > 100) {
                printf("lay\t\n"); // Print "lay" if heart rate is high
            } else {
                printf("nomal\t\n"); // Print "nomal" if heart rate is normal
            }
            printf("Heart Rate: %.2f BPM\n", heart_rate); // Print heart rate
            buffer_index = 0; // Reset buffer index
        }

        usleep(5000); // Wait for 5ms
    }

    close(file); // Close the I2C device
    return 0;
}

// Initialize the MAX30102 sensor
void max30102_init(int file) {
    i2c_write(file, REG_MODE_CONFIG, 0x40); // Reset the sensor
    usleep(10000); // Wait for 10ms
    i2c_write(file, REG_MODE_CONFIG, 0x03); // Set mode to SpO2
    i2c_write(file, REG_SPO2_CONFIG, 0x27); // Set SpO2 configuration
    i2c_write(file, REG_LED1_PA, 0x24); // Set LED1 pulse amplitude
    i2c_write(file, REG_LED2_PA, 0x24); // Set LED2 pulse amplitude
    i2c_write(file, REG_FIFO_WR_PTR, 0x00); // Reset FIFO write pointer
    i2c_write(file, REG_OVF_COUNTER, 0x00); // Reset FIFO overflow counter
    i2c_write(file, REG_FIFO_RD_PTR, 0x00); // Reset FIFO read pointer
}

// Read data from the FIFO
void max30102_read_fifo(int file, int32_t *red_value, int32_t *ir_value) {
    uint8_t buffer[6];
    i2c_read(file, REG_FIFO_DATA, buffer, 6); // Read 6 bytes of data

    // Combine bytes to form red and IR values
    *red_value = (buffer[0] << 16) | (buffer[1] << 8) | buffer[2];
    *ir_value = (buffer[3] << 16) | (buffer[4] << 8) | buffer[5];
}

// Apply a low-pass filter to the buffer
void low_pass_filter(int32_t *buffer, int length) {
    float alpha = 0.1; // Filter coefficient
    for (int i = 1; i < length; i++) {
        buffer[i] = alpha * buffer[i] + (1 - alpha) * buffer[i - 1];
    }
}

// Calculate heart rate from the IR buffer
float calculate_heart_rate(int32_t *ir_buffer, int buffer_length) {
    int peaks = 0; // Number of peaks detected
    int threshold = 1000; // Threshold for peak detection
    for (int i = 1; i < buffer_length - 1; i++) {
        if (ir_buffer[i] > ir_buffer[i - 1] && ir_buffer[i] > ir_buffer[i + 1] && ir_buffer[i] > threshold) {
            peaks++; // Count peaks
        }
    }
    float heart_rate = (peaks / (float)buffer_length) * 60.0 * 2.0; // Calculate heart rate
    return heart_rate;
}

// Write to an I2C register
int i2c_write(int file, uint8_t reg, uint8_t value) {
    uint8_t buffer[2];
    buffer[0] = reg;
    buffer[1] = value;
    if (write(file, buffer, 2) != 2) {
        perror("Failed to write to the i2c bus");
        return -1;
    }
    return 0;
}

// Read from an I2C register
int i2c_read(int file, uint8_t reg, uint8_t *buf, int length) {
    if (write(file, &reg, 1) != 1) {
        perror("Failed to write to the i2c bus");
        return -1;
    }
    if (read(file, buf, length) != length) {
        perror("Failed to read from the i2c bus");
        return -1;
    }
    return 0;
}
