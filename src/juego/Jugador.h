#ifndef JUGADOR_H_
#define JUGADOR_H_

#include <stack>
#include <string>
#include <vector>
#include <memory>

#include "Logger.h"

#include "Dealer.h"
#include "Saludador.h"
#include "CanalCartas.h"
#include "SyncBarrier.h"
#include "MesaCompartida.h"
#include "MarcadorCompartido.h"


namespace game {

	class Jugador {
		public:
			Jugador(int idJugador, int cantJugadores, std::vector<int> semIDs, std::string keyCode);
			virtual ~Jugador() = default;

			// Juega el juego, devuelve "0" si es el ganador, "1" si perdió
			int jugar(std::stack<int> cartas, std::string nombreCanal);

		private:
			const int m_idJugador;

			MesaCompartida m_mesa;
			MarcadorCompartido m_marcador;

			std::stack<int> m_cartas;

			utils::SyncBarrier m_jugarCarta;
			utils::SyncBarrier m_chequearCarta;
			Saludador m_saludador;
			utils::SyncBarrier m_chequearTurno;
			utils::SyncBarrier m_chequearFin;

			std::shared_ptr<utils::Logger> m_log;


			bool jugarCarta();
			void ejecutarAtrevido();
			bool finalizarTurno(bool esMiTurno);
	};

}

#endif


