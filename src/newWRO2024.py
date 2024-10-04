import cv2
import numpy as np
import RPi.GPIO as pin
pin.setmode(pin.BOARD)
pin.setwarnings(False)



pin.setup(36,pin.OUT)
pin.setup(38,pin.OUT)

area=0
red_area=0
green_area=0
choosen=0
color='r'
turn='n'

redlower1 = np.array([175, 100, 50])
redupper1 = np.array([180, 255, 255])

redlower2 = np.array([0, 100, 150])
redupper2 = np.array([5, 255, 255])

greenlower = np.array([40, 80, 60])
greenupper = np.array([80, 255, 200])


webcam = cv2.VideoCapture(1)

def detect(mask, min_area, G, R):
    _, mask_contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    pos=0
    area=0
    if len(mask_contours) !=0:
        largest_contour = max(mask_contours, key=cv2.contourArea)
        if cv2.contourArea(largest_contour) > min_area:
            x, y, w, h = cv2.boundingRect(largest_contour)
            cv2.rectangle(bgr, (x, y), (x + w, y + h), (0, G, R), 2)
            pos=((x+(x+w))/2)/640*100
            area= cv2.contourArea(largest_contour)
    return pos, area

while True:
    _, bgr = webcam.read()
    bgr = bgr[170:480, 0:640]
    hsv = cv2.cvtColor(bgr, cv2.COLOR_BGR2HSV)
    
    mask1 = cv2.inRange(hsv, redlower1, redupper1)
    mask2 = cv2.inRange(hsv, redlower2, redupper2) 
    mask=mask1+mask2
    red_pos, red_area = detect(mask, 1700, 0, 255)            #1700 is the area of the smallest red detection distance
    mask = cv2.inRange(hsv,greenlower,greenupper)
    green_pos ,green_area= detect(mask, 1000, 255, 0)          #1700 is the area of the smallest green detection distance

#     print("R: ", int(red_pos), "   G:", int(green_pos))
    
    if green_area>red_area :
        choosen=green_pos
        color='g'
    elif red_area>green_area :
        choosen=red_pos
        color='r'
    else :
        choosen=0
        color='n'
        
    if color=='r':
        if choosen > 30 : #<------degree of turn
               turn='r'
        elif choosen < 31 : #<------degree of turn
               turn='n'
    elif color=='g':
        if choosen < 70 : #<------degree of turn
               turn='l'
        elif choosen  >69 : #<------degree of turn
               turn='n'
        
            
    if turn =='n' :
        pin.output(36, pin.LOW)
        pin.output(38, pin.LOW)
    elif turn =='r' :
        pin.output(36,pin.LOW)
        pin.output(38,pin.HIGH)
    elif turn =='l' :
        pin.output(36,pin.HIGH)
        pin.output(38,pin.LOW)
    else :
        pin.output(36,pin.LOW)
        pin.output(38,pin.LOW)      
        
        
        
        
              
    
    print("R: ", red_area, "   G:", green_area, "   turn:", turn)                                                                                                                                
    cv2.imshow("window", bgr)
    key =cv2.waitKey(1)