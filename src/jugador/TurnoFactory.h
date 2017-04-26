#ifndef TURNO_FACTORY_H
#define TURNO_FACTORY_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include <memory>
#include <vector>
#include <string>
#include <stdexcept>
#include <system_error>
#include <cerrno>

#include "Turno.h"


namespace player {

	class TurnoFactory {
		public:
			static std::vector< std::unique_ptr<Turno> > buildTurnos(int cant);

		private:
			TurnoFactory() { }
			static void checkError(int res, std::string txt);
	};
}

#endif