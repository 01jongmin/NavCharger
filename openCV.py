import cv2
import serial
# import numpy as np


# def rescale_frame(frame, percent=75):
#     width = int(frame.shape[1] * percent / 100)
#     height = int(frame.shape[0] * percent / 100)
#     dim = (width, height)
#     return cv2.resize(frame, dim, interpolation=cv2.INTER_AREA)

def map(x, in_min, in_max, out_min, out_max):
    return (x - in_min) * (out_max - out_min) // (in_max - in_min) + out_min


firstLocationFound = False
locationFound = False
firstLocationX = None
firstLocationY = None


cap = cv2.VideoCapture(0)


while (cap.isOpened() and not locationFound):

    _, frame = cap.read()

    imgGray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    edges = cv2.Canny(imgGray, 10, 250)
    
    cv2.imshow("edges", edges)

    kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (7, 7))

    closed = cv2.morphologyEx(edges, cv2.MORPH_CLOSE, kernel)
    
    cv2.imshow("closed", closed)
    
    contours, _ = cv2.findContours(closed, cv2.RETR_TREE,
                                   cv2.CHAIN_APPROX_SIMPLE)

    # CONTOURS
    for contour in contours:

        if cv2.contourArea(contour) > 10000:
            (x, y, w, h) = cv2.boundingRect(contour)

            approx = cv2.approxPolyDP(contour,
                                      0.1*cv2.arcLength(contour, True), True)

            if len(approx) == 4:
                if firstLocationFound:
                    secondLocationX = int(x+w/2)
                    secondLocationY = int(y+h/2)

                    if (abs(firstLocationX - secondLocationX) + abs(firstLocationY - secondLocationY) < 10):
                        ser = serial.Serial('/dev/ttyACM0', 9600)

                        mappedSecondLocationX = map(secondLocationX, 568, 65, 0, 100)
                        mappedSecondLocationY = map(secondLocationY, 55, 333, 0, 60)

                        target = str(mappedSecondLocationX) + "," + str(mappedSecondLocationY)
                        ser.write(target.encode())
                        locationFound = True
                        #firstLocationFound = False
                        # cv2.circle(frame, (secondLocationX, secondLocationY),
                        #            10, (0, 255, 0), thickness=-1)
                        # print("x: " + str(secondLocat`ionX) + " y: " + str(secondLocationY))
                    else:
                        firstLocationX = secondLocationX
                        firstLocationY = secondLocationY
                else:
                    firstLocationFound = True
                    firstLocationX = int(x+w/2)
                    firstLocationY = int(y+h/2)

                cv2.drawContours(frame, [approx], 0, (0, 255, 0), 5)

                cv2.putText(frame, "PHONE!!!", (int(x+w/2), int(y+h/2)),
                            cv2.FONT_HERSHEY_COMPLEX, 0.5,
                            (0, 255, 0))

    cv2.imshow("Frame",frame)
    
    if cv2.waitKey(1) & 0xFF == ord('q'):
        cv2.destroyAllWindows()



