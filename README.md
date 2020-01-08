# AlarmSystemMonitoring
Monitor my own alarm system
Connect to my Magellan MG5000 and report by sms/email on alarms
Can also monitor current status of the Alarm - Arm/Unarm


## to run:

```
docker build -t alarm_system_monitor . 
docker run -id -e "SYSALARM_DEVICE_ID=XXXXXXXXXXX" -e "SYSALARM_API_TOKEN=YYYYYYYYYYYYYYYYYYYY" -p 5000:5000 alarm_system_monitor
```


## For Zappa

```
zappa deploy dev
```

dont forget to add env variable on AWS side:
- SYSALARM_API_TOKEN
- SYSALARM_API_TOKEN