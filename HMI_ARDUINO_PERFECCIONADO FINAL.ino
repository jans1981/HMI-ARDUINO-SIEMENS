#include <Settimino.h>

//PROGRAMA PARA VER LOS VALORES DE UN AUTOMATA SIEMENS EN UN LCD CONECTADO A TRAVES DE ARDUINO POR JANS MARZO 2021

 

#include <Wire.h>                 //ESTA ES LA LIBRERIA QUE HACE FALTA PARA USAR EL PROTOCOLO I2C DEL ARDUINO
#include <LiquidCrystal_I2C.h>    //ESTA ES LA LIBRERIA QUE HACE FALTA PARA USAR EL LCD , TANTO EL DE 4 LINEAS COMO EL DE 2 LINEAS

//_____________________________
//void(* resetFunc) (void) = 0; //declare reset function @ address 0     // ESTA ES UNA FUNCION QUE SE PUEDE LLAMAR DESDE EL MAIN LOOP  PARA RESETEAR POR SOFTWARE SI LA CONEXION SE BLOQUEARA , LLAMANDOLA CON resetFUNC(); pero no la voy utilizar  no hace falta
 //_____________________________
 
LiquidCrystal_I2C lcd(0x27,20,4);  // AQUI SE LE PASA LA DIRECCION I2C AL LCD , QUE ES 03F PARA EL DE 16 CARACTERES Y 2 LINEAS Y  0X27 AL DEL LCD DE 20 CARACTERES Y 4 LINEAS
                                    //CUANDO NO SE SABE LA DIRECCION I2C SE PUEDE BUSCAR CON EL PROGRAMA AL EFECTO BUSCAR I2C DE ARDUINO
 
 
//INCLUIR LAS LIBRERIAS

#include <SPI.h>                 //LIBRERIA SPI HACE FALTA PARA COMUNICARSE EL ARDUINO CON EL ETHERNET SHIELD
#include <Ethernet.h>             // LIBRERIA ETHERNET DEL LA TARJETA ETHERNET W5100
         

// DECLARAMOS LA VARIABLE DE LA DIRECCION MAC DE LA TARJETA DE RED W5100 DEL  ARDUINO

byte mac[] = {0x90,0xA2,0xDA,0x0F,0x08,0xE11};

  // DECLARAMOS LA VARIABLE DE LA DIRECCION IP DEL ARDUINO Y DEL AUTOMATA
 
IPAddress Local(192,168,1,5);   // IP LOCAL DEL ARDUINO
IPAddress PLC(192,168,1,10);   // IP DEL AUTOMATA S1200 rack 0 slot 2 para s300 y 0,0 para 1200

byte mibuffer={50}; //DECLARAMOS EL BUFFER PARA ALMACENAR LOS DATOS , ES UN BUFFER O ARRAY DE 50 BYTES SUFICIENTE PARA LO QUE NECESITAMOS , NO SE DEBE TRANAJAR CON ARRAYS DE BYTES GRANDES NI HACER OPERACIONES DE LEER MUCHOS CON FUNCIONES
                    //PORQUE TE PPUEDES COMER TODA LA MEMORIA Y DESPUES TE FALLAN LOS DISPOSITIVOS QUE USAN MEMORIA , LCDS , ETC , ASI QUE USAR LO MINIMO NECESARIO SIEMPRE



byte * puntero=&mibuffer; //DECLARAMOS UN PUNTERO CON * (asterisco), LOS PUENTEROS SE DECLARAN ASI  X EJEMPLO BYTE * NOMBREPUNETRO; Y SI LO QUIERES APUNTAR AL BUFFER LE DICES = &NOMBREDELBUFFER Y LO APUNTAMOS AL BUFFER CON EL & aspersand ,
                          //el & ASPERSAND AL PONERLO DELANTE DE UNA VARIABLE LO QUE HACES ES QUE ESA VARIABLE INDICA LA DIRECCION DE MEMORIA DONDE ESTA DICHA VARIABLE
                          //por ejemplo SI QUIERES ANOTAR EN UNA VARIABLE INT LA DIRECCION DE OTRA VARIABLE INT , HACES int var1=&var2 , asi la direccion de memoria de  var2 queda almacenda en var1
                          //POR ESO EN EL PUNTERO SE VA A GUARDAR LA DIRECCION DONDE SE ALMACENA LA VARIABLE MIBUFFER , EL PUNTERO VA APUNTAR A ESA DIRECCION PARA DESPUES RECORRERLA ADELANTE O ATRAS SEGUN NECESITEMOS X BYTES

