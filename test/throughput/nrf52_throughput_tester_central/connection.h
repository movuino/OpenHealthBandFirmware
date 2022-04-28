#ifndef CONNECTION_H
# define CONNECTION_H

# define ASSERT_CHARACTERISTIC(chr) if (!(chr).discover()) {Serial.printf("Characteristic "#chr" not found"); return;} else {Serial.println("Characteristic "#chr" found");}
# define ASSERT_SERVICE(svc, conn) if (!(svc).discover(conn)) {Serial.printf("Characteristic "#svc" not found"); return;}  else {Serial.println("Service "#svc" found");}

# define ENABLE_NOTIFICATIONS(chr) if ((chr).enableNotify()) {Serial.println("Ready to receive "#chr" characteristic data");} else { Serial.println("Couldn't enable notify "#chr". Increase DEBUG LEVEL for troubleshooting");}

#endif