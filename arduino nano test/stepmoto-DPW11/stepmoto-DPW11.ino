//*********************************************
//https://www.twblogs.net/a/5c0d2345bd9eee5e40ba9e10
//
// 2019/9/23 for DPW11 step

#include <Stepper.h>

#define STEPS 100
// int1-4: 8,9,10,11
Stepper stepper(STEPS, 8, 11, 9, 10);
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
    stepper.step(1); //4步模式下旋轉一週用2048 步。
    delay(500);

    // 逆時針旋轉半周
    Serial.println("ni");
    stepper.step(-1); //4步模式下旋轉一週用2048 步。
    delay(500);
}
