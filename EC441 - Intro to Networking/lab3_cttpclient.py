import socket
import sys
import time

class CalcTextClient(object):
    server_commands = []
    """ CalcTextClient. A Client for the CalcText Transfer Protocol (CTTP).

    This client is provided as-is, you don't have to make any modifications to it
    for this lab. Run this client to connect to a CalcTextServer.

    CalcTextClient can also be used in conjunction with a "replay file", which is
    a text file containing a line-delimited list of commands that will be sent to
    the CalcTextServer automatically. This can be handy for testing a large range
    of protocol features at once.
    """
    def __init__(self, host, port, replay_file=None):
        self.host = host
        self.port = int(port)
        self.replay_file = replay_file
        print("This is CalcTextClient.\n\n" \
              "\u001b[1mLegend:\u001b[0m Server: [\u001b[36mValid CTTP/1.0 Server Response\u001b[0m] [\u001b[31mResponse is not valid CTTP/1.0\u001b[0m]\n" \
              "        Client: [\u001b[33mThis is a client-side warning (FYI)\u001b[0m]\n" \
              "\u001b[1mNOTE:\u001b[0m This client performs basic CTTP/1.0 validation but does NOT warn about incorrect CALC results!\n")
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.connect((self.host, self.port))
        print("You are connected to CalcTextServer {0}:{1} via CTTP/1.0.".format(self.host, self.port))

        # receive the initial welcome message printed by the server
        data = self.socket.recv(1024).decode('utf-8')
        self.print_server(data)

        # Decide whether to switch to file replay mode or shell:
        if self.replay_file:
            self.file_replay_mode()
        else:
            self.shell_mode()
    
        # close the connection
        self.socket.close()

    @classmethod
    def validate_cttp(cls, message):
        # you don't have to understand this function.
        # It is a quick & dirty, probably buggy, rudimentary CTTP/1.0 validator.
        try:
            if not message.startswith("CTTP/1.0 "):
                return (False, "Message does not start with <CTTP-Version>")
            message = message.replace("CTTP/1.0 ", "")
            # parse HELP
            if message.startswith("HELP"):
                return (bool(len(message.split('\n')[1].split(' '))), None)
            # parse CALC
            if message.startswith("CALC"):
                m = message.split('\n')[1]
                try:
                    m = float(m)
                except ValueError:
                    return (False, "Content is not a floating point number")
                return (True, None)
            if message.startswith("BYE"):
                return (False, "Server does not say 'BYE' as per protocol specification")
            if message.startswith("KTHXBYE"):
                return (True, None)
            if message.startswith("ERROR"):
                return (bool(len(message.split('\n')[1])), "Missing error message")
        except Exception:
            pass
        return (False, "Unknown Message Code or format")

    def print_server(self, message):
        """ Decodes server responses to strings and prints them in red color.
        """
        valid, valid_msg = CalcTextClient.validate_cttp(message)
        if valid:
            print("\u001b[36m{0}\u001b[0m".format(message))
        else:
            print("\u001b[33mClient Warning - Invalid CTTP/1.0 Message: {0}!\n\u001b[31m{1}\u001b[0m".format(valid_msg,message))

    def query(self, message):
        # Client automatically adds CTTP-Version for convenience
        if not "CTTP/1.0" in message:
            message = "CTTP/1.0 "+message
        # Send encoded input to server
        messagebytes = message.encode()
        self.socket.sendall(messagebytes)
        # Receive response
        returnbytes =  self.socket.recv(1024)
        return returnbytes.decode('utf-8')

    def shell_mode(self):
        print("Note that the Client automatically adds the CTTP-Version for convenience.")
        print("Type HELP to receive usage help from the Server.")
        response = ""
        # Run the CTTP shell
        while not "KTHXBYE" in response:
            # Display caret symbol and wait for input
            print("> ", end='')
            cmd = input()
            response = self.query(cmd)
            # Pretty-print server response
            self.print_server(response)

    def file_replay_mode(self):
        last_cmd = ""
        with open(self.replay_file) as f:
            while True:
                cmd = f.readline()
                if not cmd: break
                cmd = cmd.strip()
                if not cmd: break
                print(">> {0}".format(cmd))
                response = self.query(cmd)
                last_cmd = cmd
                # Pretty-print server response
                self.print_server(response)
                # Wait before sending next command
                time.sleep(1)
        if not "BYE" in last_cmd:
            response = self.query("BYE")
            self.print_server(response)







if __name__ == "__main__":
    # collect parameters from command line
    host = sys.argv[1] if len(sys.argv) > 1 else 'localhost'
    port = sys.argv[2] if len(sys.argv) > 2 else 24680
    replay_file = sys.argv[3] if len(sys.argv) > 3 else None
    # Set up a CalcTextClient with these parameters
    c = CalcTextClient(host, port, replay_file=replay_file)
    pass