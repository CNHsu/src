# coding=utf-8
import time
import serial  # 引用pySerial模組

data= 'start'

COM_PORT = 'COM12'    # 指定通訊埠名稱
BAUD_RATES = 9600    # 設定傳輸速率
ser = serial.Serial(COM_PORT, BAUD_RATES)   # 初始化序列通訊埠
print(ser.name)
try:
    while True:
        while ser.in_waiting:# 若收到序列資料…
             data_raw = ser.readline()  # 讀取一行
             data_raw2 = data_raw.decode("utf8","ignore")
             print('1接收到的原始資料：', data_raw2)
             st1=data_raw2
             f=st1.find("test")
             if f !=-1:
                print("start if")
                ser.flushInput()
                for cc in range(80,83):
                    ser.write(str(chr(cc)).encode())
                    ser.write(b' \n')
                    #time.sleep(3)
                    while ser.in_waiting:# 若收到序列資料…
                     data_raw = ser.readline()  # 讀取一行
                     data = data_raw.decode()   # 用預設的UTF-8解碼
                     print('2接收到的原始資料：',cc, data)
                     print('2接收到的資料：', data)
                     ser.flushInput()
             else:
                 print('1>>接收到的資料：', st1)
                 print('2>>接收到的資料：', data)
                 #ser.flushInput()
except KeyboardInterrupt:
    ser.close()    # 清除序列通訊物件
    print('再見！')
