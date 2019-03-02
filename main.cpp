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

int PosMuros[8] = {-1}; //[PosMuro] = X o Y
int NextMuro[8] = {-1};
int Posicion[2][2] = {{4, 0}, {4, 8}}; // X, Y
int NextPosi[2][2] = {{4, 0}, {4, 8}}; // X, Y

int MurosTotales[2] = {5, 5};
bool Turno = A; // False: Turno de A || True: Turno de B
bool FichaActual = PEON;

string Mensaje = "";


void Debug()
{
    cout << "\n";

    /*for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++) cout << "PosMuros[" << i << "][" << j << "]: " << PosMuros[i][j] << endl;
    }*/
    cout << "Posicion[A][X]: " << Posicion[A][X] << " __ NextPosi[A][X]: " << NextPosi[A][X] << endl;
    cout << "Posicion[A][Y]: " << Posicion[A][Y] << " __ NextPosi[A][Y]: " << NextPosi[A][Y] << endl;
    cout << "Posicion[B][X]: " << Posicion[B][X] << " __ NextPosi[B][X]: " << NextPosi[B][X] << endl;
    cout << "Posicion[B][Y]: " << Posicion[B][Y] << " __ NextPosi[B][Y]: " << NextPosi[B][Y] << endl;
    for(int j = 0; j < 8; j++) cout << "PosMuros[" << j << "] = " << PosMuros[j] << ";";
    for(int j = 0; j < 8; j++) cout << "NextMuro[" << j << "] = " << NextMuro[j] << ";";
    cout << "\n";
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



            if(j < 8 && i == j && PosMuros[j] == Y) cout << "#";
            else cout << " "; // Acá puede haber un muro vertical

        }
        cout << "\xBA" << endl; // Saltamos de linea para dejar espacio a los muros horizontales
        cout << " " << i << " \xBA ";
        for(int j = 0; j < 9; j++)
        {
            if(j < 8 && j == i && PosMuros[j] == X) cout << "###";
            else if(j < 8 && j-1 == i && j > 0 && PosMuros[j-1] == X) // Si en la anterior hay muro
            {
                cout << " "; // Acá puede haber un muro horizontal
            }
            else
            {
                if(j < 8 && j == i && PosMuros[j] == Y) cout << " #";
                else cout << "  ";
            }
        }
        cout << "\xBA";
        if(i == 0) cout << "\tEs el turno del jugador " << ( (Turno == A)?("A"):("B") ) << ". Muros Disponibles: " << MurosTotales[Turno];
        if(i == 1) cout << "\t@: La ficha actual es: " << ( (FichaActual == PEON)?("Peon"):("Muro") );
        if(i == 3) cout << "\t" << Mensaje;
        if(i == 4) cout << "\tPara mover el Peon/Muro, usa las flechas";
        if(i == 5) cout << "\tCon el Muro, usa la letra O para cambiar entre vertical y horizontal";
        if(i == 6) cout << "\tPara cambiar entre Peon y Muro, usa TAB";
        if(i == 7) cout << "\tPara terminar el turno y mover el Peon/Muro a la posición elegida, usa ENTER";
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

int ComprobarProximaPos(int DaEnter = 0)
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
    if(DaEnter == 1) // Presiona Enter
    {
        if(NextPosi[Turno][X] == Posicion[Turno][X] && NextPosi[Turno][Y] == Posicion[Turno][Y]) // La posición es igual
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
        else // Movera a la nueva posición
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



void GameLoop()
{
    int c, MoverCasillas = 1;
    while(true)
    {
        if(kbhit()) // Detecto la presion de una tecla
        {
            c = getch();
            if(c == 27) break; // tecla ESC
            else if(c == 13) // Presiona Enter y hace su movimiento
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
            else // Hacemos esto para evitar que las dos primeras lineas de abajo surtan efecto al presionar ENTER
            {

                if(c == 9) // Presiona TAB :: Cambia entre peon, y muro
                {
                    FichaActual = (FichaActual == PEON)?(MURO):(PEON);
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
                        if(FichaActual == PEON) NextPosi[Turno][Y] = Posicion[Turno][Y] - MoverCasillas;
                        //else NextMuro[]
                    }
                    else if(c == 80) // Flecha abajo
                    {
                        NextPosi[Turno][Y] = Posicion[Turno][Y] + MoverCasillas;
                    }
                    else if(c == 75) // Felcha Izq
                    {
                        NextPosi[Turno][X] = Posicion[Turno][X] - MoverCasillas;

                    }
                    else if(c == 77) // Flecha derecha
                    {
                        NextPosi[Turno][X] = Posicion[Turno][X] + MoverCasillas;
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

                }
            }
            ImprimirTablero();

        }
        Sleep(20);
    }
}

int main ()
{
    fill_n(PosMuros, 8, -1);
    fill_n(NextMuro, 8, -1);
    PosMuros[1] = Y;
    ImprimirTablero();
    GameLoop();
    return 0;
}
