import sys
sys.stdout.reconfigure(encoding='utf-8')

import cv2
import time
import imutils
import datetime
import winsound

print("Đang khởi tạo camera...")
cam = cv2.VideoCapture(0)
if not cam.isOpened():
    print("Lỗi: Không thể mở camera với ID 0. Thử kiểm tra webcam hoặc đổi ID.")
    cam = cv2.VideoCapture(1)
    if not cam.isOpened():
        print("Lỗi: Không thể mở camera với ID 1. Kiểm tra thiết bị hoặc kết nối.")
        exit()

print("Camera đã mở thành công!")
time.sleep(0.5)

firstFrame = None
prevFrame = None
area = 300
motion_detected = False
out = None

fourcc = cv2.VideoWriter_fourcc(*'mp4v')
fps = 20.0
frame_width = 640
frame_height = None

# Khởi tạo biến đo FPS
start_time = time.time()
frame_count = 0

while True:
    ret, frame = cam.read()
    if not ret:
        print("Lỗi: Không thể đọc frame từ camera!")
        break
    
    frame = cv2.flip(frame, 1)
    text = "Normal"
    frame = imutils.resize(frame, width=frame_width)
    
    if frame_height is None:
        frame_height = frame.shape[0]
    
    grayImg = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    grayFrame = cv2.GaussianBlur(grayImg, (11, 11), 0)

    if firstFrame is None:
        firstFrame = grayFrame
        prevFrame = grayFrame
        continue

    frameDiff = cv2.absdiff(prevFrame, grayFrame)
    threshFrame = cv2.threshold(frameDiff, 30, 255, cv2.THRESH_BINARY)[1]
    threshFrame = cv2.dilate(threshFrame, None, iterations=1)

    cnts = cv2.findContours(threshFrame.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    cnts = imutils.grab_contours(cnts)

    for c in cnts:
        if cv2.contourArea(c) < area:
            continue
        (x, y, w, h) = cv2.boundingRect(c)
        cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
        text = "Moving Object detected"
        motion_detected = True

    prevFrame = grayFrame.copy()

    if motion_detected and out is None:
        timestamp = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
        video_filename = f"motion_{timestamp}.mp4"
        out = cv2.VideoWriter(video_filename, fourcc, fps, (frame_width, frame_height))
        if not out.isOpened():
            print("Lỗi: Không thể tạo file video!")
        else:
            print(f"Bắt đầu ghi video: {video_filename}")
            winsound.Beep(1000, 500)
    
    if out is not None:
        out.write(frame)
    
    if text == "Normal" and motion_detected:
        motion_detected = False
        if out is not None:
            out.release()
            out = None
            print("Đã dừng ghi video")

    timestamp = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    cv2.putText(frame, timestamp, (10, frame_height - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 1)
    cv2.putText(frame, text, (10, 20), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)
    
    cv2.imshow("Surveillance Feed", frame)

    # Đo FPS
    frame_count += 1
    if frame_count % 10 == 0:
        elapsed = time.time() - start_time
        fps_actual = frame_count / elapsed
        print(f"FPS: {fps_actual:.2f}")

    key = cv2.waitKey(10) & 0xFF
    if key == 27:
        print("Thoát chương trình bởi người dùng.")
        break
    elif key == ord('r'):
        firstFrame = None
        prevFrame = None

if out is not None:
    out.release()
cam.release()
cv2.destroyAllWindows()
print("Đã giải phóng tài nguyên.")