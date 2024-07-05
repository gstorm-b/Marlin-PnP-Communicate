import cv2 as cv2
import numpy as np
from frame_calib import FrameCalibrate

def find_circle(image):
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY) 
    gray_blurred = cv2.blur(gray, (3, 3)) 
    detected_circles = cv2.HoughCircles(gray_blurred,  
                   cv2.HOUGH_GRADIENT, 1, 20, param1 = 50, 
               param2 = 30, minRadius = 1, maxRadius = 100)

    if detected_circles is not None: 
        detected_circles = np.uint16(np.around(detected_circles)) 
        for pt in detected_circles[0, :]: 
            x, y, r = pt[0], pt[1], pt[2] 
            print(x, y, r)
            # Draw the circumference of the circle. 
            cv2.circle(image, (x, y), r, (0, 255, 0), 5) 
            # Draw a small circle (of radius 1) to show the center. 
            cv2.circle(image, (x, y), 1, (0, 0, 255), 5) 
            img = cv2.resize(image, (900, 700), interpolation=cv2.INTER_LINEAR)
            cv2.imshow("Detected Circle", img) 
            cv2.waitKey(0) 
        return x, y, r

calib_info = FrameCalibrate("./calib_camera.json")

image_a = cv2.imread('./workspace_image/image_0.bmp')
image_b = cv2.imread('./workspace_image/image_1.bmp')
image_c = cv2.imread('./workspace_image/image_2.bmp')
image_d = cv2.imread('./workspace_image/image_3.bmp')

image_a_distort = calib_info.undistortImage(image_a)
image_b_distort = calib_info.undistortImage(image_b)
image_c_distort = calib_info.undistortImage(image_c)
image_d_distort = calib_info.undistortImage(image_d)

x1, y1, r1 = find_circle(image_a_distort)
x2, y2, r2 = find_circle(image_b_distort)
x3, y3, r3 = find_circle(image_c_distort)
x4, y4, r4 = find_circle(image_d_distort)

####
#  raw point A: 0 - 50 - 55.5
#  raw point B: 240 - 50 - 55.5
#  raw point C: 0 - 200 - 55.5
#  raw point D: 240 - 200 - 55.5
# ###

calib_info.setWorldPoints((0, 50), (240, 50), (0, 200), (240, 200))
calib_info.setImagePoints((x1, y1), (x2, y2), (x3, y3), (x4, y4))
calib_info.calibrateWorkspace()