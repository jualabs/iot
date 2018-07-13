#include "Communication.h"

// WidgetTable table;

// BLYNK_ATTACH_WIDGET(table, V1);

int rowIndex = 0;

// Button on V10 adds new items
/*
BLYNK_WRITE(V9) {
  if (param.asInt()) {
    table.addRow(rowIndex, "Test row", millis() / 1000);
    table.pickRow(rowIndex);
    rowIndex++;
  }
}

// Button on V11 clears the table
BLYNK_WRITE(V11) {
  if (param.asInt()) {
    table.clear();
    rowIndex = 0;
  }
}
*/

Communication* comm;

void dataCallbackWrapper(uint32_t *client, const char* topic, uint32_t topic_len, const char *data, uint32_t lenght) {
}

// global static pointer used to ensure a single instance of the class.
Communication* Communication::pInstance = nullptr;

/** This function is called to create an instance of the class.
    Calling the constructor publicly is not allowed. The constructor
    is private and is only called by this getInstance() function.
*/

Communication* Communication::getInstance() {
   if (!pInstance)   // Only allow one instance of class to be generated.
      pInstance = new Communication();

   return pInstance;
}

Communication::Communication() {
}

void Communication::initCommunication() {
/*
	Blynk.begin(auth, ssid, pass);
	  // Setup table event callbacks
	  table.onOrderChange([](int indexFrom, int indexTo) {
	    Serial.print("Reordering: ");
	    Serial.print(indexFrom);
	    Serial.print(" => ");
	    Serial.print(indexTo);
	    Serial.println();
	  });
	  table.onSelectChange([](int index, bool selected) {
	    Serial.print("Item ");
	    Serial.print(index);
	    Serial.print(selected ? " marked" : " unmarked");
	  });
*/
}

void Communication::checkCommunication() {
//	Blynk.run();
}
