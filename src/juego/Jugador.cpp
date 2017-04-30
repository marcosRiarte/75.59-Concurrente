#include "Jugador.h"



namespace game {


	Jugador::Jugador(int id, std::stack<int> cartas, std::shared_ptr<Turno> t, std::shared_ptr<Turno> prox)
		: m_id(id), m_cartas(cartas), m_turno(t), m_turnoProximoJugador(prox), m_cartaPrev(0)
	{
		utils::SignalHandler::getInstance()->registrarHandler(CardCheckHandler::SIG_CARTA_JUGADA , &this->m_cardHandler);
	}


	int Jugador::jugar() {
		while (!this->m_cartas.empty()) {
			msg("Espero mi turno");

			bool esMiTurno = this->m_turno->wait_p();
			if (esMiTurno) {
				msg("Juego una carta: " + this->m_cartas.top());
				this->m_cartas.pop();

				msg("Pasé el turno al siguiente jugador");
				this->m_turnoProximoJugador->signal_v();

			} else {
				msg("Alguien jugó una carta: " + this->m_cardHandler.cartaJugada);
				saludar(this->m_cardHandler.cartaJugada, this->m_cartaPrev);
				this->m_cartaPrev = this->m_cardHandler.cartaJugada;

				// TODO: Escuchar los mensajes de todos los jugadores
				// Podria ser un semaforo que vaya disminuyendo por cada mensaje recibido 
				// hasta llegar a cero y que siga la ejecucion cuando sea cero.
			}
		}

		msg("GANE!");

		return 0;
	}


	void Jugador::saludar(int carta, int cartaPrev) {
		// TODO: Mandar los mensajes al ether
		switch (carta) {
			case 7:
				msg("Atrevido!");
				break;

			case 10:
				msg("Buenos dias señorita");
				break;

			case 11:
				msg("Buenas noches caballero");
				break;

			case 12:
				msg("( ゜ω゜)ゝ");
				break;

			default:
				if (carta == cartaPrev) {
					msg("Atrevido!");
				}
				break;
		}
	}


	void Jugador::msg(std::string txt) {
		std::cout << "\t" << this->m_id << " ==> " << txt << std::endl;
	}


	Jugador::~Jugador() {
		utils::SignalHandler::destruir();
	}

}
