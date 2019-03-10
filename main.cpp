#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <algorithm>

#define JUGADOR_A "A"
#define JUGADOR_B "B"

#define PEON true
#define MURO false

#define A 0
#define B 1

#define Y 1
#define X 0

#define NextTurno (Turno == A)?(B):(A)

using namespace std;

int PosMuros[8][8] = {-1}; //[PosMuroX][PosMuroY] = X o Y (Si esta vertical u horizontal
int NextMuro[8][8] = {-1};
int IDXMuro = 0;
int IDYMuro = 0;
int Posicion[2][2] = {{4, 0}, {4, 8}}; // X, Y
int NextPosi[2][2] = {{4, 0}, {4, 8}}; // X, Y

bool FlagA[9][9], FlagB[9][9];

int MurosTotales[2] = {5, 5};
bool Turno = A; // False: Turno de A || True: Turno de B
bool FichaActual = PEON;

string Mensaje = "";


void Debug()
{
    /*cout << "\n";
    cout << "Posicion[A][X]: " << Posicion[A][X] << " __ NextPosi[A][X]: " << NextPosi[A][X] << endl;
    cout << "Posicion[A][Y]: " << Posicion[A][Y] << " __ NextPosi[A][Y]: " << NextPosi[A][Y] << endl;
    cout << "Posicion[B][X]: " << Posicion[B][X] << " __ NextPosi[B][X]: " << NextPosi[B][X] << endl;
    cout << "Posicion[B][Y]: " << Posicion[B][Y] << " __ NextPosi[B][Y]: " << NextPosi[B][Y] << endl;*/

    /*for(int j = 0; j < 8; j++)
    {
        for(int i = 0; i < 8; i++)
        {
            cout << "NM[" << j << "][" << i << "] = " << NextMuro[j][i] << "; ";
        }
        cout << endl;
    }*/

    /*for(int j = 0; j < 8; j++)
    {
        for(int i = 0; i < 8; i++)
        {
            cout << "PM[" << j << "][" << i << "] = " << PosMuros[j][i] << "; ";
        }
        cout << endl;
    }*/

    /*cout << "IDXMuro: " << IDXMuro << " IDYMuro: " << IDYMuro;
    cout << "\n";*/
}