float MD10;   //DECLARAMOS UNA VARIABLE TIPO FLOAT PARA LEER LA VARIABLE MD10
float MD20;   //DECLARAMOS UNA VARIABLE TIPO FLOAT PARA LEER LA VARIABLE MD20
float MD30;   //DECLARAMOS UNA VARIABLE TIPO FLOAT PARA LEER LA VARIABLE MD30
float MD40;   //DECLARAMOS UNA VARIABLE TIPO FLOAT PARA LEER LA VARIABLE MD40


boolean primerencendido;

boolean marca0;
boolean marca1;
boolean marca2;
boolean marca3;
boolean marca4;
boolean marca5;
boolean marca6;
boolean marca7;







S7Client Client;  //INICIALIZAR EL CLIENTE DE S7 (SETIMINO BASADO EN SNAP7)


//---------------------------------------------------------------------- INICIALIZAR PUERTO SERIE Y ETHERNET
void setup() {
   lcd.init(); //IMPORTANTE ENCIENDE EL LCD
  lcd.backlight(); //LE DAMOS LUZ
  
     
    Serial.begin(9600);   //INICIALIZO PUERTO SERIE A 9600 BAUDIOS PARA MONITOREAR

    // INICIALIZAR LA LIBRERIA ETHERNET Y ACTIVAR LA TARJETA DE RED
   
    Ethernet.begin(mac, Local);

    primerencendido=true; //SE PONE A TRUE LA PRIMERA UNICA VEZ EN VOID SETUP





 
   
}

//---------------------------------------------------------------------- LEER LOS DATOS DEL PLC -----------------

