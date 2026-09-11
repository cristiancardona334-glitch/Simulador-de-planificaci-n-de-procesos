#include <iostream>   // Permite utilizar cout y cin para mostrar y recibir datos
#include <vector>     // Permite almacenar varios procesos en un vector
#include <queue>      // Permite utilizar una cola para el algoritmo Round Robin
#include <string>     // Permite utilizar datos de tipo texto

using namespace std;


// ESTADOS DE UN PROCESO

enum Estado {

    NUEVO,          // El proceso acaba de llegar al sistema
    LISTO,          // El proceso espera su turno para usar la CPU
    EJECUTANDO,     // El proceso está utilizando la CPU
    BLOQUEADO,      // El proceso está temporalmente bloqueado
    TERMINADO       // El proceso terminó completamente
};


// ESTRUCTURA DE UN PROCESO

// Se crea una estructura llamada Proceso.

struct Proceso {

    string id;

    // Identificador del proceso. Ejemplo: P1, P2 o P3.

    string nombre;
    // Nombre de la actividad.

    int llegada;
    // Indica el momento en el que el proceso llega al sistema.

    int prioridad;

    // Indica la prioridad del proceso.En este simulador:
    // 1 = prioridad más alta
    // 2 = prioridad media
    // 3 = prioridad más baja

    int cpu;
    // Cantidad total de tiempo que necesita el proceso para completar su ejecución.

    int restante;
    // Tiempo de CPU que todavía le falta al proceso.

    Estado estado;
    // Guarda el estado actual del proceso.
};


// FUNCIÓN PARA MOSTRAR EL ESTADO

// Esta función recibe un estado y devuelve su nombre en texto.

string mostrarEstado(Estado estado) {

    switch (estado) {

        case NUEVO:
            return "Nuevo";

        case LISTO:
            return "Listo";

        case EJECUTANDO:
            return "En ejecucion";

        case BLOQUEADO:
            return "Bloqueado";

        case TERMINADO:
            return "Terminado";
    }

    // Si por alguna razón el estado no coincide

    return "Desconocido";
}


// MOSTRAR LOS PROCESOS

// Esta función recibe el vector que contiene todos los procesos

void mostrarProcesos(vector<Proceso> procesos) {

    cout << "\n========== PROCESOS DEL SIGET ==========\n";

    // Recorremos todos los procesos uno a uno.

    for (auto &p : procesos) {

        cout << p.id
             << " - " << p.nombre

             // Mostramos el momento de llegada.
             << " | Llegada: " << p.llegada

             // Mostramos la prioridad.
             << " | Prioridad: " << p.prioridad

             // Mostramos el tiempo total de CPU.
             << " | CPU: " << p.cpu

             << endl;
    }
}


// REINICIAR LOS PROCESOS

// Esta función permite volver todos los procesos a su estado inicial.
// Es necesaria porque vamos a ejecutar dos algoritmos:
// 1. Round Robin
// 2. Prioridad
// Si ejecutamos primero Round Robin, los procesos terminan.
// Por eso debemos reiniciarlos antes de ejecutar Prioridad.

void reiniciar(vector<Proceso> &procesos) {

    // Recorremos todos los procesos.
    for (auto &p : procesos) {

        // Restauramos el tiempo restante al tiempo original de CPU.
        p.restante = p.cpu;

        // Todos los procesos vuelven al estado NUEVO.
        p.estado = NUEVO;
    }
}


// ALGORITMO ROUND ROBIN

// Round Robin reparte el tiempo de CPU entre los procesos.

