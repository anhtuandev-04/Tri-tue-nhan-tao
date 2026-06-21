import cv2  # image
import time  # delay
import imutils  # resize

cam = cv2.VideoCapture(0)  # cam id
time.sleep(0.5)  # time delay for camera initialization

firstFrame = None
area = 500  # diện tích tối thiểu để coi là đối tượng
prevFrame = None  # lưu khung trước đó để so sánh chuyển động

while True:
    ret, frame = cam.read()  # read frame from camera
    if not ret:
        print("Không thể đọc từ camera!")
        break
    
    # Lật khung hình theo chiều ngang (phản chiếu)
    frame = cv2.flip(frame, 1)
    
    text = "Normal"  # mặc định là không có chuyển động
    frame = imutils.resize(frame, width=1000)  # resize the resolution

    grayImg = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)  # color to grayscale
    grayFrame = cv2.GaussianBlur(grayImg, (21, 21), 0)  # smoothened

    if firstFrame is None:
        firstFrame = grayFrame  # capturing 1st frame on the first iteration
        prevFrame = grayFrame  # khởi tạo prevFrame
        continue

    # Tính sự khác biệt giữa khung trước và khung hiện tại để phát hiện chuyển động mới
    frameDiff = cv2.absdiff(prevFrame, grayFrame)
    threshFrame = cv2.threshold(frameDiff, 25, 255, cv2.THRESH_BINARY)[1]
    threshFrame = cv2.dilate(threshFrame, None, iterations=2)

    # Tìm contours của các vùng thay đổi
    cnts = cv2.findContours(threshFrame.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    cnts = imutils.grab_contours(cnts)

    # Chỉ vẽ khung khi có đối tượng di chuyển
    for c in cnts:
        if cv2.contourArea(c) < area:  # bỏ qua các vùng nhỏ
            continue
        (x, y, w, h) = cv2.boundingRect(c)
        cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
        text = "Moving Object detected"

    # Cập nhật prevFrame cho lần lặp tiếp theo
    prevFrame = grayFrame.copy()

    # In trạng thái và hiển thị text trên khung hình
    print(text)
    cv2.putText(frame, text, (10, 20), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)
    cv2.imshow("cameraFeed", frame)

    key = cv2.waitKey(1) & 0xFF
    if key == 27:  # thoát khi nhấn ESC
        break
    elif key == ord('r'):  # nhấn 'r' để reset firstFrame và prevFrame
        firstFrame = None
        prevFrame = None

cam.release()
cv2.destroyAllWindows()