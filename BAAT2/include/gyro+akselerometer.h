#include <Arduino.h>
#include <Wire.h>

const int i2c_addr = 0x68;  // MPU-6050 I2C address
int16_t gyro_x, gyro_y, gyro_z;
int32_t gyro_x_cal, gyro_y_cal, gyro_z_cal;
boolean set_gyro_angles = false;

int16_t acc_x, acc_y, acc_z;
float acc_total_vector;
float angle_roll_acc, angle_pitch_acc;

float angle_pitch, angle_roll;
float angle_pitch_output, angle_roll_output;

float acc_x_ms2, acc_y_ms2, acc_z_ms2;
float acc_x_ms2_output, acc_y_ms2_output, acc_z_ms2_output;

long loop_timer;
int temp;
int displaycount = 0;

void setup_mpu_6050_registers();
void read_mpu_6050_data();

void MPUsetup() {
  Serial.begin(115200);
  Wire.begin(21, 22);  // Initialize I2C with SDA on 21 and SCL on 22 for ESP32
  
  setup_mpu_6050_registers(); 

  // Calibration
  for (int cal_int = 0; cal_int < 1000; cal_int++) {
    read_mpu_6050_data(); 
    gyro_x_cal += gyro_x;
    gyro_y_cal += gyro_y;
    gyro_z_cal += gyro_z;
    delay(3);                                                          
  }

  gyro_x_cal /= 1000;                                                 
  gyro_y_cal /= 1000;                                                 
  gyro_z_cal /= 1000;
  
  loop_timer = micros();                                               
}

void calculateAccPitchRoll(){
  read_mpu_6050_data();
  
  // Convert raw accelerometer values to m/s^2
  acc_x_ms2 = (float)acc_x / 4096.0 * 9.81;
  acc_y_ms2 = (float)acc_y / 4096.0 * 9.81;
  acc_z_ms2 = (float)acc_z / 4096.0 * 9.81;

  // Apply complementary filter to smooth accelerometer values
  acc_x_ms2_output = acc_x_ms2_output * 0.9 + acc_x_ms2 * 0.1;
  acc_y_ms2_output = acc_y_ms2_output * 0.9 + acc_y_ms2 * 0.1;
  acc_z_ms2_output = acc_z_ms2_output * 0.9 + acc_z_ms2 * 0.1;

  gyro_x -= gyro_x_cal;                                                
  gyro_y -= gyro_y_cal;                                                
  gyro_z -= gyro_z_cal;                                                
         
  angle_pitch += gyro_x * 0.0000611;                                  
  angle_roll += gyro_y * 0.0000611;                                     
  angle_pitch += angle_roll * sin(gyro_z * 0.000001066);               
  angle_roll -= angle_pitch * sin(gyro_z * 0.000001066);               

  acc_total_vector = sqrt((acc_x * acc_x) + (acc_y * acc_y) + (acc_z * acc_z)); 

  if (acc_total_vector == 0) {
    Serial.println("Warning: acc_total_vector is zero.");
    return;  // Skip the rest of the loop to avoid division by zero
  }

  angle_pitch_acc = asin((float)acc_y / acc_total_vector) * 57.296; 
  angle_roll_acc = asin((float)acc_x / acc_total_vector) * -57.296;       

  if (set_gyro_angles) { 
    angle_pitch = angle_pitch * 0.9996 + angle_pitch_acc * 0.0004; 
    angle_roll = angle_roll * 0.9996 + angle_roll_acc * 0.0004;        
  } else { 
    angle_pitch = angle_pitch_acc;
    angle_roll = angle_roll_acc;
    set_gyro_angles = true;                                            
  }
  
  angle_pitch_output = angle_pitch_output * 0.9 + angle_pitch * 0.1; 
  angle_roll_output =  (angle_roll_output * 0.9 + angle_roll * 0.1);  
  
  displaycount++;
  if (displaycount > 10) {
    // Print the smoothed accelerometer values
    Serial.print("acc_x (m/s^2): "); Serial.print(acc_x_ms2_output);
    Serial.print(" acc_y (m/s^2): "); Serial.print(acc_y_ms2_output);
    Serial.print(" acc_z (m/s^2): "); Serial.println(acc_z_ms2_output);

    // Print the smoothed pitch and roll values
    Serial.print(" Pitch: ");
    Serial.print(angle_pitch_output);
    Serial.print(" Roll: ");
    Serial.println(angle_roll_output);
    
    displaycount = 0;
  }
  
  while (micros() - loop_timer < 4000);
  loop_timer = micros();
}

void setup_mpu_6050_registers() {
  Wire.beginTransmission(i2c_addr); 
  Wire.write(0x6B);  
  Wire.write(0x00);
  Wire.endTransmission(); 

  Wire.beginTransmission(i2c_addr); 
  Wire.write(0x1C);   
  Wire.write(0x10); 
  Wire.endTransmission(); 

  Wire.beginTransmission(i2c_addr);
  Wire.write(0x1B);
  Wire.write(0x08); 
  Wire.endTransmission(); 
}

void read_mpu_6050_data(){ 
  Wire.beginTransmission(i2c_addr);  
  Wire.write(0x3B);
  Wire.endTransmission(); 
  Wire.requestFrom(i2c_addr, 14);    
  while(Wire.available() < 14);
  
  acc_x = (Wire.read() << 8) | Wire.read();                                  
  acc_y = (Wire.read() << 8) | Wire.read();                                  
  acc_z = (Wire.read() << 8) | Wire.read();                                  
  temp = (Wire.read() << 8) | Wire.read();                                   
  gyro_x = (Wire.read() << 8) | Wire.read();                                 
  gyro_y = (Wire.read() << 8) | Wire.read();                                 
  gyro_z = (Wire.read() << 8) | Wire.read();                                 
}