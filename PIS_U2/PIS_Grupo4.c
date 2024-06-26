#include <stdio.h>
#include <time.h> 
#include <math.h>
//ESTRUCTURAS
struct Ubicacion{
    float lat, lon;
}; 
struct TiempoActual{
    int aC,mC,dC,hC,minC;
};
struct Tiempo{
    int a,m,d,a1,m1,d1;
};
struct DeclinacionSolar{
    double dsG, dsR;
};
struct EcuacionTiempo{
    double etG, etR;
};
struct ZonaHoraria{
    int lonEstand, zH;
};
struct HoraSolarVerdadera{
    double TsD; 
    int TsH, TsM;
};
struct AlturaSol{
    double HG, HR, asG, asR;
};
struct Azimut{
    double aziG, aziR;
};
struct PanelSolarSpa{
    double aziG, asG;
};
//FUNCIONES Y PROCEDIMIENTOS
//CONVERSIONES
void Pgar(double *variable){
    double Pi=3.1415926535;
    *variable=((*variable*Pi)/180.0); //Conversion de Grados a Radianes
}
void Prag(double *variable){
    double Pi=3.1415926535;
    *variable=((*variable*180)/Pi);   //Conversion de Radianes a Grados
}
//TIEMPO DEL SISTEMA
void PtiempoActual(int *aC,int *mC, int *dC, int *hC, int *minC){
    time_t t;
    time(&t);
    struct tm *tm_info = localtime(&t);
    *aC = tm_info->tm_year + 1900; //Numero de aÑos desde 1900
    *mC = tm_info->tm_mon + 1;     //Numero de meses desde (0-11)
    *dC = tm_info->tm_mday;
    *hC = tm_info->tm_hour;
    *minC = tm_info->tm_min;
}
//CALCULOS Y DATOS NECESARIOS PARA LAS ECUACIONES
void PubicacionDireccion(float lat, float lon){ 
    //Procedimiento para determinar direcciones: N,S,E,O
    if (lat>0){
        printf("Valor de latitud de su zona geografica\n%f N\n", lat);
    } else{
        printf("Valor de latitud de su zona geografica\n%f S\n", (-1*lat));
    }
    if (lon>0){
        printf("Valor de longitud de su zona geografica\n%f E\n", lon);
    } else{
        printf("Valor de longitud de su zona geografica\n%f O\n", (-1*lon));
    }
}
void Ptiempo(int *a, int aC, int *m, int mC, int *d, int dC, int *a1, int *m1, int *d1){
    //Procedimiento para determinar el tiempo transcurrido durante este aÑo.
    *d=1; //Dia en el que se empieza
    *m=1; //Mes en el que se empieza
    *a1=aC-*a; //Para contabilizar que el año siempre sea 0
    *m1=mC-*m; //Para contabilizar los meses completos que han habido
    for(*m=1; *m<mC; *m=*m+1){ //Bucle para que sume los dias correspondientes de acuerdo a cada mes del año
        if (*m==1||*m==3||*m==5||*m==7||*m==8||*m==10||*m==12){ //Meses de 31 dias
            dC+=31; 
        } else if(*m==2){ //Mes de 28 o 29 dias de acuerdo a si el año es bisiesto
            if(aC%4==0){
                dC+=29; //El año si es bisiesto
            }else{
                dC+=28; //El año no es bisiesto
            }
        } else if(*m==4||*m==6||*m==9||*m==11){ //Meses de 30 dias
            dC+=30;
        }
    }
    *d1=dC; //Para guardar en una variable el total de dias desde que empezo el año
}
//1era FORMULA: DECLINACION SOLAR
void PdeclinancionSolar(int a, int d1, double *decSolar, double *decSolarR){
    double ec; //Inicializo una variable para realizar calculos
    if(a%4==0){
        ec=360.0/366.0*(d1+10); //Para el caso de que fuera año bisiesto
    } else{
        ec=360.0/365.0*(d1+10); //Para el caso de que no fuera aÑo bisiesto
    }
    Pgar(&ec); //Convierto el resultado a Radianes
    *decSolar=(-23.44)*cos(ec); //Resultado en grados
    *decSolarR=*decSolar; //Asigno valor para ser convertido
    Pgar(&*decSolarR); //Convierto el resultado a Radianes
}
//2nda FORMULA: ECUACION DEL TIEMPO
void PecuacionTiempo(int a, int d1, double *ecTiempo, double *ecTiempoG){
    double B; //Inicializo una variable para realizar calculos
    if(a%4==0){
        B=360.0/366.0*(d1-81); //Para el caso de que fuera año bisiesto
    } else{
        B=360.0/365.0*(d1-81); //Para el caso de que no fuera aÑo bisiesto
    }
    Pgar(&B); //Convierto el resultado a Radianes
    *ecTiempo=9.87*(sin(2*B))-(7.53*cos(B))-(1.5*sin(B)); //Resultado en radianes
    *ecTiempoG=*ecTiempo; //Asigno valor para ser convertido
    Prag(&*ecTiempoG); //Convierto el resultado a Grados
}
//3era FORMULA: TIEMPO SOLAR VERDADERO
void PzonaHoraria(int lonEntero, int *lonEs, int *zH){
    //Procedimiento para obtener la longitud Estandar y la Zona Horaria
    if (lonEntero%15==0){ //En el caso de que la parte entera de la coordenada si fuera divisible para 15
        *lonEs=lonEntero; 
        *zH=lonEntero/15;
    }else{
        if (lonEntero>0){ //En el caso de que la parte entera de la coordenada no fuera divisible para 15 y sea un valor positivo
            while(lonEntero%15!=0){ //Bucle para al valor restarle hasta que si sea divisible
                lonEntero--;
            }
            *lonEs=lonEntero; 
            *zH=lonEntero/15;
        } else{  //En el caso de que la parte entera de la coordenada no fuera divisible para 15 y sea un valor negativo
            while(lonEntero%15!=0){ //Bucle para al valor sumarle hasta que si sea divisible
                lonEntero++;
            }
            *lonEs=lonEntero;
            *zH=lonEntero/15;
        }
    }
}
void PhoraSolarVerdadera(int hC, int minC, float lon, int lonEs, double EcTiempo, double *TsD, int *TsH, int *TsM){
    double hdec=hC+(minC/60.0); //Inicializo una variable para calcular la hora en decimales
    *TsD=hdec+((4*(lon-lonEs)+EcTiempo)/60.0); //Resultado del Tiempo Solar en decimales
    *TsH=*TsD; //Asigno el valor y obtengo el Tiempo Solar en horas
    *TsM= round((*TsD-*TsH)*60); //Calculo y redondeo el valor, obteniendo asi el valor del Tiempo Solar en minutos 
}
//4rta Formula: ALTURA DEL SOL
void PalturaSol(double TsD, double *HG, double *HR, double lat , double decSolar, double *asR, double *asG){
    *HG=15*(TsD-12); //Calculo la altura del sol sobre el horizonte en Grados
    *HR=*HG; //Asigno valor para ser convertido
    Pgar(&*HR); //Convierto el resultado a Radianes
    Pgar(&lat); //Convierto la latitud a Radianes
    *asR=asin((sin(decSolar)*sin(lat))+(cos(decSolar)*cos(lat)*cos(*HR))); //Resultado en radianes
    *asG=*asR; //Asigno valor para ser convertido
    Prag(&*asG); //Convierto el resultado a Grados
}
//5nta FORMULA: AZIMUT
void Pazimut(double decSolar, double asR, double lat, double HR ,double *aziR, double *aziG){
    double Pi=3.1415926535; //Inicializo el valor de Pi
    Pgar(&lat); //Convierto la latitud a Radianes
    *aziR=acos((sin(decSolar)-sin(asR)*sin(lat))/(cos(asR)*cos(lat))); //Resultado en grados
    if (HR>0){ //Condicion para cuando la altura del sol sobre el horizonte sea mayor a 0
        *aziR=(2*Pi)-*aziR;
        printf("El valor del azimut cambio\n"); //Indicamos en el caso de que se cumpla la condicion
    }
    *aziG=*aziR; //Asigno valor para ser convertido
    Prag(&*aziG); //Convierto el resultado a Grados
}
//FUNCION PRINCIPAL
int main(){
    //FECHA Y HORA
    struct TiempoActual tmpa; //  <-------- ESTRUCTURA
    PtiempoActual((&tmpa.aC),(&tmpa.mC),(&tmpa.dC),(&tmpa.hC),(&tmpa.minC)); //Llamo a la funcion
    printf("Fecha: %d/%d/%d , Hora: %d:%d\n", (tmpa.dC),(tmpa.mC),(tmpa.aC),(tmpa.hC),(tmpa.minC)); //Escribo los valores que me devuelve la funcion
    //COORDENADAS
    struct Ubicacion coordenada; //  <-------- ESTRUCTURA
    printf("Bienvenido al programa para calcular la direccion y altura del sol\nSe requieren de tus coordenadas para realizar el calculo\n");
    while(1){ //Bucle para detectar errores
        int datoingresado;
        printf("Ingresa como dato flotante el valor de la latitud de tu zona. Teniendo en cuenta el rango [-90,90]\n");
        datoingresado=scanf("%f", &coordenada.lat); //Para ingresar el valor de latitud
        if (datoingresado==1){ //Si el dato ingreado es correcto, procede a salir del bucle e hacer los calculos
            break; //Para salir del bucle
        } else{
            printf("Error: Entrada invalida. El dato ingresado no es un valor numerico\n"); //Indico que existe un error
            while (getchar()!='\n'); //Para limpiar el buffer de entrada y descartar los caracteres hasta encontrar un salto de linea
        }
    }
    while(1){ //Bucle para detectar errores
        int datoingresado;
        printf("Ingresa como dato flotante el valor de la longitud de tu zona. Teniendo en cuenta el rango [-180,180]\n");
        datoingresado=scanf("%f", &coordenada.lon); //Para ingresar el valor de longitud
        if (datoingresado==1){ //Si el dato ingreado es correcto, procede a salir del bucle e hacer los calculos
            break; //Para salir del bucle
        } else{
            printf("Error: Entrada invalida. El dato ingresado no es un valor numerico\n"); //Indico que existe un error
            while (getchar()!='\n'); //Para limpiar el buffer de entrada y descartar los caracteres hasta encontrar un salto de linea
        }
    }
    if (coordenada.lat==0){ //Para ingresar automaticamente el valor de prueba
        coordenada.lat=-3.99313;
    }
    if (coordenada.lon==0){ //Para ingresar automaticamente el valor de prueba
        coordenada.lon=-79.20422; 
    }
    PubicacionDireccion((coordenada.lat),(coordenada.lon)); //Llamo a la funcion
    //DIAS TRANSCURRIDOS
    struct Tiempo tmp; //  <-------- Estructura
    tmp.a=tmpa.aC; //Asigno el mismo valor para el año de referencia
    Ptiempo((&tmp.a),(tmpa.aC),(&tmp.m),(tmpa.mC),(&tmp.d),(tmpa.dC),(&tmp.a1),(&tmp.m1),(&tmp.d1)); //Llamo a la funcion
    printf("Fechas hasta ahora: %d anios, %d meses, %d dias\n", (tmp.a1),(tmp.m1),(tmp.d1)); //Escribo los valores que me devuelve la funcion
    //1era FORMULA: DECLINACION SOLAR
    //Variables utilizadas: Año, Dias que han transcurrido desde que inicio el año
    struct DeclinacionSolar decSolar; //  <----------- Estructura
    PdeclinancionSolar((tmp.a),(tmp.d1),(&decSolar.dsG),(&decSolar.dsR)); //Llamo a la funcion
    printf("Valor de la declinacion solar %f en grados\n", decSolar.dsG); //Escribo los valores que me devuelve la funcion
    //2nda FORMULA: ECUACION DEL TIEMPO
	//Variables utilizadas: Año, Dias que han transcurrido desde que inicio el año
    struct EcuacionTiempo ecTiempo; //  <------------- Estructura
    PecuacionTiempo((tmp.a),(tmp.d1),(&ecTiempo.etR),(&ecTiempo.etG)); //Llamo a la funcion
    printf("Valor de la ecuacion del tiempo %f en radianes\n", ecTiempo.etR); //Escribo los valores que me devuelve la funcion
    //LONGITUD ESTANDAR Y ZONA HORARIA
    struct ZonaHoraria zhoraria; //  <---------------- Estructura
    PzonaHoraria((coordenada.lon),(&zhoraria.lonEstand),(&zhoraria.zH)); //Llamo a la funcion
    printf("Valor de la longitud estandar: %d\nTu zona horaria es: %d\n", zhoraria.lonEstand, zhoraria.zH); //Escribo los valores que me devuelve la funcion
    //3era FORMULA: TIEMPO SOLAR VERDADERO
    //Variables utilizadas: Hora y Minuto actual, Longitud, Longitud Estandar, Ecuacion del Tiempo
    struct HoraSolarVerdadera TiempoSolar; //  <------ Estructura
    PhoraSolarVerdadera((tmpa.hC),(tmpa.minC),(coordenada.lon),(zhoraria.lonEstand),(ecTiempo.etR),(&TiempoSolar.TsD),(&TiempoSolar.TsH),(&TiempoSolar.TsM)); //Llamo a la funcion
    printf("Valor de la Hora solar:\nEn decimales: %f\nEn horas: %d\nEn minutos: %d\n", (TiempoSolar.TsD),(TiempoSolar.TsH),(TiempoSolar.TsM)); //Escribo los valores que me devuelve la funcion
    //4rta FORMULA: ALTURA DEL SOL
	//Variables utilizadas: Tiempo Solar, ALtura del Sol sobre el Horizonte (H), latitud, Declinacion Solar
    struct AlturaSol altSol; //  <-------------------- Estructura
    PalturaSol((TiempoSolar.TsD),(&altSol.HG),(&altSol.HR),(coordenada.lat),(decSolar.dsR),(&altSol.asR),(&altSol.asG)); //Llamo a la funcion
    printf("Valor de H: %f en radianes\nValor de la altura solar:\nEn radianes: %f\nEn grados: %f\n", (altSol.HR),(altSol.asR),(altSol.asG)); //Escribo los valores que me devuelve la funcion
    //5nta FORMULA: AZIMUT
	//Variables utilizadas: Declinacion SoLar, Altura del Sol, latitud, ALtura del Sol sobre el Horizonte (H)
    struct Azimut azi; //  <-------------------------- Estructura
    Pazimut((decSolar.dsR),(altSol.asR),(coordenada.lat),(altSol.HR),(&azi.aziR),(&azi.aziG)); //Llamo a la funcion
    printf("El valor del Azimut: %f en radianes\nEl valor del Azimut: %f en grados\n", (azi.aziR),(azi.aziG)); //Escribo los valores que me devuelve la funcion
    struct PanelSolarSpa psSpa; //  <----------------- Estructura
    psSpa.asG=altSol.asG; //Guardo los valores que son necesarios en Y para el panel solar
    psSpa.aziG=azi.aziG; //Guardo los valores que son necesarios en X para el panel solar
    printf("Los valores que van a ir hacia el panel son:\nPara X: %f grados <-- Azimut\nPara Y: %f grados <-- Altura Solar\n", (psSpa.aziG),(psSpa.asG)); //Escribo los valores guardados en la estructura
    return 0;
}