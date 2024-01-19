#ifndef POLL_HPP
# define POLL_HPP

#include <sys/poll.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include "Socket.hpp"

class Poll
{
	private:
		std::vector<pollfd> _pollfds;
	public:
		Poll();
		~Poll();

		void addSocketFd(Socket &socket);
		void execute();
		bool checkEvent(size_t inx);

		size_t getSize() const;

		template <typename ExceptionType>
		void checkerror(int ret, const ExceptionType &exception);

};

#endif