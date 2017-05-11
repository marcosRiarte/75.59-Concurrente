#include "SemaforoFactory.h"


namespace game {


	std::vector< std::shared_ptr<Turno> > SemaforoFactory::buildTurnos(int cant) {
		// Crea la key para los semaforos
		key_t k = ftok("/bin/ls", 19);
		utils::checkError(k, "Falló la creación de la key");

		// Crea la cantidad de semaforos señalada
		int semID = semget(k, cant, IPC_CREAT | 0644);
		utils::checkError(semID, "Falló la creación de los semaforos");

		// Inicializa todos los semaforos en cero
		for (int i = 0; i < cant; ++i) {
			semun init = { 0 };
			int res = semctl(semID, i, SETVAL, init);
			utils::checkError(res, "Falló la inicializacion de los semaforos");
		}

		// Crea los turnos 
		std::vector< std::shared_ptr<Turno> > v;
		for (int i = 0; i < cant; ++i) {
			v.push_back(std::make_shared<Turno>(semID, i));
		}

		return v;
	}


	void SemaforoFactory::destroyTurnos(std::vector< std::shared_ptr<Turno> > turnos) {
		if (turnos.empty()) {
			return;
		}

		int semID = turnos.front()->getSemId();
		int res = semctl(semID, 0, IPC_RMID);
		utils::checkError(res, "Falló la destruccion de los turnos");
	}


	//---------------------------------------


	std::shared_ptr<Saludador> SemaforoFactory::buildSaludador(int cantJugadores) {
		// Crea las keys
		key_t kSem = ftok("/bin/ls", 20);
		utils::checkError(kSem, "Falló la creación de la clave de semaforos");

		key_t kShm = ftok("/bin/ls", 27);
		utils::checkError(kShm, "Falló la creación de la clave de memoria");

		// Crea e inicializa los semaforos
		int semID = semget(kSem, 2, IPC_CREAT | 0644);
		utils::checkError(semID, "Falló la creación de los semaforos");

		semun initA = { 0 };
		int resA = semctl(semID, 0, SETVAL, initA);
		utils::checkError(resA, "Falló la inicializacion del primer semaforo");

		semun initB = { 1 };
		int resB = semctl(semID, 1, SETVAL, initB);
		utils::checkError(resB, "Falló la inicializacion del segundo semaforo");

		// Crea la memoria compartida
		int shmId = shmget(kShm, sizeof(int), 0644 | IPC_CREAT);
		utils::checkError(shmId, "Falló la creación de la memoria compartida");

		// Devuelve el saludador inicializado dentro de un smart pointer
		std::shared_ptr<Saludador> sal = std::make_shared<Saludador>(semID, shmId);
		sal->init(cantJugadores);

		return sal;
	}


	void SemaforoFactory::destroySaludador(std::shared_ptr<Saludador> s) {
		int res = semctl(s->getSemId(), 0, IPC_RMID);
		utils::checkError(res, "Falló la destruccion del saludador");

		// Destruye la memoria compartida
		shmid_ds stat;
		shmctl(s->getShmId(), IPC_STAT, &stat);
		if (stat.shm_nattch == 1) {	// El main tiene una instancia del saludador
			int r = shmctl(s->getShmId(), IPC_RMID, NULL);
			utils::checkError(r, "Falló la destruccion de la memoria compartida del saludador");
		}
	}


	//---------------------------------------


	Disparador SemaforoFactory::buildDisparador(int cantJugadores) {
		// Crea la key
		key_t kSem = ftok("/bin/ls", 48);
		utils::checkError(kSem, "Falló la creación de la clave del semaforo");

		// Crea e inicializa el semaforo
		int semID = semget(kSem, 1, IPC_CREAT | 0644);

		semun init = { cantJugadores };
		int res = semctl(semID, 0, SETVAL, init);
		utils::checkError(res, "Falló la inicializacion del semaforo");

		Disparador d(semID);

		return d;
	}


	void SemaforoFactory::destroyDisparador(Disparador d) {
		int res = semctl(d.getSemId(), 0, IPC_RMID);
		utils::checkError(res, "Falló la destruccion del disparador");
	}

}

