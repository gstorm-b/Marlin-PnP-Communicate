import cv2
import numpy as np
import codecs, json
# import glob
# import matplotlib.pyplot as plt
# import matplotlib.image as mpimg

class NumpyEncoder(json.JSONEncoder):
    def default(self, obj):
        if isinstance(obj, np.ndarray):
            return obj.tolist()
        return json.JSONEncoder.default(self, obj)

class FrameCalibrate:

    CAMERA_MATRIX = 'Camera matrix'
    OPPT_CAMERA_MATRIX = 'Optimal camera matrix'
    DISTORT_ROI = 'Distortion ROI'
    DISTORTION = 'Distortion'
    HOMO_MATRIX = 'Homography matrix'
    FRAME_SIZE = 'Frame size'
    WORLD_POINTS = 'World coordinates'
    IMAGE_POINTS = 'Image coordinates'
    

    # Camera calibration matrix
    camera_matrix = []
    optimal_camera_matrix = []
    distort_roi = []
    camera_dist = []
    homo_matrix = []
    frame_size = (0, 0)
    # Coordination point
    world_points = []
    image_points = []

    criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001)

    def __init__(self, config_path):
        self.cfg_file_path = str(config_path)
        self.camera_matrix = self.loadMatrix(self.CAMERA_MATRIX)
        self.camera_dist = self.loadMatrix(self.DISTORTION)
        self.homo_matrix = self.loadMatrix(self.HOMO_MATRIX)
        self.world_points = self.loadMatrix(self.WORLD_POINTS)
        self.image_points = self.loadMatrix(self.IMAGE_POINTS)
        self.frame_size = self.loadMatrix(self.FRAME_SIZE)
        self.optimal_camera_matrix = self.loadMatrix(self.OPPT_CAMERA_MATRIX)
        self.distort_roi = self.loadMatrix(self.DISTORT_ROI)

    def loadMatrix(self, key):
        obj_text = codecs.open(self.cfg_file_path, 'r', encoding='utf-8').read()
        json_load  = json.loads(obj_text)
        value = json_load.get(key)
        if value is not None:
            matrix_restored = np.asarray(json_load[key])
        else:
            matrix_restored = []
        return matrix_restored

    def saveMatrix(self, key, value):
        obj_text = codecs.open(self.cfg_file_path, 'r', encoding='utf-8').read()
        json_load  = json.loads(obj_text)
        json_load[key] = np.asarray(value)
        json.dump(json_load, 
                    (codecs.open(self.cfg_file_path, 'w', encoding='utf-8')), 
                    indent=4, 
                    cls=NumpyEncoder)

    def setWorldPoints(self, point_a, point_b, point_c, point_d):
        self.world_points = np.zeros((4,2), np.float32)
        self.world_points[0] = point_a
        self.world_points[1] = point_b
        self.world_points[2] = point_c
        self.world_points[3] = point_d

    def setImagePoints(self, point_a, point_b, point_c, point_d):
        self.image_points = np.zeros((4,2), np.float32)
        self.image_points[0] = point_a
        self.image_points[1] = point_b
        self.image_points[2] = point_c
        self.image_points[3] = point_d

    def getConfigFilePath(self):
        return self.cfg_file_path

    def getCameraMatrix(self):
        return self.camera_matrix
    
    def getCameraDistortion(self):
        return self.camera_dist

    def getHomographyMatrix(self):
        return self.homo_matrix

    def getOptimalCameraMatrix(self):
        return self.optimal_camera_matrix

    def getFramesize(self):
        return self.frame_size

    def calibrateCameraMatrix(self, input_images_path, result_images_path, grid_size, distance, use_chess_board = True):
        grid_points = np.zeros((grid_size[0] * grid_size[1], 3), np.float32)
        grid_points[:,:2] = np.mgrid[0:grid_size[0],0:grid_size[1]].T.reshape(-1,2)
        grid_points = grid_points * distance

        object_points = []
        image_points = []
        input_images = glob.glob(input_images_path)

        for counter in range(len(input_images)):
            image = mpimg.imread(input_images[counter], )
            if (len(image.shape) == 2) :
                gray_image = image
            else :
                gray_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

            if counter == 0:
                self.frame_size = gray_image.shape[::-1]
                print(self.frame_size)
                print(gray_image.shape[::-1])

            if use_chess_board:
                ret, corners = cv2.findChessboardCorners(image, grid_size, None)
            else:
                ret, corners = cv2.findCirclesGrid(image, grid_size, None)

            if ret == True:

                object_points.append(grid_points)
                corners2 = cv2.cornerSubPix(image, corners, (11,11), (-1,-1), self.criteria)
                image_points.append(corners)

                # color_image = cv.cvtColor(img, cv.COLOR_GRAY2BGR)
                cv2.drawChessboardCorners(image, grid_size, corners2, ret)
                save_path = result_images_path.replace('*', str(counter))
                cv2.imwrite(save_path, image)
            counter += 1
            
        ret, self.camera_matrix, self.camera_dist, rvecs, tvecs =  cv2.calibrateCamera(
                                                                    object_points, 
                                                                    image_points, 
                                                                    self.frame_size, 
                                                                    None, None)

        self.optimal_camera_matrix, self.distort_roi = cv2.getOptimalNewCameraMatrix(
                                                self.camera_matrix,
                                                self.camera_dist,
                                                self.frame_size, 1, 
                                                self.frame_size)

        self.saveMatrix(self.CAMERA_MATRIX, self.camera_matrix)
        self.saveMatrix(self.DISTORTION, self.camera_dist)
        self.saveMatrix(self.FRAME_SIZE, self.frame_size)
        self.saveMatrix(self.OPPT_CAMERA_MATRIX, self.optimal_camera_matrix)
        self.saveMatrix(self.DISTORT_ROI, self.distort_roi)

    def calibrateWorkspace(self, image, grid_size):
        self.world_points = np.float32(
            [[0, 0], [240, 0], [0, 180], [240, 180]]
        )

        ret, circle_points = cv2.findCirclesGrid(image, grid_size, None)
        
        output_image = cv2.cvtColor(image, cv2.COLOR_GRAY2BGR)

        if ret == True:
            point_a = circle_points[0]
            point_b = circle_points[grid_size[0] - 1]
            point_c = circle_points[grid_size[0]*(grid_size[1] - 1)]
            point_d = circle_points[(grid_size[0]*grid_size[1]) - 1]
            self.setImagePoints(point_a, point_b, point_c, point_d)

            cv2.circle(output_image, (int(point_a[0][0]), int(point_a[0][1])), 10, (255,0,0), 5)
            cv2.putText(output_image, 'A', (int(point_a[0][0]), int(point_a[0][1])), cv2.FONT_HERSHEY_COMPLEX, 2, (0, 0, 255), 2)
            cv2.circle(output_image, (int(point_b[0][0]), int(point_b[0][1])), 10, (255,0,0), 5)
            cv2.putText(output_image, 'B', (int(point_b[0][0]), int(point_b[0][1])), cv2.FONT_HERSHEY_COMPLEX, 2, (0, 0, 255), 2)
            cv2.circle(output_image, (int(point_c[0][0]), int(point_c[0][1])), 10, (255,0,0), 5)
            cv2.putText(output_image, 'C', (int(point_c[0][0]), int(point_c[0][1])), cv2.FONT_HERSHEY_COMPLEX, 2, (0, 0, 255), 2)
            cv2.circle(output_image, (int(point_d[0][0]), int(point_d[0][1])), 10, (255,0,0), 5)
            cv2.putText(output_image, 'D', (int(point_d[0][0]), int(point_d[0][1])), cv2.FONT_HERSHEY_COMPLEX, 2, (0, 0, 255), 2)

            print(self.image_points)
            print(self.world_points)

            self.homo_matrix, status = cv2.findHomography(self.image_points, self.world_points)
            print(self.homo_matrix)
            self.saveMatrix(self.HOMO_MATRIX, self.homo_matrix)

            return output_image

    def calibrateWorkspace(self):
        self.homo_matrix, status = cv2.findHomography(self.image_points, self.world_points)
        print(self.homo_matrix)
        self.saveMatrix(self.HOMO_MATRIX, self.homo_matrix)

    def getCoordinate(self, image_point):
        homogeneous = np.append(image_point, 1)
        # print(self.homo_matrix)
        homogeneous_dot = np.dot(self.homo_matrix, homogeneous)
        coordinate = homogeneous_dot[:2] / homogeneous_dot[2]
        return coordinate

    def undistortImage(self, image):
        image_undistort = cv2.undistort(image, self.camera_matrix, 
                                        self.camera_dist, None, self.optimal_camera_matrix)

        x, y, w, h = self.distort_roi
        image_undistort = image_undistort[y:y+h, x:x+w]
        return image_undistort
