#include <Adafruit_LittleFS.h>
#include <InternalFileSystem.h>

using namespace Adafruit_LittleFS_Namespace;

#define FILENAME    "/dataOnFlash.txt"
//#define CONTENTS    "Adafruit Little File System test file contents"

File file(InternalFS);

/*
  // the setup function runs once when you press reset or power the board
  char* read_Fash()
  {
  Serial.print("Open " FILENAME " file to read ... ");
  file.open(FILENAME, FILE_O_READ);

  // file existed
  if ( file )
  {
    Serial.println(FILENAME " file exists");
    uint32_t readlen;
    char buffer[64] = { 0 };
    readlen = file.read(buffer, sizeof(buffer));

    buffer[readlen] = 0;
    Serial.println(buffer);
    file.close();
  } else
  {
    Serial.println("Failed!");
  }

  Serial.println("Done");
  return 0;
  }

  void write_Flash(uint8_t* contents) {

  Serial.print("Open " FILENAME " file to write ... ");

  if ( file.open(FILENAME, FILE_O_WRITE) )
  {
    Serial.println("OK");
    file.write(CONTENTS, strlen(CONTENTS));
    file.close();
  }
  else
  {
    Serial.println("Failed!");
  }

  Serial.println("Done");
  }
*/

void configureFlash() {
  Serial.println("#### INTERNAL FLASH CONFIG #####");

  // Wait for user input to run. Otherwise the code will
  // always run immediately after flash and create the FILENAME in advance
  Serial.print("Enter to any keys to continue:");
  while ( !Serial.available() )
  {
    delay(1);
  }
  // Initialize Internal File System
  InternalFS.begin();

  Serial.println();
}


// the setup function runs once when you press reset or power the board
void read_Flash()
{
  Serial.print("Open " FILENAME " file to read ... ");
  file.open(FILENAME, FILE_O_READ);

  // file existed
  if ( file )
  {
    Serial.println(FILENAME " file exists ");
    uint32_t readlen;
    readlen = file.read(data_read_from_Flash, 40);

    /*
    //buffer[readlen] = 0;
    for (int i = 0; i < 40; i++) {
      Serial.print(buffer[i]);
      Serial.print("--> ");
    }
    Serial.println();
    */
    
    file.close();
  } else
  {
    Serial.println("Failed!");
  }

  Serial.println("Done");
}

void write_Flash(uint8_t* contents) {
  Serial.print("Open " FILENAME " file to write ... ");

  if ( file.open(FILENAME, FILE_O_WRITE) )
  {
    Serial.println("OK");

    file.write(contents, 40);
    file.close();
  }
  else
  {
    Serial.println("Failed!");
  }

  Serial.println("Done");
}

void format_Flash() {
  Serial.println("InternalFS Format");
  Serial.println();

  // Wait for user input to run.
  //Serial.println("This sketch will destroy all of your data in External Flash!");
  //Serial.print("Enter any keys to continue:");
  //while ( !Serial.available() ) delay(1);
  //Serial.println();
  //Serial.println();

  // Initialize Internal File System
  //InternalFS.begin();

  Serial.print("Formating ... ");
  delay(1); // for message appear on monitor

  // Format 
  InternalFS.format();

  Serial.println("Done");
}
