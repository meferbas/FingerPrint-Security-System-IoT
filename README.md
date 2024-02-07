 # Description Of The Project
- This project creates a secure system against bad people, takes photos of people in case of unauthorized entry and sends notifications to the owner via e-mail.
- This project is only a prototype and is open to development.

The circuit diagram of the project is below 👇

![image](https://github.com/meferbas/FingerPrint-Security-System-IoT/assets/103934031/7c2abc53-6982-4d1d-87bc-ff61e2fec8be)

- In this project, a laptop camera was used as a camera module.
- When the wrong fingerprint is read, the red LED turns on, and when the correct fingerprint is read, the green LED turns on.


![image](https://github.com/meferbas/FingerPrint-Security-System-IoT/assets/103934031/0729a1ed-f11b-4e66-8fd1-78b114461fc4) ![image](https://github.com/meferbas/FingerPrint-Security-System-IoT/assets/103934031/345f6b89-645c-404b-897e-d608d1d3f37d)

- When the wrong fingerprint is read, it takes a photo of the person and sends an e-mail to the owner.



![image](https://github.com/meferbas/FingerPrint-Security-System-IoT/assets/103934031/a8a34056-6e75-4d32-9ec8-f12116cde75e)

![image](https://github.com/meferbas/FingerPrint-Security-System-IoT/assets/103934031/638d92f5-5007-4017-8741-80ae2a566d09)

# Description Of The Files
**Enroll.ino:** This file is used to record fingerprints. <br>
**Fingerprint.ino:** This file is used to perform the necessary operations when the fingerprint is read. <br>
**Main.py:** This python file is used to take a photo of the bad person and send it to the owner by e-mail when the fingerprint is read incorrectly.

# The Technologies Used In This Project
- NodeMCU ESP826
- Finger Print Sensor DY-50
- OpenCV (cv2)
- SMTP Protocol
- Camera Module
- ThingSpeak (for data transfer)
- Python
- Ardunio IDE

# Authors and Thanks
-  [@Muhammet-842](https://www.github.com/Muhammet-842)
-  Thank you so much Yiğit Ceylan who helped us develop this project! ❤️

