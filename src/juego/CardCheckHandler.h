#ifndef CARDCHECK_HANDLER_H
#define CARDCHECK_HANDLER_H

#include <iostream>

#include "EventHandler.h"


namespace game {

	class CardCheckHandler : public utils::EventHandler {
		public:
			virtual int handleSignal(int signum) {
				std::cout << "Señal recibida: " << signum << std::endl;
				return 0;
			}


			virtual ~CardCheckHandler() = default;

	};
}

#endif