void roundRobin(vector<Proceso> procesos) {

    // Antes de comenzar, dejamos todos los procesos en su estado inicial.
    reiniciar(procesos);


    // Creamos una cola de procesos.
    // El primero que entra es el primero que recibe CPU.

    queue<int> cola;


    // Reloj de la simulación. Comenzamos en el tiempo 0.
    int tiempo = 0;


    // Contador de procesos terminados. Al principio no ha terminado ninguno.
    int terminados = 0;


    // Cantidad máxima de tiempo que un proceso
    int quantum = 2;


    cout << "\n========== ROUND ROBIN ==========\n";


    // El algoritmo continuará funcionando hasta que todos los procesos hayan terminado.
    while (terminados < procesos.size()) {


        // BUSCAR PROCESOS QUE YA HAYAN LLEGADO

        // Revisamos todos los procesos.
        for (int i = 0; i < procesos.size(); i++) {

            // Comprobamos dos condiciones:
            // 1. El proceso todavía está NUEVO.
            // 2. Su tiempo de llegada ya ocurrió.
            
            if (procesos[i].estado == NUEVO &&
                procesos[i].llegada <= tiempo) {


                // El proceso pasa de NUEVO a LISTO.
                procesos[i].estado = LISTO;


                // Agregamos el proceso a la cola.
                cola.push(i);


                cout << "Tiempo " << tiempo
                     << ": P" << i + 1
                     << " -> Listo\n";
            }
        }


        // COMPROBAR SI EXISTE ALGÚN PROCESO LISTO

        if (cola.empty()) {

            // Avanzamos el reloj una unidad.
            tiempo++;

            // Volvemos al inicio del while.
            continue;
        }


        // SELECCIONAR EL PRIMER PROCESO DE LA COLA


        // front() obtiene el primer proceso de la cola.

        int i = cola.front();


        // Sacamos ese proceso de la cola, porque ahora tendrá su turno de CPU.
        cola.pop();


        // EL PROCESO PASA A EJECUTANDO

        procesos[i].estado = EJECUTANDO;


        cout << "Tiempo " << tiempo
             << ": P" << i + 1
             << " -> En ejecucion\n";


        // CALCULAR CUÁNTO TIEMPO VA A EJECUTAR

        int ejecutar = min(quantum, procesos[i].restante);


        // EJECUTAR EL PROCESO UNIDAD POR UNIDAD

        // Este ciclo representa cada unidad de tiempo, que el proceso utiliza la CPU.
        for (int j = 0; j < ejecutar; j++) {


            // Reducimos una unidad del tiempo restante.
            procesos[i].restante--;

            // Avanzamos una unidad el reloj del sistema.
            tiempo++;


            // SIMULAR UN BLOQUEO

            // Aquí simulamos que el proceso necesita esperar temporalmente por una operación de entrada/salida.
            
            if (j == 0 && procesos[i].restante > 0) {


                // El proceso pasa a BLOQUEADO.
                procesos[i].estado = BLOQUEADO;


                cout << "Tiempo " << tiempo
                     << ": P" << i + 1
                     << " -> Bloqueado\n";


                // Después de simular la espera, el proceso vuelve a estar preparado para utilizar la CPU.
                procesos[i].estado = LISTO;


                cout << "Tiempo " << tiempo
                     << ": P" << i + 1
                     << " -> Listo\n";
            }

            // REVISAR SI LLEGARON NUEVOS PROCESOS

            // Mientras un proceso está ejecutándose, otro proceso puede llegar al sistema.
            for (int k = 0; k < procesos.size(); k++) {


                // Comprobamos si:
                // 1. El proceso todavía está NUEVO.
                // 2. Su tiempo de llegada ya ocurrió.

                if (procesos[k].estado == NUEVO &&
                    procesos[k].llegada <= tiempo) {


                    // Lo pasamos a LISTO.
                    procesos[k].estado = LISTO;


                    // Lo agregamos a la cola.
                    cola.push(k);


                    cout << "Tiempo " << tiempo
                         << ": P" << k + 1
                         << " -> Listo\n";
                }
            }
        }

        // COMPROBAR SI EL PROCESO TERMINÓ

        // Si el tiempo restante llegó a cero. 
        if (procesos[i].restante == 0) {


            // Cambiamos el estado a TERMINADO.
            procesos[i].estado = TERMINADO;


            // Aumentamos el contador de procesos terminados.
            terminados++;


            cout << "Tiempo " << tiempo
                 << ": P" << i + 1
                 << " -> Terminado\n";
        }

        // SI NO TERMINÓ, VUELVE A LA COLA

        else {

            // El proceso vuelve a estar LISTO.
            procesos[i].estado = LISTO;


            // Lo colocamos nuevamente al final de la cola.
       
            cola.push(i);


            cout << "Tiempo " << tiempo
                 << ": P" << i + 1
                 << " vuelve a Listo\n";
        }
    }
}