void loop()  {







//------------------CONECTAR AL PLC SIEMENS SIMATIC S300

  int conexion=Client.ConnectTo(PLC,0,2);                        // CONECTA AL PLC RACK Y SLOT DEL PLC  ****************ACORDARSE QUE EN S300/400 RACK 0 SLOT 2 Y EN EL 1200 RACK 0 SLOT 0

  delay (200); //ESPERAMOS 1 SEGUNDOS A QUE SE REALICE LA CONEXION
 
  if (conexion == 0)  //SI EL CLIENTE ESTA CONECTADO
   {
  Serial.print("PLC CONECTADO");
   }
   
  else
{
  Serial.print("PLC DESCONECTADO , VOLVIENDO A CONECTAR"); //SI NO CONECTA , REINTENTA DE NUEVO LA CONEXION
    Client.ConnectTo(PLC,0,2);
    
    delay(100);
    
}


























  


    //AQUI ARRANCA EL PROGRAMA PRINCIPAL QUE SE REPITE


 Serial.print(" TAMAÑO DE LA TRAMA PDU ENVIADA: ");
 Serial.println(Client.GetPDULength()); //IMPRIME EL TRAMAÑO DE LA TRAMA PDU QUE SE ESTA ENVIANDO DEL AUTOMATA AL ARDUINO
 
 MD10=0.00;     //PONER LAS VARIABLES A CERO POR SI CAE LA CONEXION QUE QUEDEN A CERO
 MD20=0.00;
 MD30=0.00;
 MD40=0.00;
 
 
 

Client.ReadArea(S7AreaMK,0,0,50,puntero); //AQUI LE DECIMOS LEEME LOS BYTES DEL PLC DESDE 0 HASTA 50 y los meta en el buffer a traves del puntero


                                             

MD10 = S7.FloatAt(&mibuffer,10);  //AQUI LEEMOS EL BYTE 10 DEL BUFER , LA MARCA 10 EN FORMATO FLOAT , EN REAL
MD20 = S7.FloatAt(&mibuffer,20); //AQUI LEEMOS EL BYTE 20 DEL BUFFER , LA MARCA 20 EN FORMATO FLOAT , EN REAL
MD30 = S7.FloatAt(&mibuffer,30); //AQUI LEEMOS EL BYTE 30 DEL BUFFER , LA MARCA 30 EN FORMATO FLOAT , EN REAL
MD40 = S7.FloatAt(&mibuffer,40); //AQUI LEEMOS EL BYTE 30 DEL BUFFER , LA MARCA 40 EN FORMATO FLOAT , EN REAL





//--------------------------------------------leer bits de marcas 

//asi se leen los bits de marcas , si poner el buffer , sino , no funciona , en el buffer ya puesto antes ya establecimos leer el area de marcas S7AreaMK


marca0 = S7.BitAt(0,0); //SIN PONER EL BUFFER
marca1 = S7.BitAt(0,1);  //SIN PONER EL BUFFER
marca2 = S7.BitAt(0,2);  
marca3 = S7.BitAt(0,3);



Serial.print(marca0);
Serial.print(marca1);
Serial.print(marca2);
Serial.print(marca3);









Serial.print("EL VALOR  DE MD10  ES : ");

Serial.print(MD10); // Y LA IMPRIMIMOS POR EL PUERTO SERIE

Serial.print("EL VALOR  DE MD20  ES : ");

Serial.print(MD20); // Y LA IMPRIMIMOS POR EL PUERTO SERIE


Serial.print("EL VALOR  DE MD30  ES : ");

Serial.print(MD30); // Y LA IMPRIMIMOS POR EL PUERTO SERIE


Serial.print("EL VALOR  DE MD40  ES : ");

Serial.print(MD40); // Y LA IMPRIMIMOS POR EL PUERTO SERIE


//EMPEZAR A MOSTRAR DATOS EN EL LCD

//PRIMERO SE MUESTRA UNA SOLA VEZ AL ENCENDIDO  EL TITULO DEL HMI , IP DEL AUTOMATA Y DEL PLC 

if (primerencendido==true) {
  
  lcd.setCursor(0,0);
  
  lcd.print("  HMI SIEMENS PLC .");

  lcd.setCursor(0,1);
  lcd.print("IP PLC: 192.168.1.10");
  lcd.setCursor(0,2);
  lcd.print("IP HMI: 192.168.1.5");
  lcd.setCursor(0,3);
  lcd.print("SNAP7 BASED BY JANS");
  
  delay (5000);
  lcd.clear();
  primerencendido=false; //AQUI LO PONGO A FALSE , SALGO DEL BUCLE Y ASI LA PROXIMA VEZ YA NO SE EJECUTA EL CODIGO , SOLO SE EJECUTARA LA PRIMERA VEZ QUE ENCIENDO EL HMI
}


  




  

  //--------------MOSTRAR MD10
  
  lcd.setCursor(0,0); ///POSICIONAMOS SOBRE PRIMERA LINEA
  lcd.print("MD10:");   //IMPRIMIMOS LA ETIQUETA DE LA VARIABLE EN EL LCD , TEMP PRESSURE , LEVEL ...ETC O EL NOMBRE DEL BIT O LA MARCA O LO QUE QUERAMOS
 lcd.setCursor(6,0);   //VAMOS A LA MITAD DE LA PRIMERA LINEA MAS O MENOS (POSICION 6 DE LA LINEA)
lcd.print(MD10);      //IMPRIMIMOS LA VARIABLES QUE SACAMOS DEL BUFFER EN ESA POSICION

//----------------------------MOSTRAR MD20
lcd.setCursor(0,1);     //VAMOS AL  PRINCIPIO DE LA SEGUNDA LINEA Y LO MISMO , SI EL LCD FALLA O FALTAN LINEAS , PUEDE SER QUE ESTES USANDO MUCHA MEMORIA O BUFFERS EN EL ARDUINO
lcd.print("MD20:");
lcd.setCursor(6,1);
lcd.print(MD20);
//--------------------------MOSTRAR MD30
lcd.setCursor(0,2);     //VAMOS AL  PRINCIPIO DE LA TERCERA LINEA Y LO MISMO , SI EL LCD FALLA O FALTAN LINEAS , PUEDE SER QUE ESTES USANDO MUCHA MEMORIA O BUFFERS EN EL ARDUINO
lcd.print("MD30:");

lcd.setCursor(6,2);
lcd.print(MD30);


lcd.setCursor(0,3);     //VAMOS AL  PRINCIPIO DE LA CUARTA LINEA Y LO MISMO , SI EL LCD FALLA O FALTAN LINEAS , PUEDE SER QUE ESTES USANDO MUCHA MEMORIA O BUFFERS EN EL ARDUINO
lcd.print("MD40:");

lcd.setCursor(6,3);
lcd.print(MD40);






//------------------------------------AHORA AQUI VAMOS ENSEÑAR EN EL LCD QUE BITS DE MARCAS ESTAN ACTIVAS


//NOS POSICIONAMOS CASI AL FINAL DE LA PRIMERA LINEA DEL LCD:



//************************************YO SUELO USAR ESTA PRIMERA MARCA PARPADEANDO COMO MARCA DE CICLO COMO HEARTBEAT PARA VIGILAR LA CONEXION.
 lcd.setCursor(14,0); 

 if (marca0 == true) 
    lcd.print("M0.0 *");
 else
   lcd.print("M0.0  "); 
//-----------------------------NOS SITUAMOS A MEDIADOS DE  LA SEGUNDA LINEA  PARA VISUALIZAR EL ESTADO DEL BIT DE OTRA MARCA

lcd.setCursor(14,1); 

 if (marca1 == true) 
    lcd.print("M0.1 *");
 else
   lcd.print("M0.1  ");
//-----------------------------NOS SITUAMOS A MEDIADOS DE  LA TERCERA LINEA  PARA VISUALIZAR EL ESTADO DEL BIT DE OTRA MARCA
lcd.setCursor(14,2); 

 if (marca2 == true) 
    lcd.print("M0.2 *");
 else
   lcd.print("M0.2  ");
//---------------------------------------------
   //-----------------------------NOS SITUAMOS A MEDIADOS DE  LA CUARTA  PARA VISUALIZAR EL ESTADO DEL BIT DE OTRA MARCA
lcd.setCursor(14,3); 

 if (marca3 == true) 
    lcd.print("M0.3 *");
 else
   lcd.print("M0.3  ");
 
 
 delay(200); 
//-----------------------------------------REPETIR VISUALIZACION EN 200 MILISEGUNDOS

 

 mibuffer==NULL;  //BORRADO DEL BUFFER ANTES DE HACER OTRA LECTURA
 MD10=0.00;     //PONER LAS VARIABLES A CERO POR SI CAE LA CONEXION QUE QUEDEN A CERO
 MD20=0.00;
 MD30=0.00;
 MD40=0.00;


     //ESTO REFRESCA EL LCD SIN PARPADEO , LO HAY QUE PONER DESPUES DE UN DELAY  , YA QUE SI NO SE BORRARIA TAN RAPIDO QUE NO VERIAS NADA
}
//DESPUES DEL DELAY PUEDES PONER OTRAS LINEAS IGUAL QUE LAS ANTERIORES SEGUIDAS DE OTRO DELAY Y ASI MOSTRAR OTRAS TANTAS VARIABLES , YA QUE SE IRIAN VIENDO INTERMITENTES MOSTRADO CADA 2 SEGUNDOS 2 VARIABLES EN LAS 2 LINEAS DEL LCD
                                             // FIN DEL PROGRAMA   jans marzo 2021
