#include "Timer.h"

using namespace std;

//Constructor por defecto para inicializar el cronómetro.
Timer::Timer(){
	//inicio = 0;
	//fin = 0;
	activado = false;
}

//Método para poner en marcha el cronómetro.
void Timer::start(){
	if(!activado){
		activado = true;
	}
	START
}

//Método para detener el cronómetro.
void Timer::stop(){
	if(activado){
		activado = false;
		STOP
	}
}

//Método que devuelve el tiempo transcurrido en segundos.
double Timer::get_time() const{
	if(activado){
		GET_TIME_RUNNING
	}
	else{
		GET_TIME_STOPPED
		
	}
}

//Método que imprime el tiempo transcurrido desde que se inició el cronómetro.
void Timer::message() const{
	cout << "Se ha tardado " << get_time() << " segundos.\n";
}