void ImprimirTablero()
{
    // \xCD linea horizontal
    // \xBA linea vertical
    // \xCE cruz
    // \xB9 linea vertical con pico a la izq

    system("cls");

    cout << "Y\\X\xBA  0 1 2 3 4 5 6 7 8\xBA" << endl; // Lineas X superiores
    cout << "\xCD\xCD\xCD\xCE"; // Inicio izquierdo lineas verticales sup

    for(int i = 0; i < 19; i++) cout << "\xCD"; // Lineas verticales superiores

    cout << "\xB9" << endl; // Linea final de la parte superior

    // Empezamos a hacer el interior del tablero
    for(int i = 0; i < 9; i++) // Y
    {
        cout << "  " << " \xBA ";
        for(int j = 0; j < 9; j++) // X
        {
            // Posicionamos al jugador o ponemos el cuadrado
            if(Posicion[A][0] == j && Posicion[A][1] == i) cout << JUGADOR_A;
            else if(Posicion[B][0] == j && Posicion[B][1] == i) cout << JUGADOR_B;
            else
            {
                if(NextPosi[Turno][X] == j && NextPosi[Turno][Y] == i) cout << "x"; // Ponemos la x donde se va a mover el jugador cuando presione Enter
                else cout << "\xB1"; // Se pone el cuadro jugable"
            }

            if(j < 8 && i > 0 && NextMuro[j][i-1] == Y) cout << "5";
            else
            {
                if(NextMuro[j][i] != -1)
                {
                    if(j < 8 && i < 8 && NextMuro[j][i] == Y) cout << "4"; // Pone el # en la linea jugable
                    else if(j < 8 && i > 0 && i < 8 && PosMuros[j][i-1] == Y) cout << "2"; else cout << " "; // Aca puede haber un muro vertical
                    //La comprobacion de la linea de arriba es para poner el 2
                }
                else
                {
                    if(j < 8 && i < 8 && PosMuros[j][i] == Y) cout << "1"; // Pone el # en la linea jugable
                    else if(j < 8 && i > 0 && i < 8 && PosMuros[j][i-1] == Y && NextMuro[j][i-1] == -1) cout << "2";
                    else cout << " "; // Aca puede haber un muro vertical
                }
            }



        }
        cout << "\xBA" << endl; // Saltamos de linea para dejar espacio a los muros horizontales
        cout << " " << i << " \xBA ";
        for(int j = 0; j < 9; j++)
        {
            if(j < 8 && i < 8 && NextMuro[j][i] == X) cout << "@@@";
            else if(j < 8 && i < 8 && PosMuros[j][i] == X) cout << "###";
            else if(j < 8 && j > 0 && i < 8 && PosMuros[j-1][i] == X) // Si en la anterior hay muro
            {
                if(PosMuros[j][i] == Y) cout << "3"; else if(NextMuro[j][i] == Y) cout << "6"; else cout << " "; // Aca puede haber un muro horizontal
            }
            else
            {
                if(j < 8 && i < 8 && PosMuros[j][i] == Y) cout << " 3"; // Pone el # en la linea de espacios con un espacio antes
                else if(j < 8 && i < 8 && NextMuro[j][i] == Y) cout << " 6";
                else
                {
                    cout << "  ";
                }
            }

        }
        bool cont = false;
        for(int h = 0; h < 8; h++) if(NextMuro[h][i] == X/* || PosMuros[i][h] == X*/) cont = true;
        if(cont == true) cout << "\e[D"; // Elimina el espacio todo feo que queda y desordena el tablero
        //cout << " ";
        cout << "\xBA";
        if(i == 0) cout << "\tEs el turno del jugador " << ( (Turno == A)?("A"):("B") ) << ". Muros Disponibles: " << MurosTotales[Turno];
        if(i == 1) cout << "\t@: La ficha actual es: " << ( (FichaActual == PEON)?("Peon"):("Muro ")) << ( (FichaActual == MURO)?(NextMuro[IDXMuro][IDYMuro]):(-2) );
        if(i == 3) cout << "\t" << Mensaje;
        if(i == 4) cout << "\tPara mover el Peon/Muro, usa las flechas";
        if(i == 5) cout << "\tCon el Muro, usa la letra O para cambiar entre vertical y horizontal";
        if(i == 6) cout << "\tPara cambiar entre Peon y Muro, usa TAB";
        if(i == 7) cout << "\tPara terminar el turno y mover el Peon/Muro a la posicion elegida, usa ENTER";
        cout << endl;
    }

    // El final del tablero
    cout << "\xCD\xCD\xCD\xCA";
    for(int i = 0; i < 19; i++){
        cout << "\xCD";
    }
    cout << "\xBC";
    Debug();
}

