//Home system to connect to magellan MG5000
//Report on IFTTT for alarms and is queriable by ifttt to get the value (arm/disarm)
//Author:Marc-Olivier Arsenault
//Date:Sept 2016

int stateInPacket = 0; //to move in their bit pattern
int sysdalarmValue = 2; //to tell IFTTT if system is armed or not

void setup() {
    Serial1.begin(9600);
    Particle.function("ArmedStatus", getStatus);
}

void loop() {
    if (Serial1.available() > 0) {
        handleData(Serial1.read());
     }
}

//Handle all the actual data from the system
//This method break down each packet the reassamble the frame from the panel
void handleData(int newValue){
    if (newValue == 224) //E0 new start for the paquet
        stateInPacket = 1;
    else{
        switch(stateInPacket){

            case 1: //represent 20 (the first two digit of the year)
                if(newValue==20)
                    stateInPacket++;
                else
                    stateInPacket = 0;
                break;

            case 2: //represent 16 (or more) (the last two digit of the year)
                if(newValue>15)
                    stateInPacket++;
                else
                    stateInPacket = 0;
                break;

            case 3: //represent month of the year 1-12
                if(newValue==Time.month())
                    stateInPacket++;
                else
                    stateInPacket = 0;
                break;

            case 4: //represent day of the month 1-31
                if(newValue==Time.day())
                    stateInPacket++;
                else
                    stateInPacket = 0;
                break;

            case 5: //represent hour
                stateInPacket++;
                break;

            case 6: //represent minute
                stateInPacket++;
                break;

            case 7: //Groupe of alarm (tells witch component is used)
                if(newValue==2)
                    stateInPacket++;
                else
                    stateInPacket = 0;
                break;

            case 8: //SubGroupe of alarm what is the reason (arm disarm alarm etc.)
                if(newValue==12) //ARMÉ 12 ou 0x0C
                {
                    stateInPacket++;
                    sysdalarmValue=1; //Maintenant concidéré comme armé
                }
                else if(newValue==4) //Steady ALARM 04
                {
                    stateInPacket++;
                    sendAlarm(); //lance une alarme
                }
                else if(newValue==11) //Désharmé 11 ou 0x0B
                {
                    stateInPacket++;
                    sysdalarmValue=2; //Maintenant concidéré comme non armé
                }
                else
                    stateInPacket = 0;
                break;

            default:
                stateInPacket = 0;
        }
    }
}

//Get The alarm system value (armed or not) to be read from ifttt
//1 is armed 2 is unarmed
int getStatus(String command) {
    if (sysdalarmValue == 1)
        Particle.publish("status","ARMÉ");
    else
        Particle.publish("status","NON ARMÉ");
    return sysdalarmValue;
}

//method to call when the
void sendAlarm() {
    Particle.publish("ALARM");
}