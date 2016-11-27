#include <cstdlib>
#include <iostream>
#include "randomchatbot.h"
using namespace std;

#include <execinfo.h>
#include <signal.h>
#include <string.h>

#include <iostream>
#include <cstdlib>
#include <stdexcept>

#include <typeinfo>

void printStackTrace(void **array, size_t, size_t size)
{
	char ** messages = backtrace_symbols(array, size);
	std::cerr << "Returned " << size << " frames" << endl;
	for (size_t i = 0; i < size && messages != nullptr; ++i)
	{
		char *mangled_name = nullptr, *offset_begin = nullptr, *offset_end = nullptr;

		for (char *p = messages[i]; *p; ++p)
		{
			if (*p == '(')
			{
				mangled_name = p;
			}
			else if (*p == '+')
			{
				offset_begin = p;
			}
			else if (*p == ')')
			{
				offset_end = p;
				break;
			}
		}

		if (mangled_name && offset_begin && offset_end &&
				mangled_name < offset_begin)
		{
			*mangled_name++ = '\0';
			*offset_begin++ = '\0';
			*offset_end++ = '\0';

			int status;
			char * real_name = abi::__cxa_demangle(mangled_name, nullptr, nullptr, &status);

			if (status == 0)
			{
				std::cerr << "backtrace:: (" << i << ") " << messages[i] << " : "
									<< real_name << "+" << offset_begin << offset_end
									<< std::endl;

			}
			else
			{
				std::cerr << "backtrace:: (" << i << ") " << messages[i] << " : "
									<< mangled_name << "+" << offset_begin << offset_end
									<< std::endl;
			}
			free(real_name);
		}
		else
		{
			std::cerr << "backtrace:: (" << i << ") " << messages[i] << std::endl;
		}
	}
	std::cerr << std::endl;
}

void terminateHandler()
{
	static bool tried_throw = false;
	try
	{
		if (!tried_throw++)
			throw;
	}
	catch (const std::exception &e) {
		std::cerr << __FUNCTION__  << " caught unhandled exception. what(): " << e.what() << std::endl <<
								 "Exception name: " << typeid(e).name() << std::endl;
	}
	catch (...)
	{
		std::cerr << __FUNCTION__  << " caught unknown/unhandled exception." << std::endl;
	}
	std::cerr << std::endl;
	void * array[200];
	int size = backtrace(array, 200);
	printStackTrace(array, 200, size);

	abort();
}

// /usr/include/asm/ucontext.h
typedef struct _sig_ucontext
{
	unsigned long     uc_flags;
	struct ucontext   *uc_link;
	stack_t           uc_stack;
	struct sigcontext uc_mcontext;
	sigset_t          uc_sigmask;
} sig_ucontext_t;

void abortSignalHandler(int sig_num, siginfo_t * info, void * ucontext)
{
	sig_ucontext_t * uc = (sig_ucontext_t *)ucontext;

	void * caller_address = (void *) uc->uc_mcontext.rip;

	std::cerr << "signal " << sig_num
						<< " (" << strsignal(sig_num) << "), address is "
						<< info->si_addr << " from " << caller_address
						<< std::endl << std::endl;

	void * array[200];
	int size = backtrace(array, 200);

	array[1] = caller_address;

	printStackTrace(array, 200, size);

	exit(EXIT_FAILURE);
}

bool sigInt = false;
int main(int, char**)
{
	{
		std::set_terminate(terminateHandler);
		struct sigaction sigact;

		sigact.sa_sigaction = abortSignalHandler;
		sigact.sa_flags = SA_RESTART | SA_SIGINFO;

		if (sigaction(SIGABRT, &sigact, (struct sigaction *)NULL) != 0)
		{
			std::cerr << "error setting handler for signal " << SIGABRT
								<< " (" << strsignal(SIGABRT) << ")\n";
			exit(EXIT_FAILURE);
		}
	}
	signal(SIGINT, [](int s)
	{
		printf("\nSIGINT -> %i \n", s);
		sigInt = true;
	});

	signal(SIGSEGV, [](int)
	{
		throw std::runtime_error("[Signal] Segmentation fault");
	});

	std::setlocale(LC_ALL, "C.UTF-8");
	sleep(1);
	cout << "Bot running" << endl;
	RandomChatBot botInstance;

	while(!sigInt)
	{
		botInstance.start();
	}
	return 0;
}