// ALGORITMO DE PLANIFICACIÓN POR PRIORIDAD

// En este algoritmo, el sistema busca el proceso que tenga la prioridad más alta.

void prioridad(vector<Proceso> procesos) {


    // Reiniciamos todos los procesos antes de comenzar.
    reiniciar(procesos);


    // Reloj de la simulación.
    int tiempo = 0;


    // Cantidad de procesos terminados.
    int terminados = 0;


    cout << "\n========== PRIORIDAD ==========\n";


    // Repetimos hasta que todos los procesos terminen.
    while (terminados < procesos.size()) {


        // Esta variable almacenará el proceso seleccionado.

        // -1 significa que todavía no hemos seleccionado ninguno.
        int seleccionado = -1;


        // REVISAR LOS PROCESOS

        for (int i = 0; i < procesos.size(); i++) {


            // Si el proceso es NUEVO y ya llegó, pasa al estado listo.
            if (procesos[i].estado == NUEVO &&
                procesos[i].llegada <= tiempo) {


                procesos[i].estado = LISTO;


                cout << "Tiempo " << tiempo
                     << ": P" << i + 1
                     << " -> Listo\n";
            }

            // BUSCAR EL PROCESO DE MAYOR PRIORIDAD

            // Solamente podemos seleccionar procesos que estén en estado LISTO.
            if (procesos[i].estado == LISTO) {


                if (seleccionado == -1 ||
                    procesos[i].prioridad <
                    procesos[seleccionado].prioridad) {


                    // Guardamos el número del proceso seleccionado.
                    seleccionado = i;
                }
            }
        }


        // SI NO HAY PROCESOS DISPONIBLES

        // Si seleccionado sigue siendo -1, significa que todavía no hay procesos listos.
        if (seleccionado == -1) {


            // Avanzamos el reloj.
            tiempo++;


            // Volvemos a comenzar el ciclo.
            continue;
        }


        // EL PROCESO SELECCIONADO COMIENZA A EJECUTAR

        procesos[seleccionado].estado = EJECUTANDO;


        cout << "Tiempo " << tiempo
             << ": P" << seleccionado + 1
             << " -> En ejecucion\n";


        // EJECUTAR EL PROCESO

        // En este algoritmo el proceso ejecuta todo el tiempo que necesita.

        // No utilizamos quantum como en Round Robin.
        while (procesos[seleccionado].restante > 0) {


            // Consumimos una unidad de CPU.
            procesos[seleccionado].restante--;


            // Avanzamos el tiempo.
            tiempo++;

            // SIMULAR UN BLOQUEO

            // Si el proceso todavía tiene trabajo pendiente
            // y acaba de consumir su primera unidad de CPU,
            // simulamos temporalmente un bloqueo.
            if (procesos[seleccionado].restante > 0 &&
                procesos[seleccionado].restante ==
                procesos[seleccionado].cpu - 1) {


                // El proceso pasa a BLOQUEADO.
                procesos[seleccionado].estado = BLOQUEADO;


                cout << "Tiempo " << tiempo
                     << ": P" << seleccionado + 1
                     << " -> Bloqueado\n";


                // Después del bloqueo simulado, vuelve directamente a EJECUTANDO.
                procesos[seleccionado].estado = EJECUTANDO;


                cout << "Tiempo " << tiempo
                     << ": P" << seleccionado + 1
                     << " -> En ejecucion\n";
            }


            // DETECTAR NUEVOS PROCESOS

            // Mientras el proceso actual utiliza la CPU, pueden llegar nuevos procesos.
            for (int i = 0; i < procesos.size(); i++) {


                // Si el proceso todavía está NUEVO
                // y su tiempo de llegada ya ocurrió...
                if (procesos[i].estado == NUEVO &&
                    procesos[i].llegada <= tiempo) {


                    // Lo pasamos a LISTO.
                    procesos[i].estado = LISTO;


                    cout << "Tiempo " << tiempo
                         << ": P" << i + 1
                         << " -> Listo\n";
                }
            }
        }

        // EL PROCESO TERMINÓ

        // Cuando restante llega a cero, el proceso terminó completamente.
        procesos[seleccionado].estado = TERMINADO;


        // Aumentamos el número de procesos terminados.
        terminados++;


        cout << "Tiempo " << tiempo
             << ": P" << seleccionado + 1
             << " -> Terminado\n";
    }
}

