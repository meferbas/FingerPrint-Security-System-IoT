import cv2
import serial
import smtplib
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from email.mime.image import MIMEImage
import os
import time 


def send_mail_with_image(sender_email, sender_password, recipient_email, subject, body, image_path):
    msg = MIMEMultipart()
    msg['From'] = sender_email
    msg['To'] = recipient_email
    msg['Subject'] = subject

    msg.attach(MIMEText(body, 'plain'))

    with open(image_path, 'rb') as f:
        img_data = f.read()
    image = MIMEImage(img_data, name=os.path.basename(image_path))
    msg.attach(image)

    server = smtplib.SMTP('smtp.gmail.com', 587)
    server.starttls()
    server.login(sender_email, sender_password)
    server.send_message(msg)
    server.quit()
    print("E-posta gönderildi.")

ser = serial.Serial('COM6', 115200, timeout=1)
cap = cv2.VideoCapture(0)

try:
    last_time_printed = time.time()
    while True:
        line = ser.readline().decode('utf-8', errors='ignore').rstrip()
        if line == 'N':
            print("Yanlış parmak izi algılandı, kamera aktif.")
            success, img = cap.read()
            if success:
                image_path = "unauthorized.jpg"
                cv2.imwrite(image_path, img)
                print("Fotoğraf çekildi, e-posta gönderiliyor...")
                cv2.imshow("Unauthorized Access", img)

            
                send_mail_with_image(
                    sender_email='Sender@mail.com',
                    sender_password='senderPassword',
                    recipient_email='Receiver@mail.com',
                    subject='Hatalı Giriş Algılandı!',
                    body='Yanlış parmak izi tespit edildi. Ekteki fotoğrafı kontrol ediniz.',
                    image_path=image_path
                )
                
        elif line == 'Y':
                print("Parmak izi okuma başarılı.")
        else:
            if time.time() - last_time_printed > 8:  
                print("Parmak izi bekleniyor...")
                last_time_printed = time.time()
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
finally:
    cap.release()
    cv2.destroyAllWindows()
    ser.close()