int ComprobarProximaPos(int DaEnter = 0, int UPorIZQ = 3) //0 UP
{
    Mensaje = " ";
    if(NextPosi[Turno][X] < 0 || // Llego al limite izquierdo
       NextPosi[Turno][Y] < 0 || // LLego al limite superior
       NextPosi[Turno][X] > 8 || // Llego al limite derecho
       NextPosi[Turno][Y] > 8)   // Llego al limite inferior
    {
        Mensaje = "No puedes mover las fichas fuera del tablero!";
        Beep(700, 250);
        return 0;
    }


    if(NextPosi[B][Y] == 0 || // B LLego al limite contrario
    NextPosi[A][Y] == 8)   // A LLego al limite contrario
    {
        // Ya hay un posible ganador
        Mensaje = "POSIBLE GANADOR!!!. Solo debes presionar ENTER, jugador " + (string)((Turno == A)?(JUGADOR_A):(JUGADOR_B));

    }
            if(UPorIZQ == 3) // abajo
        {

        }
    if(DaEnter == 1) // Presiona Enter
    {

        if(NextPosi[Turno][X] == Posicion[Turno][X] && NextPosi[Turno][Y] == Posicion[Turno][Y]) // La posicion es igual
        {
            Mensaje = "No puedes mover en la misma posicion al Peon!";
            Beep(600, 250);
            return 0;
        }

        /*else if(NextPosi[Turno][X] == Posicion[NextTurno][X] && NextPosi[Turno][Y] == Posicion[NextTurno][Y]) // Misma pos que contrincante
        {
            Mensaje = "Mueves dos casillas saltando al enemigo.";
            Beep(200, 250);
            return 2;
        }*/
        else // Movera a la nueva posicion
        {

            Posicion[Turno][X] = NextPosi[Turno][X];
            Posicion[Turno][Y] = NextPosi[Turno][Y];
            Beep(400, 250);
            if(Posicion[B][Y] == 0 || // B LLego al limite contrario
                Posicion[A][Y] == 8)   // A LLego al limite contrario
            {
                // Ya hay un ganador, hay que terminar la partida
                Mensaje = "GANADOR!!!. Ha ganador el jugador " + (string)((Turno == A)?(JUGADOR_A):(JUGADOR_B));
                return 2;
            }
            return 1;
        }
    }
    return 1;
}
#define debuger cout << "NX: " << NX << " NY: " << NY << " AX: " << AX << " AY: " << AY << endl;
bool Salida(int Jugador, int AX, int AY, int D)
{
    //system("pause");
    int NY, NX;
    NY = AY;
    NX = AX;
    if(D == 1) // Arriba
    {
        cout << "UP\n";
    }
    else if(D == 2) // Derecha
    {
        cout << "der\n";
    }
    else if(D == 3) // Abajo
    {
        cout << "aba\n";
    }
    else if(D == 4) // Izquierda
    {
        cout << "izq\n";
    }

    if(D == 1) // Arriba
    {
        NY--;debuger
        if(NY < 0)
        {
            cout << "Limite Up\n";
            return false;
        }
        if(PosMuros[NX][NY] == X || PosMuros[NX-1][NY] == X)
        {
            cout << "Hay Muro UP" << endl;
            return false;
        }
    }
    else if(D == 2) // Derecha
    {
        NX++;debuger
        if(NX > 8)
        {
            cout << "Limite Der\n";
            return false;
        }
        if(PosMuros[AX][AY] == Y || PosMuros[AX][AY-1] == Y)
        {
            cout << "Hay Muro DER" << endl;
            return false;
        }
    }
    else if(D == 3) // Abajo
    {
        NY++;debuger
        if(NY > 8)
        {
            cout << "Limite aba\n";
            return false;
        }
        if(PosMuros[AX][AY] == X || PosMuros[AX-1][AY] == X)
        {
            cout << "Hay Muro DN" << endl;
            system("pause");
            return false;
        }
    }
    else if(D == 4) // Izquierda
    {
        NX--;debuger
        if(NX < 0)
        {
            cout << "Limite izq\n";
            return false;
        }
        if(PosMuros[NX][NY] == Y || PosMuros[NX][NY-1] == Y)
        {
            cout << "Hay Muro IZQ" << endl;
            return false;
        }
    }
    if(Jugador == A)
    {
        if(FlagA[NX][NY] == true)
        {
            cout << "Volvio"<<endl;
            return false;
        }
        FlagA[AX][AY] = true;
        if(NY == 8)
        {
            cout << "Salida";
            return true;
        }
    }
    else if(Jugador == B) {if(NX == 0) return true;}
    return (Salida(Jugador, NX, NY, 1) ||
            Salida(Jugador, NX, NY, 2) ||
            Salida(Jugador, NX, NY, 3) ||
            Salida(Jugador, NX, NY, 4));
}


