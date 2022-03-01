//*********************************************
//https://www.twblogs.net/a/5c0d2345bd9eee5e40ba9e10
//https://blog.jmaker.com.tw/uln2003-28byj-48/
//IN1  8
//IN2 9
//IN3 10
//IN4 11
//VCC VCC
//IGBD  GND
//

#include <Stepper.h>

#define STEPS 100
// int1-4: 8,9,10,11
Stepper stepper(STEPS, 8, 10, 9, 11);
//or ?
//Stepper stepper(STEPS, 8, 9, 10, 11);

void setup()
{
  // 設置電機的轉速：每分鐘爲90步
  stepper.setSpeed(30);
  // 初始化串口，用於調試輸出信息
  Serial.begin(38400);
}

void loop()
{
    // 順時針旋轉一週
    Serial.println("shun");
    stepper.step(2048); //4步模式下旋轉一週用2048 步。
    delay(500);

    // 逆時針旋轉半周
    Serial.println("ni");
    stepper.step(-1024); //4步模式下旋轉一週用2048 步。
    delay(500);
}
