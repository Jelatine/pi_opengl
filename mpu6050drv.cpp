#include "mpu6050drv.h"

MPU6050Drv::MPU6050Drv()
{
    initMPU6050();
}

void MPU6050Drv::initMPU6050()
{
    wiringPiSetup () ;
    fd = wiringPiI2CSetup (SlaveAddress);
    if (fd >= 0) {
        // fd 为负数，说明IIC连接失败
        printf("fd = %d\n",fd);
        wiringPiI2CWriteReg8(fd,PWR_MGMT_1,0x00);
        // 开启温度检测 关闭休眠
        wiringPiI2CWriteReg8(fd,SMPLRT_DIV, 0x07);
        wiringPiI2CWriteReg8(fd,CONFIG, 0x06);
        wiringPiI2CWriteReg8(fd,GYRO_CONFIG, 0x18);
        wiringPiI2CWriteReg8(fd,ACCEL_CONFIG, 0x01);
    }
    else {
        printf("IIC初始化失败");
    }
}

float MPU6050Drv::getSinglData(int reg_address)
{
//    return (wiringPiI2CReadReg8(fd,reg_address) << 8) + wiringPiI2CReadReg8(fd,reg_address + 1);
    int high = wiringPiI2CReadReg8(fd, reg_address);
    int low = wiringPiI2CReadReg8(fd, reg_address+1);
    int val = (high << 8) + low;
    return float((val >= 0x8000) ? -((65535 - val) + 1) : val);
}

std::vector<float> MPU6050Drv::getAllData()
{
    // Read gyroscope values.
    // At default sensitivity of 250deg/s we need to scale by 131.
    float angular_velocity_x = getSinglData(GYRO_XOUT_H) * .060975f;
    float angular_velocity_y = getSinglData(GYRO_YOUT_H) * .060975f;
    float angular_velocity_z = getSinglData(GYRO_ZOUT_H) * .060975f;

    // Read accelerometer values.
    // At default sensitivity of 2g we need to scale by 16384.
    // Note: at "level" x = y = 0 but z = 1 (i.e. gravity)
    // But! Imu msg docs say acceleration should be in m/2 so need to *9.807
    const float la_rescale = 16384.0 / 9.807;
    float linear_acceleration_x = getSinglData(ACCEL_XOUT_H) / la_rescale;
    float linear_acceleration_y = getSinglData(ACCEL_YOUT_H) / la_rescale;
    float linear_acceleration_z = getSinglData(ACCEL_ZOUT_H) / la_rescale;

    std::vector<float> result;
    result.clear();
    result.push_back(linear_acceleration_x);
    result.push_back(linear_acceleration_y);
    result.push_back(linear_acceleration_z);
    result.push_back(angular_velocity_x);
    result.push_back(angular_velocity_y);
    result.push_back(angular_velocity_z);
    return result;
}

std::vector<float> MPU6050Drv::getAccData()
{
    const float la_rescale = 16384.0 / 9.807;
    float linear_acceleration_x = getSinglData(ACCEL_XOUT_H) / la_rescale;
    float linear_acceleration_y = getSinglData(ACCEL_YOUT_H) / la_rescale;
    float linear_acceleration_z = getSinglData(ACCEL_ZOUT_H) / la_rescale;
    std::vector<float> result;
    result.clear();
    result.push_back(linear_acceleration_x);
    result.push_back(linear_acceleration_y);
    result.push_back(linear_acceleration_z);
    return result;

}