/*bool Salida(int Jugador, int PosX, int PosY, int Dire)
{
    //int Limite = (Jugador == A)?(8):(0);
    //if((Y == 0 && Jugador == B) || (Y == 8 && Jugador == A))
    //if(Jugador == B) cout << "Salida: " << Jugador << "-" << PosX << "-" << PosY << endl;
    //system("pause");
    cout << "Jugador: " << Jugador << " PosX: " << PosX << " PosY: " << PosY << endl;
    if(Jugador == A && FlagA[PosX][PosY] == true || Jugador == B && FlagB[PosX][PosY] == true) { cout << "\tYa paso por aca\n"; return false; }

    if(Jugador == A) FlagA[PosX][PosY] = true;
    else FlagB[PosX][PosY] = true;

    if(Dire == 1 || Dire == 2)
    {
        if(PosMuros[PosX][PosY] == X)
        {

        }
    }
    if((PosY == 0 && Jugador == B) || // Parte Superior. Para el jugador A es fuera del limite
       (PosY == 8 && Jugador == A)
       ) {cout << "\tLlego a la salida\n"; return true;}

    if((PosY == -1 && Jugador == A) || // Parte Superior. Para el jugador A es fuera del limite
       (PosY ==  9 && Jugador == B) || // Parte inf.      Para el jugador B es fuera del limite
       PosX == -1 || PosX == 9
       ) {cout << "\tFuera del limite\n"; return false;}

    bool    c1 = Salida(Jugador, PosX, PosY-1, 1), // arriba
            c2 = Salida(Jugador, PosX, PosY+1, 2), // Abajo
            c3 = Salida(Jugador, PosX-1, PosY, 3), // Izquierda
            c4 = Salida(Jugador, PosX+1, PosY, 4); // Derecha
    //cout << "\tRetorna: " << c1 || c2 || c3 || c4;

    return c1 || c2 || c3 || c4;
    //return true;
}*/

bool ComprobarSalida()
{
    memset( &FlagA[0][0], 0, sizeof(FlagA) );
    memset( &FlagB[0][0], 0, sizeof(FlagB) );
    //FlagA[Posicion[A][X]][Posicion[A][Y]] = true;
    bool SA = Salida(A, Posicion[A][X], Posicion[A][Y], 3);
    bool SB = true;//Salida(B, Posicion[B][X], Posicion[B][Y], 0);
    cout << "Salida de A: " << SA << endl;
    cout << "Saluda de B: " << SB << endl;
    system("pause");
    //return true;
    if(SA == true && SB == true) return true; else return false;
}

int ComprobarPosMuro()
{
    Mensaje = " ";
    //Comprobaremos que no haya muros al rededor del nuevo
    int Orientacion = NextMuro[IDXMuro][IDYMuro]; // Si esta en X o Y
    if(PosMuros[IDXMuro][IDYMuro] == NextMuro[IDXMuro][IDYMuro])
    {
        Mensaje = "No puedes poner el muro en esta posición!";
        Beep(700, 250);
        return 0;
    }
    if(Orientacion == Y)
    {
        if((IDYMuro > 0 && IDYMuro < 8) &&
           ((PosMuros[IDXMuro][IDYMuro+1] != -1 && PosMuros[IDXMuro][IDYMuro+1] == Y) ||
            (PosMuros[IDXMuro][IDYMuro-1] != -1 && PosMuros[IDXMuro][IDYMuro-1] == Y) )) // Hay muros arriba, o abajo
        {
            Mensaje = "No puedes poner el muro en esta posición!";
            Beep(700, 250);
            return 0;
        }
    }
    else if(Orientacion == X)
    {
        if(
           (IDXMuro < 8 && PosMuros[IDXMuro+1][IDYMuro] != -1 && PosMuros[IDXMuro+1][IDYMuro] == X) ||
           (IDXMuro > 0 && PosMuros[IDXMuro-1][IDYMuro] != -1 && PosMuros[IDXMuro-1][IDYMuro] == X) ) // Hay muros a la izq, o der
        {
            Mensaje = "No puedes poner el muro en esta posición!";
            Beep(700, 250);
            return 0;
        }
    }
    PosMuros[IDXMuro][IDYMuro] = NextMuro[IDXMuro][IDYMuro]; // Ponemos el muro nuevo
    if(ComprobarSalida()) // Comprobamos que tengamos una salida para los dos jugadores
    {
        cout << "Si";
        system("pause");
        //PosMuros[IDXMuro][IDYMuro] = NextMuro[IDXMuro][IDYMuro]; // Ponemos el muro nuevo
        MurosTotales[Turno]--;
    }
    else
    {
        PosMuros[IDXMuro][IDYMuro] = -1; // Quitamos el que pusimos si bloquea el camino
        cout << "No";
        system("pause");
        Mensaje = "Estas bloqueando el camino. Eso esta prohibido!";
        Beep(700, 250);
        return 0;
    }
    return 1; // No hay error
}