// 8. FUNCIÓN PRINCIPAL

int main() {


    // ========================================================
    // CREACIÓN DE LOS PROCESOS DEL SIGET
    // ========================================================

    // Creamos un vector que contiene tres procesos.
    //
    // Cada proceso tiene:
    //
    // ID
    // Nombre
    // Tiempo de llegada
    // Prioridad
    // Tiempo de CPU
    // Tiempo restante
    // Estado inicial
    vector<Proceso> procesos = {


        // ----------------------------------------------------
        // PROCESO 1
        // ----------------------------------------------------

        {
            "P1",
            "Control semaforico",
            0,
            3,
            5,
            5,
            NUEVO
        },


        // ----------------------------------------------------
        // PROCESO 2
        // ----------------------------------------------------

        {
            "P2",
            "Deteccion de accidente",
            1,
            1,
            3,
            3,
            NUEVO
        },


        // ----------------------------------------------------
        // PROCESO 3
        // ----------------------------------------------------

        {
            "P3",
            "Monitoreo de trafico",
            2,
            2,
            4,
            4,
            NUEVO
        }
    };


    // Variable que almacenará la opción
    // seleccionada por el usuario.
    int opcion;


    // ========================================================
    // MENÚ PRINCIPAL
    // ========================================================

    // do-while permite mostrar el menú repetidamente
    // hasta que el usuario seleccione Salir.
    do {


        cout << "\n====================================\n";
        cout << "     SIMULADOR DE CPU - SIGET\n";
        cout << "====================================\n";


        // Opción 1:
        // Mostrar los procesos.
        cout << "1. Mostrar procesos\n";


        // Opción 2:
        // Ejecutar Round Robin.
        cout << "2. Ejecutar Round Robin\n";


        // Opción 3:
        // Ejecutar planificación por prioridad.
        cout << "3. Ejecutar Prioridad\n";


        // Opción 4:
        // Ejecutar ambos algoritmos.
        cout << "4. Ejecutar ambos algoritmos\n";


        // Opción 5:
        // Finalizar el programa.
        cout << "5. Salir\n";


        // Pedimos al usuario que seleccione una opción.
        cout << "Seleccione una opcion: ";


        // Guardamos la opción introducida.
        cin >> opcion;


        // ====================================================
        // EVALUAR LA OPCIÓN SELECCIONADA
        // ====================================================

        switch (opcion) {


            // ------------------------------------------------
            // OPCIÓN 1
            // ------------------------------------------------

            case 1:

                // Mostramos la información de los procesos.
                mostrarProcesos(procesos);

                break;


            // ------------------------------------------------
            // OPCIÓN 2
            // ------------------------------------------------

            case 2:

                // Ejecutamos solamente Round Robin.
                roundRobin(procesos);

                break;


            // ------------------------------------------------
            // OPCIÓN 3
            // ------------------------------------------------

            case 3:

                // Ejecutamos solamente Prioridad.
                prioridad(procesos);

                break;


            // ------------------------------------------------
            // OPCIÓN 4
            // ------------------------------------------------

            case 4:

                // Ejecutamos primero Round Robin.
                roundRobin(procesos);


                // Después ejecutamos Prioridad. Los procesos se reinician automáticamente
                // dentro de la función prioridad().
                prioridad(procesos);

                break;


            // ------------------------------------------------
            // OPCIÓN 5
            // ------------------------------------------------

            case 5:

                cout << "\nFin del programa.\n";

                break;

            // OPCIÓN NO VÁLIDA

            default:

                // Si el usuario escribe un número diferente
                // de 1, 2, 3, 4 o 5.
                cout << "\nOpcion no valida.\n";
        }


    // El menú continúa apareciendo mientras la opción seleccionada sea diferente de 5.
    } while (opcion != 5);


    return 0;
}
