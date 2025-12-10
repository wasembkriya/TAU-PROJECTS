# Face tracker using OpenCV and Arduino
# by Shubham Santosh
# Modified to enlarge squares and add stability (Dead Zone)

import cv2
import serial, time

# Load the cascade
face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')
cap = cv2.VideoCapture(0)

# Settings for box sizes
# --- שליטה בגודל הריבוע האדום (פנים) ---
face_padding = 0

# --- שליטה בגודל הריבוע הלבן (מרכז) ---
# זה משמש גם כ"אזור המת" - אם הפנים בתוך הריבוע הזה, המצלמה לא תזוז
center_box_size = 80

# הגדרת מרכז המסך מראש
screen_center_x = 640 // 2
screen_center_y = 480 // 2

# Arduino setup
try:
    ArduinoSerial = serial.Serial('com4', 9600, timeout=0.1)
    time.sleep(1)

except:
   print("Error: Arduino not connected or wrong port. Continuing without serial.")
   ArduinoSerial = None

while cap.isOpened():
    ret, frame = cap.read()
    if not ret:
        break

    frame = cv2.flip(frame, 1)  # mirror the image
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    faces = face_cascade.detectMultiScale(gray, 1.1, 6)  # detect the face

    # ברירת מחדל: צבע הריבוע המרכזי הוא לבן
    center_color = (255, 255, 255)

    for x, y, w, h in faces:
        # Calculate center logic
        center_x = x + w // 2
        center_y = y + h // 2

        # plot the center of the face
        cv2.circle(frame, (center_x, center_y), 2, (0, 255, 0), 2)

        # --- SHINUI 1: Enlarge Face Rectangle ---
        start_point = (max(0, x - face_padding), max(0, y - face_padding))
        end_point = (min(640, x + w + face_padding), min(480, y + h + face_padding))
        cv2.rectangle(frame, start_point, end_point, (0, 0, 255), 3)

        # --- SHINUI 3: Stability Logic (Dead Zone) ---
        # בדיקה: האם מרכז הפנים נמצא בתוך הריבוע המרכזי?
        diff_x = abs(center_x - screen_center_x)
        diff_y = abs(center_y - screen_center_y)

        # אם הפנים רחוקות מהמרכז יותר מגודל הריבוע - שלח פקודה לארדואינו
        if diff_x > center_box_size or diff_y > center_box_size:
            if ArduinoSerial:
                string = 'X{0:d}Y{1:d}'.format(center_x, center_y)
                ArduinoSerial.write(string.encode('utf-8'))
                # print("Moving: ", string) # לניפוי באגים
        else:
            # אם הפנים בתוך הריבוע - אל תשלח כלום (מנועים לא זזים)
            # נשנה את צבע הריבוע המרכזי לירוק כדי לסמן שהפנים "נעולות" במרכז
            center_color = (0, 255, 0)

            # --- SHINUI 2: Enlarge Center Region Rectangle ---
    # מצייר את הריבוע המרכזי (בצבע לבן או ירוק בהתאם למצב)
    cv2.rectangle(frame,
                  (screen_center_x - center_box_size, screen_center_y - center_box_size),
                  (screen_center_x + center_box_size, screen_center_y + center_box_size),
                  center_color, 3)

    cv2.imshow('img', frame)

    # press q to Quit
    if cv2.waitKey(10) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()