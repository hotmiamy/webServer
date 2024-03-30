#pragma once

#include <sys/poll.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include "Socket.hpp"

class Poll
{
	private:
		std::vector<pollfd> _pollfds;
		std::vector<Socket> _sockets;

	public:
		Poll();
		~Poll();

		void addFd(const Socket &socket,const int &fd);
		void execute();
		bool checkEvent(size_t inx, long diff, bool &timeout);
		void removeEventFd(const Socket &socket);
		void clearAllFds();

		const std::vector<pollfd> &getEvent() const;
		std::size_t getSize();
		const Socket &getSocket(size_t inx) const;
		const int &getFd(int inx) const;

		template <typename ExceptionType>
		void checkerror(int ret, const ExceptionType &exception);

};
