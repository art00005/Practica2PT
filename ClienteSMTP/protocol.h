#ifndef protocolostpte_practicas_headerfile
#define protocolostpte_practicas_headerfile
#endif

// COMANDOS DE APLICACION
#define SC "USER"  // SOLICITUD DE CONEXION USER usuario 
#define PW "PASS"  // Password del usuario  PASS password

#define SD  "QUIT"  // Finalizacion de la conexion de aplicacion
#define SD2 "EXIT"  // Finalizacion de la conexion de aplicacion y finaliza servidor

// RESPUESTAS A COMANDOS DE APLICACION
#define OK  "OK"
#define ER  "ERROR"

//FIN DE RESPUESTA
#define CRLF "\r\n"

//ESTADOS
#define S_INI 0
#define S_HELO 1
#define S_MAIL 2
#define S_RCPT 3
#define S_DATA 4
#define S_CAB 5
#define S_SALIR 6
#define S_QUIT 7
#define S_EXIT 8

//PUERTO DEL SERVICIO
#define TCP_SERVICE_PORT	25

// NOMBRE Y PASSWORD AUTORIZADOS
#define USER		"alumno"
#define PASSWORD	"123456"

//ENVIO
#define SP			" "
#define	ECHO		"echo"
#define OK			"OK"

//VARIABLES SMTP
#define MAIL			"MAIL"
#define FROM			"FROM"
#define RCPT			"RCPT"
#define TO				"TO"
#define DATA			"DATA"
#define EHLO			"EHLO"
#define QUIT			"QUIT"

//COMPARACION
#define COMP			"-"