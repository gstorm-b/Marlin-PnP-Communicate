from marlin_client import MarlinClient
from marlin_client import Position
from frame_calib import FrameCalibrate
import cv2
import numpy as np

host = '127.0.0.1'
port = 27800

client = MarlinClient(host, port)
client.connectToHost()

# client.marlinHoming()
# client.marlinAddPnpDoneMarker()
# client.marlinWaitForPnpDone()
client.marlinSetPump(1)
print('Homing done')

calib_info = FrameCalibrate("./calib_camera.json")

image = cv2.imread('./image/image_0.bmp')
image = calib_info.undistortImage(image)

gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY) 
gray_blurred = cv2.blur(gray, (3, 3)) 
detected_circles = cv2.HoughCircles(gray_blurred,  
                cv2.HOUGH_GRADIENT, 1, 20, param1 = 50, 
            param2 = 30, minRadius = 1, maxRadius = 100)

if detected_circles is not None: 
    detected_circles = np.uint16(np.around(detected_circles)) 
    for pt in detected_circles[0, :]: 
        x, y, r = pt[0], pt[1], pt[2] 
        # print(x, y, r) 
        # Draw the circumference of the circle. 
        cv2.circle(image, (x, y), r, (0, 0, 255), 5) 
        # Draw a small circle (of radius 1) to show the center. 
        cv2.circle(image, (x, y), 1, (0, 255, 0), 5) 
        # img = cv2.resize(image, (900, 700), interpolation=cv2.INTER_LINEAR)
        # cv2.imshow("Detected Circle", img) 
        # cv2.waitKey(0) 


    show_image = cv2.resize(image, (900, 700), interpolation=cv2.INTER_LINEAR)
    cv2.imshow('result', show_image)
    cv2.waitKey(0)

    counter = 0

    for pt in detected_circles[0, :]: 
        x, y, r = pt[0], pt[1], pt[2] 
        real_point = calib_info.getCoordinate((x, y))
        print(real_point)
        pick = Position(real_point[0], real_point[1], 53, 0)
        place = Position((240 - (counter*10)), 195, 51, 180)
        counter += 1
        # print(str(pick.x), str(pick.y), str(pick.z))
        # client.marlinMoveL(pick, 100)
        # client.marlinAddPnpDoneMarker()
        client.marlinPnp(pick, 15, place, 15, 300, 150, 500)
        client.marlinWaitForPnpDone()
        print('Move to {0} done!'.format(pick.toString()))
        # cv2.waitKey(0)

client.marlinSetPump(0)
client.disconnectFromHost()