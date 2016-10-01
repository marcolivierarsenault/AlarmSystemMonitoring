//Home system to connect to magellan MG5000 
//Report on IFTTT for alarms and is queriable by ifttt to get the value (arm/disarm)
//Author:Marc-Olivier Arsenault
//Date:Sept 2016

int stateInPacket = 0; //to move in their bit pattern 
int sysdalarmValue = 2; //to tell IFTTT if system is armed or not
bool DEBUG = FALSE;
int values[500] = {0};
int position = 0;//position in the list
int lastRead = 0;//position to read in the list
int lastData = 0;//EPOCH value
String dataValues = "";

void setup() {
    Serial1.begin(9600);
    Particle.variable("lastRead",lastRead);
    Particle.variable("lastData",lastData);
    Particle.variable("dataValues",dataValues);
    Particle.function("UpdateString", updateString);
    Particle.function("ArmedStatus", getStatus);
    if (DEBUG){
        Serial.begin(9600);
    }
}

void loop() {
    if (Serial1.available() > 0) {
        values[position] = (Serial1.read());
        position ++;
        if(position == 500)
            cleanoutList();
    }
    else{
        if (position > lastRead) {
            handleData(values[lastRead]);
            lastRead ++;
            lastData = Time.now();
        }
        
    }
}

//Handle all the actual data from the system
//This method break down each packet the reassamble the frame from the panel
void handleData(int newValue){
    if (newValue == 224){ //E0 new start for the paquet 
        stateInPacket = 1;
        if (DEBUG)
            Serial.write(newValue);
    }
    else{
        switch(stateInPacket){
            
            case 1: //represent 20 (the first two digit of the year)
                if (DEBUG)
                    Serial.write(newValue);
                if(newValue==20)
                    stateInPacket++;
                else 
                    stateInPacket = 0;
                break;
            
            case 2: //represent 16 (or more) (the last two digit of the year)
                if (DEBUG)
                    Serial.write(newValue);
                if(newValue>15)
                    stateInPacket++;
                else 
                    stateInPacket = 0;
                break;  
            
            case 3: //represent month of the year 1-12
                if (DEBUG)
                    Serial.write(newValue);
                if(newValue==Time.month())
                    stateInPacket++;
                else 
                    stateInPacket = 0;
                break;
            
            case 4: //represent day of the month 1-31
                if (DEBUG)
                    Serial.write(newValue);
                if(newValue==Time.day())
                    stateInPacket++;
                else 
                    stateInPacket = 0;
                break;
                
            case 5: //represent hour 
                if (DEBUG)
                    Serial.write(newValue);
                stateInPacket++;
                break;
                
            case 6: //represent minute 
                if (DEBUG)
                    Serial.write(newValue);
                stateInPacket++;
                break;
            
            case 7: //Groupe of alarm (tells witch component is used)
                if (DEBUG)
                    Serial.write(newValue); 
                if(newValue==2)
                    stateInPacket++;
                else 
                    stateInPacket = 0;
                break;
            
            case 8: //SubGroupe of alarm what is the reason (arm disarm alarm etc.)
                if (DEBUG)
                    Serial.write(newValue);
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
    return sysdalarmValue;
}

//method to call when the 
void sendAlarm() {
    Particle.publish("ALARM");
}

int updateString(String command){
    dataValues = "";
    int i;
    for (i=0;i<position;i++) {
        dataValues += String(values[i], HEX);
        dataValues += String(';');
    }
}

//move the list 400 position ahead (to keep the size of the list small)
void cleanoutList() {
    int i;
    for (i = 0; i < 100; i++) {
        values[i] = values[i+400];
    }
    for (i = 100; i < 500; i++) {
        values[i] = 0;
    }
    lastRead = lastRead - 400;
    position = position - 400;
    
}
