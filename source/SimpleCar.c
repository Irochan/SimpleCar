#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "lms2012.h"

// チャンネル
#define CH_A	0x01
#define CH_B	0x02
#define CH_C	0x04
#define CH_D	0x08

int dev;	// デバイスドライバ

/** プログラムスタート */
int startPrg(void) {
    unsigned char buf[4];
    int result;
    
    buf[0] = opPROGRAM_START;
    result = write(dev, buf, 1);
    
    return result;
}

/** プログラムストップ */
int stopPrg(void) {
    unsigned char buf[4];
    int result;
    
    buf[0] = opPROGRAM_STOP;
    result = write(dev, buf, 1);
    
    return result;
}

/** モーターストップ */
int stopMotor (unsigned char ch) {
    unsigned char buf[4];
    int result;
    
    buf[0] = opOUTPUT_STOP;
    buf[1] = ch;
    
    result = write(dev, buf, 2);
    
    return result;
}

/** モータースタート */
int startMotor (void) {
    unsigned char buf[4];
    int result;
    
    buf[0] = opOUTPUT_START;
    buf[1] = CH_A | CH_B | CH_C | CH_D;
    
    result = write(dev, buf, 2);
    
    return result;
}

/** モーター速度の設定 */
int setMotorPower (unsigned char ch, unsigned char power) {
    unsigned char buf[4];
    int result;
    
    buf[0] = opOUTPUT_POWER;
    buf[1] = ch;
    buf[2] = power;
    
    result = write(dev, buf, 3);
    
    return result;
}

/** モーターリセット */
int resetMotor (unsigned char ch) {
    unsigned char buf[4];
    int result;
    
    buf[0] = opOUTPUT_RESET;
    buf[1] = ch;
    
    result  = write(dev, buf, 2);
    
    return result;
}


int main(void) {
    dev = open("/dev/lms_pwm", O_RDWR);
    if (dev < 0) {
        printf("Cannot open device\n");
        exit(1);
    }
    
    unsigned char leftmt = CH_B;
    unsigned char rightmt = CH_C;
    
    stopPrg();
    startPrg();
    
    resetMotor(leftmt);
    resetMotor(rightmt);
    startMotor();
    
    setMotorPower(leftmt, 40);
    setMotorPower(rightmt, 40);
    sleep(4);
    setMotorPower(leftmt, 0);
    setMotorPower(rightmt, 0);
    sleep(1);
    setMotorPower(rightmt, 40);
    setMotorPower(leftmt, -40);
    sleep(2);
    setMotorPower(rightmt, 0);
    setMotorPower(leftmt, 0);
    sleep(1);
    setMotorPower(leftmt, 40);
    setMotorPower(rightmt, 40);
    sleep(4);
    setMotorPower(leftmt, 0);
    setMotorPower(rightmt, 0);
    sleep(1);
    setMotorPower(leftmt, -40);
    setMotorPower(rightmt, 40);
    sleep(2);
    setMotorPower(leftmt, 0);
    setMotorPower(rightmt, 0);
    sleep(1);
    
    stopMotor(leftmt);
    stopMotor(rightmt);
    sleep(2);
    
    stopPrg();
    
    close(dev);
    exit(0);

    
    return 0;
}