void CambiarFicha()
{
    FichaActual = (FichaActual == PEON)?(MURO):(PEON);
    IDXMuro = IDYMuro = 0;
    memset( &NextMuro[0][0], -1, sizeof(NextMuro) );
    if(FichaActual == MURO)
    {
        if(MurosTotales[Turno] < 1) // Ya no tiene muros
        {
            FichaActual = PEON;
            Mensaje = "Ya no tienes muros disponibles para usar";
        }
        else NextMuro[IDXMuro][IDYMuro] = X;

    }
}

void GameLoop()
{
    int c, MoverCasillas = 1;
    while(true)
    {
        if(kbhit()) // Detecto la presion de una tecla
        {
            c = getch();
            //cout << "Letra: " << c << endl;
            if(c == 27) break; // tecla ESC
            else if(c == 13) // Presiona Enter y hace su movimiento
            {
                if(FichaActual == MURO)
                {
                    int check = ComprobarPosMuro();
                    if(check == 0) // Nope
                    {
                        ImprimirTablero();
                        continue;
                    }
                    else if(check == 1) CambiarFicha(), Turno = NextTurno;
                }
                else
                {
                    int check = ComprobarProximaPos(1);
                    if(check == 1) Turno = NextTurno;
                    else if(check == 2)
                    {
                        // Gana el jugador en Turno
                        ImprimirTablero();
                        break;
                    }
                }
            }
            else // Hacemos esto para evitar que las dos primeras lineas de abajo surtan efecto al presionar ENTER
            {

                if(c == 9) // Presiona TAB :: Cambia entre peon, y muro
                {
                    CambiarFicha();
                    ImprimirTablero();
                    continue;
                }
                if((c == 111 || c == 79) && FichaActual == MURO) // presiona la letra O o o (Mayuscula y minuscula)
                {
                    NextMuro[IDXMuro][IDYMuro] = (NextMuro[IDXMuro][IDYMuro] == X)?(Y):(X);
                    ImprimirTablero();
                    continue;
                }
                if(FichaActual == PEON)
                {
                    MoverCasillas = 1;
                    Mover2Casillas:
                    NextPosi[Turno][X] = Posicion[Turno][X];
                    NextPosi[Turno][Y] = Posicion[Turno][Y];
                    if(c == 72)  // Flecha arriba
                    {
                        NextPosi[Turno][Y] = Posicion[Turno][Y] - MoverCasillas;
                        if(PosMuros[NextPosi[Turno][X]][NextPosi[Turno][Y]] == X || PosMuros[NextPosi[Turno][X]-1][NextPosi[Turno][Y]] == X)
                        {
                           NextPosi[Turno][Y] = Posicion[Turno][Y];
                           Beep(900, 250);

                        }
                    }
                    else if(c == 80) // Flecha abajo
                    {
                        NextPosi[Turno][Y] = Posicion[Turno][Y] + MoverCasillas;
                        if(PosMuros[Posicion[Turno][X]][Posicion[Turno][Y]] == X || PosMuros[Posicion[Turno][X]-1][Posicion[Turno][Y]] == X)
                        {
                            NextPosi[Turno][Y] = Posicion[Turno][Y];
                            Beep(900, 250);
                        }
                    }
                    else if(c == 75) // Felcha Izq
                    {
                        NextPosi[Turno][X] = Posicion[Turno][X] - MoverCasillas;
                        if(PosMuros[NextPosi[Turno][X]][NextPosi[Turno][Y]] == Y || PosMuros[NextPosi[Turno][X]][NextPosi[Turno][Y]-1] == Y)
                        {
                            NextPosi[Turno][X] = Posicion[Turno][X];
                            Beep(900, 250);
                        }

                    }
                    else if(c == 77) // Flecha derecha
                    {
                        NextPosi[Turno][X] = Posicion[Turno][X] + MoverCasillas;
                        if(PosMuros[Posicion[Turno][X]][Posicion[Turno][Y]] == Y || PosMuros[Posicion[Turno][X]][Posicion[Turno][Y]-1] == Y)
                        {
                            NextPosi[Turno][X] = Posicion[Turno][X];
                            Beep(900, 250);
                        }
                    }
                    else continue;
                    ComprobarProximaPos();
                    if(NextPosi[Turno][X] == Posicion[NextTurno][X] && NextPosi[Turno][Y] == Posicion[NextTurno][Y]) // Verificamos si a donde se movera esta el contrincante
                    {
                        MoverCasillas = 2;
                        Mensaje = "Mueves dos casillas saltando al enemigo.";
                        goto Mover2Casillas;
                    }
                }
                else // Esta moviendo el Muro
                {
                    if(c == 75) // Felcha Izq
                    {
                        IDXMuro--;
                        NextMuro[((IDXMuro < 0)?(7):(IDXMuro))][IDYMuro] = NextMuro[IDXMuro+1][IDYMuro]; // Pone la X o Y que tenia anteriormente
                        NextMuro[IDXMuro+1][IDYMuro] = -1; // Al que tenia anterior lo setea en -1 para que no aparezca
                    }
                    else if(c == 77) // Flecha derecha
                    {
                        IDXMuro++;
                        NextMuro[((IDXMuro > 7)?(0):(IDXMuro))][IDYMuro] = NextMuro[IDXMuro-1][IDYMuro]; // Pone la X o Y que tenia anteriormente
                        NextMuro[IDXMuro-1][IDYMuro] = -1; // Al que tenia anterior lo setea en -1 para que no aparezca
                    }
                    else if(c == 72)  // Flecha arriba
                    {
                        IDYMuro--;
                        NextMuro[IDXMuro][((IDYMuro < 0)?(7):(IDYMuro))] = NextMuro[IDXMuro][IDYMuro+1]; // Pone la X o Y que tenia anteriormente
                        NextMuro[IDXMuro][IDYMuro+1] = -1; // Al que tenia anterior lo setea en -1 para que no aparezca
                    }
                    else if(c == 80) // Flecha abajo
                    {
                        IDYMuro++;
                        NextMuro[IDXMuro][((IDYMuro > 7)?(0):(IDYMuro))] = NextMuro[IDXMuro][IDYMuro-1]; // Pone la X o Y que tenia anteriormente
                        NextMuro[IDXMuro][IDYMuro-1] = -1; // Al que tenia anterior lo setea en -1 para que no aparezca
                    }
                    if(IDXMuro < 0) IDXMuro = 7;
                    else if(IDXMuro > 7) IDXMuro = 0;
                    if(IDYMuro > 7) IDYMuro = 0;
                    else if(IDYMuro < 0) IDYMuro = 7;
                }
            }
            ImprimirTablero();
        }
        //Sleep(20);
    }
}

int main ()
{
 //   system("mode 650");
    memset( &PosMuros[0][0], -1, sizeof(PosMuros) );
    memset( &NextMuro[0][0], -1, sizeof(NextMuro) );
    PosMuros[1][1] = X;
    PosMuros[3][1] = X;
    PosMuros[5][1] = X;
    ImprimirTablero();
    GameLoop();
    return 0;
}
