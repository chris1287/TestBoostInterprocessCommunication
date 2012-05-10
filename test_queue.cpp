#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/program_options.hpp>
#include <iostream>

using namespace boost::interprocess;
using namespace boost::program_options;
using namespace std;

int main(int argc, char **argv)
{
	char process_type = '\0';

	// ------------------------------
	// - Manage options             -
	// ------------------------------

	options_description desc( "Allowed options" );
	desc.add_options()
		("help,h", "produce help message")
		("process-type,p", value<char>(&process_type), "type of process to start; c = client, s = server.")
		;

	variables_map vm;
	store( parse_command_line(argc, argv, desc), vm );
	notify( vm );

	if(vm.count( "help" ))
	{
		cerr << desc << endl;
		return 1;
	}

	if(process_type == 's')
	{
		// server
		try
		{
			message_queue::remove("test_queue");
			
			message_queue mq
				(
				 	create_only,
					"test_queue",
					10,
					sizeof(int)
				);

			for(int i = 0; i < 10; i++)
				mq.send(&i, sizeof(i), 0);
		}
		catch ( interprocess_exception &ex )
		{
			cerr << ex.what() << endl;
			return 1;
		}
	}
	else if(process_type == 'c')
	{
		// client
		try
		{
			message_queue mq
				(
					open_only,
					"test_queue"
				);

			unsigned int priority;
			size_t recv_size;

			for(int i = 0; i < 10; i++)
			{
				int x;
				mq.receive(&x, sizeof(x), recv_size, priority);
				if(recv_size != sizeof(x))
					return 1;

				cout << x << endl;
			}
		}
		catch ( interprocess_exception &ex )
		{
			// up to the client to remove the queue after reading
			message_queue::remove("test_queue");
			cerr << ex.what() << endl;
			return 1;
		}

		// up to the client to remove the queue after reading
		message_queue::remove("test_queue");
	}
	else
	{
		cerr << "unknown process type: " << process_type << endl;
		return 1;
	}

	return 0;
}
