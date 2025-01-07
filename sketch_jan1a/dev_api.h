
#include <Arduino_JSON.h>

String readA2D(){
   JSONVar A2D ;
   A2D["value"] = (unsigned long) analogRead(A0);
   String jsonString = JSON.stringify(A2D);

   return jsonString;
}
