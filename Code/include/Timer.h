#ifndef TIMER_H
#define TIMER_H
    # include "OS_Timer.h"

    # include <iostream>


#ifdef UNIX_TIMER
    # include <sys/time.h>
    #define DEC_INI double inicio;
    #define DEC_FIN double fin;
    #define START struct timeval tp; gettimeofday(&tp,NULL); inicio = (double) tp.tv_sec +( double) tp.tv_usec / 1000000.0;
    #define STOP  struct timeval tp; gettimeofday(&tp,NULL); fin = (double) tp.tv_sec +( double) tp.tv_usec / 1000000.0;
    #define GET_TIME_RUNNING  struct timeval tp; gettimeofday(&tp,NULL); return (double) tp.tv_sec +( double) tp.tv_usec / 1000000.0 - inicio;
    #define GET_TIME_STOPPED  return fin -inicio;
#endif

#ifdef USUAL_TIMER
    # include <ctime>
    # include <stdlib.h>
    #define DEC_INI time_t inicio;
    #define DEC_FIN time_t fin;
    #define START inicio = clock();
    #define STOP  fin = clock();
    #define GET_TIME_RUNNING return (clock() - inicio)/ (double)CLOCKS_PER_SEC;
    #define GET_TIME_STOPPED return (fin - inicio)/ (double)CLOCKS_PER_SEC;
#endif

#ifdef CPP11_TIMER
    # include <chrono>
    #define DEC_INI std::chrono::steady_clock::time_point inicio;
    #define DEC_FIN std::chrono::steady_clock::time_point fin;
    #define START inicio = std::chrono::steady_clock::now();
    #define STOP  fin = std::chrono::steady_clock::now();
    #define GET_TIME_RUNNING return (std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now()-inicio).count())/1000000000.0;
    #define GET_TIME_STOPPED return (std::chrono::duration_cast<std::chrono::nanoseconds>(fin-inicio).count())/1000000000.0;
#endif
    
    /**
    * @brief Clase que permite evaluar el tiempo que tarda en ejecutarse un algoritmo.
    */
    
    class Timer{

    private:
        /* DATOS MIEMBRO */

        /**
         * @brief Valor que guarda el instante inicial para poder medir el tiempo.
         */
    	DEC_INI

        /**
         * @brief Valor que guarda el instante final para el que se desea medir el tiempo.
         */
        DEC_FIN

        /**
         * @brief Dato miembro que indica si el cronómetro está en marcha o no.
         */
    	bool activado;
    
    public:

        /**
         * @brief Constructor de la clase. Inicializa el cronómetro con valores correctos.
         */
    	Timer();

        /* MÉTODOS PÚBLICOS */

        /**
         * @brief Inicia el cronómetro.
         */
    	void start();

        /**
         * @brief Detiene el cronómetro.
         */
    	void stop();

        /**
         * @brief Devuelve el tiempo en segundos transcurrido desde que se inició el cronómetro.
         * @return Segundos.
         */	
    	double get_time() const;

        /**
         * @brief Indica si el cronómetro está en marcha.
         * @return Valor booleano que será verdadero solo cuando esté activado.
         */
    	inline bool isRunning() const{
            return this->activado;
        }

        /**
         * @brief Imprime un mensaje con el tiempo transcurrido.
         */
        void message() const;
    };

#endif												
