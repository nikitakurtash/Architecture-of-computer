import cv2
import numpy as np

def add_image_overlay(frame, overlay, alpha):
    overlay_resized = cv2.resize(overlay, (frame.shape[1], frame.shape[0]))
    return cv2.addWeighted(overlay_resized, alpha, frame, 1 - alpha, 0)

def template_matching(frame, template):
    template_gray = cv2.cvtColor(template, cv2.COLOR_BGR2GRAY)
    frame_gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    w, h = template_gray.shape[::-1]

    res = cv2.matchTemplate(frame_gray, template_gray, cv2.TM_CCOEFF_NORMED)
    threshold = 0.8
    loc = np.where(res >= threshold)

    for pt in zip(*loc[::-1]):
        cv2.rectangle(frame, pt, (pt[0] + w, pt[1] + h), (0,255,255), 2)

    return frame

cap = cv2.VideoCapture(0)

fourcc = cv2.VideoWriter_fourcc(*'XVID')
out = cv2.VideoWriter('output.avi', fourcc, 20.0, (640, 480))

cv2.namedWindow('frame')
cv2.createTrackbar('Alpha', 'frame', 0, 10, lambda x: None)

overlay_image = cv2.imread('poly.png')
template_image = cv2.imread('finger.jpg')

while cap.isOpened():
    ret, frame = cap.read()
    if ret:
        alpha = cv2.getTrackbarPos('Alpha', 'frame') / 10
        frame = add_image_overlay(frame, overlay_image, alpha)
        frame = template_matching(frame, template_image)

        out.write(frame)
        cv2.imshow('frame', frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    else:
        break

cap.release()
out.release()
cv2.destroyAllWindows()
