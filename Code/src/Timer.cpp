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
	//inicio = clock();
	//inicio = std::chrono::steady_clock::now();
	struct timeval tp; gettimeofday(&tp,NULL); inicio = (double) tp.tv_sec +( double) tp.tv_usec / 1000000.0;
}

//Método para detener el cronómetro.
void Timer::stop(){
	if(activado){
		activado = false;
		//fin = clock();
		//fin = std::chrono::steady_clock::now();
		struct timeval tp; gettimeofday(&tp,NULL); fin = (double) tp.tv_sec +( double) tp.tv_usec / 1000000.0;
	}
}

//Método que devuelve el tiempo transcurrido en segundos.
double Timer::get_time() const{
	if(activado){
		//return (clock() - inicio)/ (double)CLOCKS_PER_SEC;
		//return (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now()-inicio).count());
		struct timeval tp; gettimeofday(&tp,NULL); return (double) tp.tv_sec +( double) tp.tv_usec / 1000000.0 - inicio;
	}
	else{
		//return (fin - inicio)/ (double)CLOCKS_PER_SEC;
		//return (std::chrono::duration_cast<std::chrono::seconds>(fin-inicio).count());
		return fin -inicio;
	}
}

//Método que imprime el tiempo transcurrido desde que se inició el cronómetro.
void Timer::message() const{
	cout << "Se ha tardado " << get_time() << " segundos.\n";
}



